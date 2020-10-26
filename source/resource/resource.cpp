#include "resource/resource.hpp"

#include <filesystem>
#include <functional>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

#include "essential/log.hpp"

#include "render/font.hpp"
#include "gl/shader.hpp"
#include "gl/texture.hpp"

#include "lodepng/lodepng.h"

bool was_identified = false;

template <typename resource_t>
struct resource {
	std::filesystem::directory_entry path;
	std::shared_ptr<resource_t> pointer;

	resource(std::filesystem::directory_entry const &path)
		: path(path), pointer(nullptr) {}
};
std::unordered_map<std::u8string, resource<void>> fragment_shaders;
std::unordered_map<std::u8string, resource<void>> vertex_shaders;
std::unordered_map<std::u8string, resource<void>> geometry_shaders;
std::unordered_map<std::u8string, resource<void>> compute_shaders;
std::unordered_map<std::u8string, resource<void>> tesselation_control_shaders;
std::unordered_map<std::u8string, resource<void>> tesselation_evaluation_shaders;
std::unordered_map<std::u8string, resource<clap::gl::texture::_2d>> textures;
std::unordered_map<std::u8string, resource<clap::render::font>> fonts;
std::unordered_map<std::u8string, resource<void>> unknown;

void identify_shaders(std::filesystem::directory_entry path) {
	for (auto folder : std::filesystem::directory_iterator(path)) {
		if (folder.is_directory()) {
			auto type = clap::gl::detail::convert::to_shader_type_from_string(folder.path().filename().string());
			for (auto entry : std::filesystem::recursive_directory_iterator(folder))
				if (!entry.is_directory()) {
					auto identificator = entry.path().lexically_relative(folder).replace_extension().u8string();

					//clap::log::message::negligible << "A " << type << " shader was identified.";
					//clap::log::info::minor << "Identificator: '" << identificator << "'.";
					//clap::log::info::minor << "Path: '" << entry.path() << "'.";

					switch (type) {
						case clap::gl::shader::type::fragment:
							fragment_shaders.emplace(identificator, entry);
							break;
						case clap::gl::shader::type::vertex:
							vertex_shaders.emplace(identificator, entry);
							break;
						case clap::gl::shader::type::geometry:
							geometry_shaders.emplace(identificator, entry);
							break;
						case clap::gl::shader::type::compute:
							compute_shaders.emplace(identificator, entry);
							break;
						case clap::gl::shader::type::tesselation_control:
							tesselation_control_shaders.emplace(identificator, entry);
							break;
						case clap::gl::shader::type::tesselation_evaluation:
							tesselation_evaluation_shaders.emplace(identificator, entry);
							break;
						default:
							clap::log::warning::major << "Unknown shader type: " << type << ".";
					}
				}
		} else {
			clap::log::warning::major << "Only directories are supported at the root of the shader directory.";
			clap::log::info::major << "'" << path.path() << "' was found instead.";
		}
	}
}
void identify_textures(std::filesystem::directory_entry path) {
	for (auto subpath : std::filesystem::recursive_directory_iterator(path))
		if (!subpath.is_directory()) {
			auto identificator = subpath.path().lexically_relative(path).replace_extension().u8string();

			//clap::log::message::negligible << "A texture was identified.";
			//clap::log::info::minor << "Identificator: '" << identificator << "'.";
			//clap::log::info::minor << "Path: '" << subpath.path() << "'.";

			textures.emplace(identificator, subpath);
		}
}
void identify_fonts(std::filesystem::directory_entry path) {
	for (auto subpath : std::filesystem::recursive_directory_iterator(path))
		if (!subpath.is_directory()) {
			auto identificator = subpath.path().lexically_relative(path).replace_extension().u8string();

			//clap::log::message::negligible << "A font was identified.";
			//clap::log::info::minor << "Identificator: '" << identificator << "'.";
			//clap::log::info::minor << "Path: '" << subpath.path() << "'.";

			fonts.emplace(identificator, subpath);
		}
}
void identify_unknown(std::filesystem::directory_entry path) {
	for (auto subpath : std::filesystem::recursive_directory_iterator(path))
		if (!subpath.is_directory()) {
			auto identificator = subpath.path().lexically_relative(path).replace_extension().u8string();

			//clap::log::message::negligible << "An unknown resource was identified.";
			//clap::log::info::minor << "Identificator: '" << identificator << "'.";
			//clap::log::info::minor << "Path: '" << subpath.path() << "'.";

			unknown.emplace(identificator, subpath);
		}
}

using identify_function_t = std::function<void(std::filesystem::directory_entry)>;
using path_funtion_binding = std::pair<
	std::set<std::filesystem::path>,
	identify_function_t
>;
using path_function_binding_map = std::map<
	std::set<std::filesystem::path>,
	identify_function_t
>;
bool is_one_of(std::filesystem::path const &path,
			   std::set<std::filesystem::path> const &list) {
	for (auto const &entry : list)
		if (path.filename() == entry)
			return true;
	return false;
}
void folderwise_load(std::filesystem::directory_entry const &path,
					 path_function_binding_map const &input,
					 identify_function_t const &on_fail) {
	for (auto pair : input)
		if (is_one_of(path, pair.first)) {
			pair.second(path);
			return;
		}
	on_fail(path);
}

void clap::resource::identify() {
	const std::set<std::filesystem::path> paths = {
		std::filesystem::absolute(u8"resource"),
		std::filesystem::absolute(u8"../resource"),
		std::filesystem::absolute(u8"../../resource"),
		std::filesystem::absolute(u8"../../../resource"),
		std::filesystem::absolute(u8"../../../../resource"),

		std::filesystem::absolute(u8"clap/resource"),
		std::filesystem::absolute(u8"../clap/resource"),
		std::filesystem::absolute(u8"../../clap/resource"),
		std::filesystem::absolute(u8"../../../clap/resource"),
		std::filesystem::absolute(u8"../../../../clap/resource"),

		std::filesystem::absolute(u8"engine/resource"),
		std::filesystem::absolute(u8"../engine/resource"),
		std::filesystem::absolute(u8"../../engine/resource"),
		std::filesystem::absolute(u8"../../../engine/resource"),
		std::filesystem::absolute(u8"../../../../engine/resource"),
	};
	const path_function_binding_map resource_folder_names = {
		{
			std::set<std::filesystem::path>{
				u8"shader", u8"shaders", u8"Shader", u8"Shaders"
			}, &identify_shaders
		},
		{
			std::set<std::filesystem::path>{
				u8"texture", u8"textures", u8"Texture", u8"Textures"
			}, &identify_textures
		},
		{
			std::set<std::filesystem::path>{
				u8"font", u8"fonts", u8"Font", u8"Fonts"
			}, &identify_fonts
		}
	};

	for (auto &path : paths)
		if (std::filesystem::exists(path)) {
			clap::log::message::major << "Identifying resources from '" << path << "'.";

			for (auto &subpath : std::filesystem::directory_iterator(path))
				if (std::filesystem::is_directory(subpath))
					folderwise_load(subpath, resource_folder_names, &identify_unknown);
				else {
					log::warning::major << "Only directories are supported in the root of the resource directory.";
					log::info::major << "'" << subpath.path() << "' was found instead.";
				}
		}

	auto print_lambda = [](auto name, auto obj) {
		if (!obj.empty()) {
			log::info::major << name << " count: " << obj.size() << ".";
			for (auto &entry : obj)
				log::info::minor << '\t' << entry.first;
		}
	};

	log::message::major << "Resource identificaion is complete.";
	print_lambda("Fragment shader", ::fragment_shaders);
	print_lambda("Vertex shader", ::vertex_shaders);
	print_lambda("Geometry shader", ::geometry_shaders);
	print_lambda("Compute shader", ::compute_shaders);
	print_lambda("Tesselation control shader", ::tesselation_control_shaders);
	print_lambda("Tesselation evaluation shader", ::tesselation_evaluation_shaders);
	print_lambda("Texture", ::textures);
	print_lambda("Font", ::fonts);
	print_lambda("Unknown resource", ::unknown);
	was_identified = true;
}
void clap::resource::clear() {
	fragment_shaders.clear();
	vertex_shaders.clear();
	geometry_shaders.clear();
	compute_shaders.clear();
	tesselation_control_shaders.clear();
	tesselation_evaluation_shaders.clear();
	textures.clear();
	fonts.clear();
	::unknown.clear();
	log::message::major << "Resources were cleared.";
	was_identified = false;
}

namespace clap::resource {
	namespace shader {
		detail::storage<std::optional<clap::gl::shader::detail::object>, detail::fragment_shader_t> fragment;
		detail::storage<std::optional<clap::gl::shader::detail::object>, detail::vertex_shader_t> vertex;
		detail::storage<std::optional<clap::gl::shader::detail::object>, detail::geometry_shader_t> geometry;
		detail::storage<std::optional<clap::gl::shader::detail::object>, detail::compute_shader_t> compute;
		detail::storage<std::optional<clap::gl::shader::detail::object>, detail::tesselation_control_shader_t> tesselation_control;
		detail::storage<std::optional<clap::gl::shader::detail::object>, detail::tesselation_evaluation_shader_t> tesselation_evaluation;
	}
	detail::storage<std::shared_ptr<clap::gl::texture::_2d>, detail::texture_t> texture;
	detail::storage<std::shared_ptr<clap::render::font>, detail::font_t> font;
	detail::storage<std::filesystem::directory_entry const *, detail::unknown_t> unknown;
}

template<clap::gl::shader::type type, typename storage_t>
std::optional<clap::gl::shader::detail::object> get_shader(storage_t const &storage, std::u8string const &identificator) {
	auto iterator = storage.find(identificator);
	if (iterator != storage.end()) {
		return clap::gl::shader::from_file(type, iterator->second.path);
	} else {
		clap::log::warning::major << "Requested shader isn't present in resource directories.";
		clap::log::info::major << "Identificator: '" << identificator << "'.";
		return std::nullopt;
	}
}
std::optional<clap::gl::shader::detail::object> clap::resource::detail::storage<std::optional<clap::gl::shader::detail::object>, clap::resource::detail::fragment_shader_t>::operator[](std::u8string const &identificator) {
	return get_shader<clap::gl::shader::type::fragment>(fragment_shaders, identificator);
}
std::optional<clap::gl::shader::detail::object> clap::resource::detail::storage<std::optional<clap::gl::shader::detail::object>, clap::resource::detail::vertex_shader_t>::operator[](std::u8string const &identificator) {
	return get_shader<clap::gl::shader::type::vertex>(vertex_shaders, identificator);
}
std::optional<clap::gl::shader::detail::object> clap::resource::detail::storage<std::optional<clap::gl::shader::detail::object>, clap::resource::detail::geometry_shader_t>::operator[](std::u8string const &identificator) {
	return get_shader<clap::gl::shader::type::geometry>(geometry_shaders, identificator);
}
std::optional<clap::gl::shader::detail::object> clap::resource::detail::storage<std::optional<clap::gl::shader::detail::object>, clap::resource::detail::compute_shader_t>::operator[](std::u8string const &identificator) {
	return get_shader<clap::gl::shader::type::compute>(compute_shaders, identificator);
}
std::optional<clap::gl::shader::detail::object> clap::resource::detail::storage<std::optional<clap::gl::shader::detail::object>, clap::resource::detail::tesselation_control_shader_t>::operator[](std::u8string const &identificator) {
	return get_shader<clap::gl::shader::type::tesselation_control>(tesselation_control_shaders, identificator);
}
std::optional<clap::gl::shader::detail::object> clap::resource::detail::storage<std::optional<clap::gl::shader::detail::object>, clap::resource::detail::tesselation_evaluation_shader_t>::operator[](std::u8string const &identificator) {
	return get_shader<clap::gl::shader::type::tesselation_evaluation>(tesselation_evaluation_shaders, identificator);
}

std::shared_ptr<clap::gl::texture::_2d> clap::resource::detail::storage<std::shared_ptr<clap::gl::texture::_2d>, clap::resource::detail::texture_t>::operator[](std::u8string const &identificator) {
	auto iterator = ::textures.find(identificator);
	if (iterator != ::textures.end()) {
		if (!iterator->second.pointer) {
			std::vector<unsigned char> image_data;
			unsigned width, height;
			unsigned error_code = lodepng::decode(image_data, width, height, iterator->second.path.path());
			if (error_code != 0) {
				clap::log::warning::major << "Error while trying to decode a texture file.";
				clap::log::info::critical << "Error code: " << error_code << '.';
				clap::log::info::critical << lodepng_error_text(error_code);
				return nullptr;
			}

			iterator->second.pointer = std::make_shared<clap::gl::texture::_2d>(image_data.data(), width, height);
			clap::log::message::minor << "A texture (" << image_data.size() << " bytes) was loaded.";
			clap::log::info::major << "Identificator: '" << iterator->first << "'.";
			clap::log::info::minor << "Path: '" << iterator->second.path.path() << "'.";
		}
		return iterator->second.pointer;
	} else {
		clap::log::warning::major << "Requested texture isn't present in resource directories.";
		clap::log::info::minor << "Identificator: '" << identificator << "'.";
		return nullptr;
	}
}

std::shared_ptr<clap::render::font> clap::resource::detail::storage<std::shared_ptr<clap::render::font>, clap::resource::detail::font_t>::operator[](std::u8string const &identificator) {
	auto iterator = ::fonts.find(identificator);
	if (iterator != ::fonts.end()) {
		if (!iterator->second.pointer) {
			auto ptr = new clap::render::font{ clap::render::font::load(iterator->second.path.path()) };
			iterator->second.pointer = std::shared_ptr<clap::render::font>(ptr);
			clap::log::message::minor << "A font (" << iterator->first << ") was loaded.";
			clap::log::info::minor << "Path: '" << iterator->second.path.path() << "'.";
		}
		return iterator->second.pointer;
	} else {
		clap::log::warning::major << "Requested font isn't present in resource directories.";
		clap::log::info::major << "Identificator: '" << identificator << "'.";
		return nullptr;
	}
}

std::filesystem::directory_entry const *clap::resource::detail::storage<std::filesystem::directory_entry const *, clap::resource::detail::font_t>::operator[](std::u8string const &identificator) {
	auto iterator = ::unknown.find(identificator);
	if (iterator != ::unknown.end())
		return &iterator->second.path;
	else
		return nullptr;
}

#include "resource/resource.hpp"

#include <filesystem>
#include <functional>
#include <unordered_map>
#include <set>
#include <string>

#include "essential/log.hpp"

#include "render/font.hpp"
#include "gl/shader.hpp"
#include "gl/texture.hpp"

bool was_identified = false;

template <typename resource_t>
struct resource {
	std::filesystem::directory_entry path;
	std::shared_ptr<resource_t> pointer;

	resource(std::filesystem::directory_entry const &path) 
		: path(path), pointer(nullptr) {}
};
using shader_program_data = std::map<clap::gl::shader::type, std::filesystem::path>;

std::unordered_map<std::u8string, shader_program_data> shader_programs;
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

					clap::log::message::negligible << "A " << type << " shader was identified.";
					clap::log::info::minor << "Identificator: '" << identificator << "'.";
					clap::log::info::minor << "Path: '" << entry.path() << "'.";

					shader_programs[identificator].emplace(type, entry);
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

			clap::log::message::negligible << "A texture was identified.";
			clap::log::info::minor << "Identificator: '" << identificator << "'.";
			clap::log::info::minor << "Path: '" << subpath.path() << "'.";

			textures.emplace(identificator, subpath);

			/*
			std::vector<unsigned char> image_data;
			unsigned width, height;
			unsigned error_code = lodepng::decode(image_data, width, height, subpath.path());
			if (error_code != 0) {
				clap::log::warning::major << "Error while trying to decode a texture file.";
				clap::log::info::critical << "Error code: " << error_code << '.';
				clap::log::info::critical << lodepng_error_text(error_code);
				return;
			} else {
				clap::log::message::minor << "A texture (" << image_data.size() << " bytes) was loaded.";
				clap::log::info::major << "Path: '" << subpath.path() << "'.";
			}

			clap::resource::detail::load_resource(
				subpath.path().lexically_relative(path).replace_extension().u8string(),
				clap::resource::texture::_2d,
				new clap::gl::texture::_2d(image_data.data(), width, height)
			);
			*/
		}
}
void identify_fonts(std::filesystem::directory_entry path) {
	for (auto subpath : std::filesystem::recursive_directory_iterator(path))
		if (!subpath.is_directory()) {
			auto identificator = subpath.path().lexically_relative(path).replace_extension().u8string();

			clap::log::message::negligible << "A font was identified.";
			clap::log::info::minor << "Identificator: '" << identificator << "'.";
			clap::log::info::minor << "Path: '" << subpath.path() << "'.";

			fonts.emplace(identificator, subpath);

			/*
			clap::resource::detail::load_resource(
				subpath.path().lexically_relative(path).replace_extension().u8string(),
				clap::resource::font,
				new clap::render::font{ clap::render::font::load(subpath.path()) }
			);
			clap::log::message::minor << "A font was loaded.";
			clap::log::info::major << "Path: '" << subpath.path() << "'.";
			*/
		}
}
void identify_unknown(std::filesystem::directory_entry path) {
	for (auto subpath : std::filesystem::recursive_directory_iterator(path))
		if (!subpath.is_directory()) {
			auto identificator = subpath.path().lexically_relative(path).replace_extension().u8string();

			clap::log::message::negligible << "An unknown resource was identified.";
			clap::log::info::minor << "Identificator: '" << identificator << "'.";
			clap::log::info::minor << "Path: '" << subpath.path() << "'.";

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

	log::message::major << "Resource identificaion is complete.";
	log::info::major << "Shader program count: " << shader_programs.size() << ".";
	for (auto &entry : shader_programs)
		log::info::minor << '\t' << entry.first;
	log::info::major << "Texture count: " << textures.size() << ".";
	for (auto &entry : textures)
		log::info::minor << '\t' << entry.first;
	log::info::major << "Font count: " << fonts.size() << ".";
	for (auto &entry : fonts)
		log::info::minor << '\t' << entry.first;
	log::info::major << "Unknown resource count: " << unknown.size() << ".";
	for (auto &entry : unknown)
		log::info::minor << '\t' << entry.first;
	was_identified = true;
}
void clap::resource::clear() {
	shader_programs.clear();
	textures.clear();
	fonts.clear();
	unknown.clear();
	log::message::major << "Resources were cleared.";
	was_identified = false;
}
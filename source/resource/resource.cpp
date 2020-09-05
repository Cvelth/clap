#include "resource/resource.hpp"
#include "essential/log.hpp"
#include "gl/shader.hpp"
#include "gl/texture.hpp"
#include "render/font.hpp"

#include <filesystem>
#include <functional>
#include <set>

#include "lodepng/lodepng.h"

static bool was_loaded = false;

void clap::resource::detail::unloaded_resource_check() {
	if (!was_loaded) {
		clap::log::warning::major << "An attempt to access resource before it was loaded.";
		clap::log::info::major << "Call 'clap::resource::load()' before accessing them.";
	}
}
void clap::resource::detail::non_existent_file_error(std::basic_string<char8_t> const &identificator) {
	clap::log::error::major << "Attempting to access a non-existent resource.";
	clap::log::info::major << "Requested name is '" << identificator << "'.";
}

template<typename T> void clap::resource::detail::call_destructor(T *ptr) { delete ptr; }
template void clap::resource::detail::call_destructor<clap::gl::shader::detail::object>(clap::gl::shader::detail::object *);
template void clap::resource::detail::call_destructor<clap::gl::texture::_1d>(clap::gl::texture::_1d *);
template void clap::resource::detail::call_destructor<clap::gl::texture::_2d>(clap::gl::texture::_2d *);
template void clap::resource::detail::call_destructor<clap::gl::texture::_3d>(clap::gl::texture::_3d *);
template void clap::resource::detail::call_destructor<clap::gl::texture::_1d_array>(clap::gl::texture::_1d_array *);
template void clap::resource::detail::call_destructor<clap::gl::texture::_2d_array>(clap::gl::texture::_2d_array *);
template void clap::resource::detail::call_destructor<clap::gl::texture::rectangle>(clap::gl::texture::rectangle *);
template void clap::resource::detail::call_destructor<clap::gl::texture::multisample>(clap::gl::texture::multisample *);
template void clap::resource::detail::call_destructor<clap::gl::texture::multisample_array>(clap::gl::texture::multisample_array *);
template void clap::resource::detail::call_destructor<clap::render::font>(clap::render::font *);

void load_shaders(std::filesystem::directory_entry const &path) {
	for (auto folder : std::filesystem::directory_iterator(path)) {
		if (folder.is_directory()) {
			auto type = clap::gl::detail::convert::to_shader_type_from_string(folder.path().filename().string());
			for (auto entry : std::filesystem::recursive_directory_iterator(folder))
				if (!entry.is_directory()) {
					auto identificator = entry.path().lexically_relative(folder).replace_extension().u8string();

					clap::log::message::minor << "A " << type << " shader was loaded.";
					clap::log::info::major << "Path: '" << entry.path() << "'.";
					auto *object = new clap::gl::shader::detail::object(clap::gl::shader::from_file(type, entry));

					switch (type) {
						case clap::gl::shader::type::fragment:
							clap::resource::detail::load_resource(identificator, clap::resource::shader::fragment, object);
							break;
						case clap::gl::shader::type::vertex:
							clap::resource::detail::load_resource(identificator, clap::resource::shader::vertex, object);
							break;
						case clap::gl::shader::type::geometry:
							clap::resource::detail::load_resource(identificator, clap::resource::shader::geometry, object);
							break;
						case clap::gl::shader::type::compute:
							clap::resource::detail::load_resource(identificator, clap::resource::shader::compute, object);
							break;
						case clap::gl::shader::type::tesselation_control:
							clap::resource::detail::load_resource(identificator, clap::resource::shader::tesselation_control, object);
							break;
						case clap::gl::shader::type::tesselation_evaluation:
							clap::resource::detail::load_resource(identificator, clap::resource::shader::tesselation_evaluation, object);
							break;
						default:
							clap::log::warning::critical << "Unsupported enum value.";
					}
				}
		} else {
			clap::log::warning::major << "Only directories are supported in the root of the shader directory.";
			clap::log::info::major << "'" << path.path() << "' was found instead.";
		}
	}
}

void load_textures(std::filesystem::directory_entry const &path) {
	for (auto subpath : std::filesystem::recursive_directory_iterator(path))
		if (!subpath.is_directory()) {
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
		}
}

void load_fonts(std::filesystem::directory_entry const &path) {
	for (auto subpath : std::filesystem::recursive_directory_iterator(path))
		if (!subpath.is_directory()) {
			clap::resource::detail::load_resource(
				subpath.path().lexically_relative(path).replace_extension().u8string(),
				clap::resource::font,
				new clap::render::font{ clap::render::font::load(subpath.path()) }
			);
			clap::log::message::minor << "A font was loaded.";
			clap::log::info::major << "Path: '" << subpath.path() << "'.";
		}
}

void load_others(std::filesystem::directory_entry const &path) {
	clap::log::warning::major << "Unsupported resource directory was found. It's ignored.";
	clap::log::info::major << "Directory name is '" << path.path().filename() << "'.";
}

using load_function_t = std::function<void(std::filesystem::directory_entry)>;
using path_set_function_pair = std::pair<
	std::set<std::filesystem::path>,
	load_function_t
>;
bool is_one_of(std::filesystem::path const &path, 
			   std::set<std::filesystem::path> const &list) {
	for (auto const &entry : list)
		if (path.filename() == entry)
			return true;
	return false;
}
void folderwise_load(std::filesystem::directory_entry const &path,
					 std::vector<path_set_function_pair> const &input,
					 load_function_t const &on_fail) {
	for (auto pair : input)
		if (is_one_of(path, pair.first)) {
			pair.second(path);
			return;
		}
	on_fail(path);
}

void clap::resource::load() {
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
	const std::vector<path_set_function_pair> resource_folder_names{
		{
			std::set<std::filesystem::path>{
				u8"shader", u8"shaders", u8"Shader", u8"Shaders"
			}, &load_shaders
		},
		{
			std::set<std::filesystem::path>{
				u8"texture", u8"textures", u8"Texture", u8"Textures"
			}, &load_textures
		},
		{
			std::set<std::filesystem::path>{
				u8"font", u8"fonts", u8"Font", u8"Fonts"
			}, &load_fonts
		}
	};

	for (auto &path : paths)
		if (std::filesystem::exists(path)) {
			log::message::major << "Loading resources from '" << path << "'.";

			for (auto &subpath : std::filesystem::directory_iterator(path))
				if (std::filesystem::is_directory(subpath))
					folderwise_load(subpath, resource_folder_names, &load_others);
				else {
					log::warning::major << "Only directories are supported in the root of the resource directory.";
					log::info::major << "'" << subpath.path() << "' was found instead.";
				}
		}

	log::message::minor << "Resource loading has been finished.";
	was_loaded = true;
}

void clap::resource::clear() {
	shader::fragment.clear();
	shader::vertex.clear();
	shader::geometry.clear();
	shader::compute.clear();
	shader::tesselation_control.clear();
	shader::tesselation_evaluation.clear();

	texture::_1d.clear();
	texture::_2d.clear();
	texture::_3d.clear();
	texture::_1d_array.clear();
	texture::_2d_array.clear();
	texture::rectangle.clear();
	texture::multisample.clear();
	texture::multisample_array.clear();

	font.clear();

	was_loaded = false;
}

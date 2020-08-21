#include "resource/resource.hpp"
#include "essential/log.hpp"
#include "gl/shader.hpp"
#include "gl/texture.hpp"
#include "render/font.hpp"

#include <filesystem>
#include <set>

static bool was_loaded = false;

void clap::resource::detail::unloaded_resource_check() {
	if (!was_loaded) {
		clap::log::warning::major << "An attempt to access resource before it was loaded.";
		clap::log::info::major << "Call 'clap::resource::load()' before accessing them.";
	}
}
void clap::resource::detail::non_existent_file_error(std::string const &identificator) {
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
					auto *object = new clap::gl::shader::detail::object(clap::gl::shader::from_file(type, entry.path().string()));
					auto identificator = entry.path().lexically_relative(folder).replace_extension().string();
					switch (type) {
						case clap::gl::shader::type::fragment:
							clap::resource::detail::load_shader(clap::resource::shader::fragment, identificator, object);
							break;
						case clap::gl::shader::type::vertex:
							clap::resource::detail::load_shader(clap::resource::shader::vertex, identificator, object);
							break;
						case clap::gl::shader::type::geometry:
							clap::resource::detail::load_shader(clap::resource::shader::geometry, identificator, object);
							break;
						case clap::gl::shader::type::compute:
							clap::resource::detail::load_shader(clap::resource::shader::compute, identificator, object);
							break;
						case clap::gl::shader::type::tesselation_control:
							clap::resource::detail::load_shader(clap::resource::shader::tesselation_control, identificator, object);
							break;
						case clap::gl::shader::type::tesselation_evaluation:
							clap::resource::detail::load_shader(clap::resource::shader::tesselation_evaluation, identificator, object);
							break;
						default:
							clap::log::warning::critical << "Unsupported enum value.";
					}
				}
		} else {
			clap::log::warning::major << "Only directories are supported in the root of the shader directory.";
			clap::log::info::major << "'" << path.path().string() << "' was found instead.";
		}
	}
}

void load_textures(std::filesystem::directory_entry const &path) {
	for (auto subpath : std::filesystem::recursive_directory_iterator(path))
		if (!subpath.is_directory())
			clap::resource::detail::load_texture(
				subpath.path().string(), 
				subpath.path().lexically_relative(path).replace_extension().string()
			);
}

void load_fonts(std::filesystem::directory_entry const &path) {
	for (auto subpath : std::filesystem::recursive_directory_iterator(path))
		if (!subpath.is_directory())
			clap::resource::detail::load_font(
				subpath.path().string(), 
				subpath.path().lexically_relative(path).replace_extension().string()
			);
}

void load_others(std::filesystem::directory_entry const &path) {
	clap::log::warning::major << "Unsupported resource directory was found. It's ignored.";
	clap::log::info::major << "Directory name is '" << path.path().filename().string() << "'.";
}

void clap::resource::load() {
	const std::set<std::filesystem::path> paths = {
		std::filesystem::absolute("resource"),
		std::filesystem::absolute("../resource"),
		std::filesystem::absolute("../../resource"),
		std::filesystem::absolute("../../../resource"),
		std::filesystem::absolute("../../../../resource"),

		std::filesystem::absolute("clap/resource"),
		std::filesystem::absolute("../clap/resource"),
		std::filesystem::absolute("../../clap/resource"),
		std::filesystem::absolute("../../../clap/resource"),
		std::filesystem::absolute("../../../../clap/resource"),

		std::filesystem::absolute("engine/resource"),
		std::filesystem::absolute("../engine/resource"),
		std::filesystem::absolute("../../engine/resource"),
		std::filesystem::absolute("../../../engine/resource"),
		std::filesystem::absolute("../../../../engine/resource"),
	};
	for (auto &path : paths)
		if (std::filesystem::exists(path)) {
			log::message::major << "Loading resources from '" << path << "'.";

			for (auto &subpath : std::filesystem::directory_iterator(path))
				if (std::filesystem::is_directory(subpath))
					if (subpath.path().filename().string() == "shader")
						load_shaders(subpath);
					else if (subpath.path().filename().string() == "texture")
						load_textures(subpath);
					else if (subpath.path().filename().string() == "font")
						load_fonts(subpath);
					else
						load_others(subpath);
				else {
					log::warning::major << "Only directories are supported in the root of the resource directory.";
					log::info::major << "'" << subpath.path().string() << "' was found instead.";
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

	was_loaded = false;
}

void clap::resource::detail::load_shader(shaders_t &storage, std::string const &name, gl::shader::detail::object *object) {
	storage.insert(std::pair(name, object));
}

void clap::resource::detail::load_font(std::string const &filename, std::string const &font_name) {
	font.insert(std::pair(font_name, new render::font{}));
}

#include "lodepng/lodepng.h"
void clap::resource::detail::load_texture(std::string const &filename, std::string const &texture_name) {
	std::vector<unsigned char> image_data;
	unsigned width, height;
	unsigned error_code = lodepng::decode(image_data, width, height, filename);
	if (error_code != 0) {
		log::warning::major << "Error while trying to decode a texture file.";
		log::info::critical << "Error code: " << error_code << '.';
		log::info::critical << lodepng_error_text(error_code);
		return;
	} else {
		log::message::minor << "A texture (" << image_data.size() << " bytes) was loaded successfully.";
		log::info::major << "Path: '" << filename << "'.";
	}

	texture::_2d.insert(std::pair(texture_name, new gl::texture::_2d(image_data.data(), width, height)));
}

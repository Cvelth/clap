#include "resource/resource.hpp"
#include "essential/log.hpp"
#include "gl/shader.hpp"

#include <filesystem>

static bool was_loaded = false;

clap::gl::shader::detail::object &clap::resource::detail::shaders_t::operator[](std::string const &identificator) {
	if (!was_loaded) {
		log::warning::major << "An attempt to access resource before it was loaded.";
		log::info::major << "Call 'clap::resource::load()' before accessing them.";
	}

	auto found = find(identificator);
	if (found != end())
		return *found->second;
	else {
		log::error::major << "Attempting to access a non-existent shader file.";
		log::info::major << "Requested name is '" << identificator << "'.";
	}
}

void clap::resource::detail::shaders_t::clear() {
	for (auto pair : *this)
		if (pair.second)
			delete pair.second;
	this->shader_storage_t::clear();
}

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
			clap::resource::detail::load_texture(subpath.path().string());
}

void load_others(std::filesystem::directory_entry const &path) {
	clap::log::warning::major << "Unsupported resource directory was found. It's ignored.";
	clap::log::info::major << "Directory name is '" << path.path().filename().string() << "'.";
}

void clap::resource::load() {
	const auto paths = { "resource", "../resource", "../../resource" };
	for (auto &path : paths)
		if (std::filesystem::exists(path)) {
			log::message::major << "Loading resources from '" << path << "'.";

			for (auto &subpath : std::filesystem::directory_iterator(path))
				if (std::filesystem::is_directory(subpath))
					if (subpath.path().filename().string() == "shader")
						load_shaders(subpath);
					else if (subpath.path().filename().string() == "texture")
						load_textures(subpath);
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

	was_loaded = false;
}

void clap::resource::detail::load_shader(shaders_t &storage, std::string const &name, gl::shader::detail::object *object) {
	storage.insert(std::pair(name, object));
}
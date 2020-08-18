#include "resource/resource.hpp"
#include "lodepng/lodepng.h"
#include "essential/log.hpp"

#include "gl/texture.hpp"

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

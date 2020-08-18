#include "gl/texture.hpp"

#include "glad/gl.h"

#include "gl/detail/state.hpp"
#include "essential/log.hpp"

clap::gl::texture::multiple::multiple(size_t count) : count(count) {
	gl::detail::state::ensure_loaded();

	ids = new uint32_t[count];
	glGenTextures(GLsizei(count), ids);
	log::message::minor << "Texture objects (" << count << ") were created.";
}
clap::gl::texture::multiple::~multiple() {
	for (size_t i = 0; i < count; i++)
		while (auto target = gl::detail::state::is_bound(id(i)))
			gl::detail::state::unbind(*target);

	glDeleteTextures(GLsizei(count), ids);
	if (count) delete[] ids;
	log::message::minor << "Texture objects (" << count << ") were destroyed.";
}

clap::gl::texture::detail::indexed clap::gl::texture::multiple::id(size_t index) {
	return detail::indexed(this, index);
}

clap::gl::texture::multiple::multiple(size_t count, uint32_t *ids) : count(count), ids(ids) {
	gl::detail::state::ensure_loaded();

	for (size_t i = 0; i < count; i++)
		if (!glIsTexture(ids[i])) {
			log::warning::critical << "Unable to perform a texture object move operation. "
				"Passed texture seems to be corrupted.";
			return;
		}

	log::message::negligible << "Texture objects (" << count << ") were moved.";
}

void clap::gl::texture::detail::indexed::bind(target target) {
	gl::detail::state::bind(target, std::move(*this));
}

void clap::gl::texture::detail::indexed::data(void *data, unsigned width, unsigned height, target target, 
											  bool should_compress, internal_format internal_format, 
											  external_format external_format, type external_type, 
											  int level) {
	/// TODO
}

clap::gl::texture::detail::indexed::operator bool() const {
	return pointer && index < pointer->count;
}
uint32_t clap::gl::texture::detail::indexed::operator*() const { return pointer->ids[index]; }

clap::gl::texture::detail::indexed::indexed(multiple *pointer, size_t index)
	: pointer(pointer), index(index) {
	if (!*this) {
		log::error::major << "Attempting to access an out of bounds texture object.";
		log::info::major << "Index is " << index << '.';
		log::info::major << "Count is " << pointer->count << '.';
	}
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::texture::target v) {
	switch (v) {
		case clap::gl::texture::target::_1d: return GL_TEXTURE_1D;
		case clap::gl::texture::target::_2d: return GL_TEXTURE_2D;
		case clap::gl::texture::target::_3d: return GL_TEXTURE_3D;
		case clap::gl::texture::target::_1d_array: return GL_TEXTURE_1D_ARRAY;
		case clap::gl::texture::target::_2d_array: return GL_TEXTURE_2D_ARRAY;
		case clap::gl::texture::target::rectangle: return GL_TEXTURE_RECTANGLE;
		case clap::gl::texture::target::cube_map: return GL_TEXTURE_CUBE_MAP;
		case clap::gl::texture::target::cube_map_array: return GL_TEXTURE_CUBE_MAP_ARRAY;
		case clap::gl::texture::target::buffer: return GL_TEXTURE_BUFFER;
		case clap::gl::texture::target::multisample: return GL_TEXTURE_2D_MULTISAMPLE;
		case clap::gl::texture::target::multisample_array: return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::texture::target clap::gl::detail::convert::to_texture_target(GLenum v) {
	switch (v) {
		case GL_TEXTURE_1D: return clap::gl::texture::target::_1d;
		case GL_TEXTURE_2D: return clap::gl::texture::target::_2d;
		case GL_TEXTURE_3D: return clap::gl::texture::target::_3d;
		case GL_TEXTURE_1D_ARRAY: return clap::gl::texture::target::_1d_array;
		case GL_TEXTURE_2D_ARRAY: return clap::gl::texture::target::_2d_array;
		case GL_TEXTURE_RECTANGLE: return clap::gl::texture::target::rectangle;
		case GL_TEXTURE_CUBE_MAP: return clap::gl::texture::target::cube_map;
		case GL_TEXTURE_CUBE_MAP_ARRAY: return clap::gl::texture::target::cube_map_array;
		case GL_TEXTURE_BUFFER: return clap::gl::texture::target::buffer;
		case GL_TEXTURE_2D_MULTISAMPLE: return clap::gl::texture::target::multisample;
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY: return clap::gl::texture::target::multisample_array;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

std::ostream &operator<<(std::ostream &stream, clap::gl::texture::target target) {
	switch (target) {
		case clap::gl::texture::target::_1d:
			stream << "texture::target::_1d"; break;
		case clap::gl::texture::target::_2d:
			stream << "texture::target::_2d"; break;
		case clap::gl::texture::target::_3d:
			stream << "texture::target::_3d"; break;
		case clap::gl::texture::target::_1d_array:
			stream << "texture::target::_1d_array"; break;
		case clap::gl::texture::target::_2d_array:
			stream << "texture::target::_2d_array"; break;
		case clap::gl::texture::target::rectangle:
			stream << "texture::target::rectangle"; break;
		case clap::gl::texture::target::cube_map:
			stream << "texture::target::cube_map"; break;
		case clap::gl::texture::target::cube_map_array:
			stream << "texture::target::cube_map_array"; break;
		case clap::gl::texture::target::buffer:
			stream << "texture::target::buffer"; break;
		case clap::gl::texture::target::multisample:
			stream << "texture::target::multisample"; break;
		case clap::gl::texture::target::multisample_array:
			stream << "texture::target::multisample_array"; break;
		default:
			clap::log::warning::major << "Unsupported enum value.";
	}
	return stream;
}
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
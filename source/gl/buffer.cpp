#include "gl/buffer.hpp"

#include "gl/detail/state.hpp"
#include "essential/log.hpp"

#include "glad/glad.h"

clap::gl::buffer::multiple::multiple(size_t count)
	: count(count), currently_mapped_id(size_t(-1)), currently_mapped_pointer(nullptr) {
	gl::detail::state::ensure_loaded();

	ids = new uint32_t[count];
	glGenBuffers(GLsizei(count), ids);
	log::message::minor << "Buffers (" << count << ") were created.";
}
clap::gl::buffer::multiple::~multiple() {
	if (currently_mapped_pointer)
		id(currently_mapped_id).unmap();

	for (size_t i = 0; i < count; i++)
		while (auto target = gl::detail::state::is_bound(id(i)))
			gl::detail::state::unbind(*target);

	auto temp_vao = gl::detail::state::unbind();

	glDeleteBuffers(GLsizei(count), ids);
	if (count) delete[] ids;
	log::message::minor << "Buffers (" << count << ") were destroyed.";

	if (temp_vao)
		gl::detail::state::bind(std::move(*temp_vao));
}

clap::gl::buffer::detail::indexed clap::gl::buffer::multiple::id(size_t index) {
	return detail::indexed(this, index);
}

clap::gl::buffer::multiple::multiple(size_t count, uint32_t *ids, size_t currently_mapped_id,
									   void *currently_mapped_pointer) : count(count), ids(ids),
										currently_mapped_id(currently_mapped_id),
										currently_mapped_pointer(currently_mapped_pointer) {
	gl::detail::state::ensure_loaded();

	for (size_t i = 0; i < count; i++)
		if (!glIsBuffer(ids[i])) {
			log::warning::critical << "Unable to perform a buffer move operation. "
				"Passed buffer seems to be corrupted.";
			return;
		}

	log::message::negligible << "Buffers (" << count << ") were moved.";
}

void clap::gl::buffer::detail::indexed::bind(target target) {
	gl::detail::state::bind(target, std::move(*this));
}

void clap::gl::buffer::detail::indexed::data(size_t size, usage usage, target target) {
	bind(target);
	glBufferData(gl::detail::convert::to_gl(target), size, nullptr, gl::detail::convert::to_gl(usage));

	log::message::minor << size << " bytes of space were allocated for a buffer bound to '" << target << "'.";
}
void clap::gl::buffer::detail::indexed::data(void *data, size_t size,
											   usage usage, target target) {
	bind(target);
	glBufferData(gl::detail::convert::to_gl(target), size, data, gl::detail::convert::to_gl(usage));

	log::message::minor << size << " bytes of data were put into a buffer bound to '" << target << "'.";
}
void clap::gl::buffer::detail::indexed::subdata(void *data, size_t size,
												  size_t offset, target target) {
	bind(target);
	glBufferSubData(gl::detail::convert::to_gl(target), offset, size, data);

	log::message::minor << size << " bytes of data were put into a buffer bound to '" 
		<< target << "' with offset: " << offset << ".";
}

void *clap::gl::buffer::detail::indexed::map(access access, target target) {
	if (pointer->currently_mapped_pointer)
		if (index == pointer->currently_mapped_id)
			return pointer->currently_mapped_pointer;
		else
			unmap();

	bind(target);
	pointer->currently_mapped_id = index;
	pointer->currently_mapped_pointer = glMapBuffer(gl::detail::convert::to_gl(target),
														   gl::detail::convert::to_gl(access));

	log::message::minor << "A buffer bound to '" << target << "' was mapped.";
	log::info::major << "The address is '" << pointer->currently_mapped_pointer << "'.";

	return pointer->currently_mapped_pointer;
}
void clap::gl::buffer::detail::indexed::unmap(target target) {
	if (pointer->currently_mapped_pointer) {
		bind(target);
		glUnmapBuffer(gl::detail::convert::to_gl(target));
		pointer->currently_mapped_pointer = nullptr;
		log::message::minor << "A buffer bound to '" << target << "' was unmapped.";
	} else
		log::warning::minor << "Attempting to unmap a pointer when none is mapped.";
}

void clap::gl::buffer::detail::indexed::copy(indexed &source, size_t size,
											   size_t read_offset, size_t write_offset) {
	bind(target::copy_write);
	source.bind(target::copy_read);
	glCopyBufferSubData(gl::detail::convert::to_gl(target::copy_read),
						gl::detail::convert::to_gl(target::copy_write),
						read_offset, write_offset, size);

	log::message::minor << "Some data was copied between two buffers.";
}
void clap::gl::buffer::detail::indexed::invalidate(size_t size, size_t offset) {
	glInvalidateBufferSubData(pointer->ids[index], offset, size);
	log::message::minor << size << " bytes of data were invalidated in a buffer "
		"with offset: " << offset << ".";
}
void clap::gl::buffer::detail::indexed::invalidate() {
	glInvalidateBufferData(pointer->ids[index]);
	log::message::minor << "A buffer was invalidated.";
}

clap::gl::buffer::detail::indexed::operator bool() const { return pointer && index < pointer->count; }
uint32_t clap::gl::buffer::detail::indexed::operator*() const { return pointer->ids[index]; }

clap::gl::buffer::detail::indexed::indexed(multiple *pointer, size_t index)
	: pointer(pointer), index(index) {
	if (!*this) {
		log::error::major << "Attempting to access an out of bounds buffer.";
		log::info::major << "Index is " << index << '.';
		log::info::major << "Count is " << pointer->count << '.';
	}
}

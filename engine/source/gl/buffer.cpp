#include "gl/buffer.hpp"

#include "gl/detail/state.hpp"
#include "log.hpp"

#include "glad/glad.h"

GLenum engine::gl::detail::convert::to_gl(engine::gl::buffer::target v) {
	switch (v) {
		case engine::gl::buffer::target::array: return GL_ARRAY_BUFFER;
		case engine::gl::buffer::target::atomic_counter: return GL_ATOMIC_COUNTER_BUFFER;
		case engine::gl::buffer::target::copy_read: return GL_COPY_READ_BUFFER;
		case engine::gl::buffer::target::copy_write: return GL_COPY_WRITE_BUFFER;
		case engine::gl::buffer::target::indirect_dispatch: return GL_DISPATCH_INDIRECT_BUFFER;
		case engine::gl::buffer::target::indirect_draw: return GL_DRAW_INDIRECT_BUFFER;
		case engine::gl::buffer::target::element_array: return GL_ELEMENT_ARRAY_BUFFER;
		case engine::gl::buffer::target::pixel_pack: return GL_PIXEL_PACK_BUFFER;
		case engine::gl::buffer::target::pixel_unpack: return GL_PIXEL_UNPACK_BUFFER;
		case engine::gl::buffer::target::query: return GL_QUERY_BUFFER;
		case engine::gl::buffer::target::shader_storage: return GL_SHADER_STORAGE_BUFFER;
		case engine::gl::buffer::target::texture: return GL_TEXTURE_BUFFER;
		case engine::gl::buffer::target::transform_feedback: return GL_TRANSFORM_FEEDBACK_BUFFER;
		case engine::gl::buffer::target::uniform: return GL_UNIFORM_BUFFER;
	}
	log::error::critical << "Unsupported enum value.";
}
engine::gl::buffer::target engine::gl::detail::convert::to_target(GLenum v) {
	switch (v) {
		case GL_ARRAY_BUFFER: return engine::gl::buffer::target::array;
		case GL_ATOMIC_COUNTER_BUFFER: return engine::gl::buffer::target::atomic_counter;
		case GL_COPY_READ_BUFFER: return engine::gl::buffer::target::copy_read;
		case GL_COPY_WRITE_BUFFER: return engine::gl::buffer::target::copy_write;
		case GL_DISPATCH_INDIRECT_BUFFER: return engine::gl::buffer::target::indirect_dispatch;
		case GL_DRAW_INDIRECT_BUFFER: return engine::gl::buffer::target::indirect_draw;
		case GL_ELEMENT_ARRAY_BUFFER: return engine::gl::buffer::target::element_array;
		case GL_PIXEL_PACK_BUFFER: return engine::gl::buffer::target::pixel_pack;
		case GL_PIXEL_UNPACK_BUFFER: return engine::gl::buffer::target::pixel_unpack;
		case GL_QUERY_BUFFER: return engine::gl::buffer::target::query;
		case GL_SHADER_STORAGE_BUFFER: return engine::gl::buffer::target::shader_storage;
		case GL_TEXTURE_BUFFER: return engine::gl::buffer::target::texture;
		case GL_TRANSFORM_FEEDBACK_BUFFER: return engine::gl::buffer::target::transform_feedback;
		case GL_UNIFORM_BUFFER: return engine::gl::buffer::target::uniform;
	}
	log::error::critical << "Unsupported enum value.";
}

GLenum engine::gl::detail::convert::to_gl(engine::gl::buffer::access v) {
	switch (v) {
		case engine::gl::buffer::access::read_only: return GL_READ_ONLY;
		case engine::gl::buffer::access::read_write: return GL_READ_WRITE;
		case engine::gl::buffer::access::write_only: return GL_WRITE_ONLY;
	}
	log::error::critical << "Unsupported enum value.";
}
engine::gl::buffer::access engine::gl::detail::convert::to_access(GLenum v) {
	switch (v) {
		case GL_READ_ONLY: return engine::gl::buffer::access::read_only;
		case GL_READ_WRITE: return engine::gl::buffer::access::read_write;
		case GL_WRITE_ONLY: return engine::gl::buffer::access::write_only;
	}
	log::error::critical << "Unsupported enum value.";
}

GLenum engine::gl::detail::convert::to_gl(engine::gl::buffer::usage v) {
	switch (v) {
		case engine::gl::buffer::usage::stream_draw: return GL_STREAM_DRAW;
		case engine::gl::buffer::usage::stream_read: return GL_STREAM_READ;
		case engine::gl::buffer::usage::stream_copy: return GL_STREAM_COPY;
		case engine::gl::buffer::usage::static_draw: return GL_STATIC_DRAW;
		case engine::gl::buffer::usage::static_read: return GL_STATIC_READ;
		case engine::gl::buffer::usage::static_copy: return GL_STATIC_COPY;
		case engine::gl::buffer::usage::dynamic_draw: return GL_DYNAMIC_DRAW;
		case engine::gl::buffer::usage::dynamic_read: return GL_DYNAMIC_READ;
		case engine::gl::buffer::usage::dynamic_copy: return GL_DYNAMIC_COPY;
	}
	log::error::critical << "Unsupported enum value.";
}
engine::gl::buffer::usage engine::gl::detail::convert::to_usage(GLenum v) {
	switch (v) {
		case GL_STREAM_DRAW: return engine::gl::buffer::usage::stream_draw;
		case GL_STREAM_READ: return engine::gl::buffer::usage::stream_read;
		case GL_STREAM_COPY: return engine::gl::buffer::usage::stream_copy;
		case GL_STATIC_DRAW: return engine::gl::buffer::usage::static_draw;
		case GL_STATIC_READ: return engine::gl::buffer::usage::static_read;
		case GL_STATIC_COPY: return engine::gl::buffer::usage::static_copy;
		case GL_DYNAMIC_DRAW: return engine::gl::buffer::usage::dynamic_draw;
		case GL_DYNAMIC_READ: return engine::gl::buffer::usage::dynamic_read;
		case GL_DYNAMIC_COPY: return engine::gl::buffer::usage::dynamic_copy;
	}
	log::error::critical << "Unsupported enum value.";
}

std::ostream &operator<<(std::ostream &stream, engine::gl::buffer::target target) {
	switch (target) {
		case engine::gl::buffer::target::array:
			stream << "buffer::target::array"; break;
		case engine::gl::buffer::target::atomic_counter:
			stream << "buffer::target::atomic_counter"; break;
		case engine::gl::buffer::target::copy_read:
			stream << "buffer::target::copy_read"; break;
		case engine::gl::buffer::target::copy_write:
			stream << "buffer::target::copy_write"; break;
		case engine::gl::buffer::target::indirect_dispatch:
			stream << "buffer::target::indirect_dispatch"; break;
		case engine::gl::buffer::target::indirect_draw:
			stream << "buffer::target::indirect_draw"; break;
		case engine::gl::buffer::target::element_array:
			stream << "buffer::target::element_array"; break;
		case engine::gl::buffer::target::pixel_pack:
			stream << "buffer::target::pixel_pack"; break;
		case engine::gl::buffer::target::pixel_unpack:
			stream << "buffer::target::pixel_unpack"; break;
		case engine::gl::buffer::target::query:
			stream << "buffer::target::query"; break;
		case engine::gl::buffer::target::shader_storage:
			stream << "buffer::target::shader_storage"; break;
		case engine::gl::buffer::target::texture:
			stream << "buffer::target::texture"; break;
		case engine::gl::buffer::target::transform_feedback:
			stream << "buffer::target::transform_feedback"; break;
		case engine::gl::buffer::target::uniform:
			stream << "buffer::target::uniform"; break;
		default:
			engine::log::warning::major << "Unsupported enum value.";
	}
	return stream;
}

engine::gl::buffer::multiple::multiple(size_t count)
	: count(count), currently_mapped_id(size_t(-1)), currently_mapped_pointer(nullptr) {
	gl::detail::state::ensure_loaded();

	ids = new uint32_t[count];
	glGenBuffers(GLsizei(count), ids);
	log::message::minor << "Buffers (" << count << ") were created.";
}
engine::gl::buffer::multiple::~multiple() {
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

engine::gl::buffer::detail::indexed engine::gl::buffer::multiple::id(size_t index) {
	return detail::indexed(this, index);
}

engine::gl::buffer::multiple::multiple(size_t count, uint32_t *ids, size_t currently_mapped_id,
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

void engine::gl::buffer::detail::indexed::bind(target target) {
	gl::detail::state::bind(target, std::move(*this));
}

void engine::gl::buffer::detail::indexed::data(size_t size, usage usage, target target) {
	bind(target);
	glBufferData(gl::detail::convert::to_gl(target), size, nullptr, gl::detail::convert::to_gl(usage));

	log::message::minor << size << " bytes of space were allocated for a buffer bound to '" << target << "'.";
}
void engine::gl::buffer::detail::indexed::data(void *data, size_t size,
											   usage usage, target target) {
	bind(target);
	glBufferData(gl::detail::convert::to_gl(target), size, data, gl::detail::convert::to_gl(usage));

	log::message::minor << size << " bytes of data were put into a buffer bound to '" << target << "'.";
}
void engine::gl::buffer::detail::indexed::subdata(void *data, size_t size,
												  size_t offset, target target) {
	bind(target);
	glBufferSubData(gl::detail::convert::to_gl(target), offset, size, data);

	log::message::minor << size << " bytes of data were put into a buffer bound to '" 
		<< target << "' with offset: " << offset << ".";
}

void *engine::gl::buffer::detail::indexed::map(access access, target target) {
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
void engine::gl::buffer::detail::indexed::unmap(target target) {
	if (pointer->currently_mapped_pointer) {
		bind(target);
		glUnmapBuffer(gl::detail::convert::to_gl(target));
		pointer->currently_mapped_pointer = nullptr;
		log::message::minor << "A buffer bound to '" << target << "' was unmapped.";
	} else
		log::warning::minor << "Attempting to unmap a pointer when none is mapped.";
}

void engine::gl::buffer::detail::indexed::copy(indexed &source, size_t size,
											   size_t read_offset, size_t write_offset) {
	bind(target::copy_write);
	source.bind(target::copy_read);
	glCopyBufferSubData(gl::detail::convert::to_gl(target::copy_read),
						gl::detail::convert::to_gl(target::copy_write),
						read_offset, write_offset, size);

	log::message::minor << "Some data was copied between two buffers.";
}
void engine::gl::buffer::detail::indexed::invalidate(size_t size, size_t offset) {
	glInvalidateBufferSubData(pointer->ids[index], offset, size);
	log::message::minor << size << " bytes of data were invalidated in a buffer "
		"with offset: " << offset << ".";
}
void engine::gl::buffer::detail::indexed::invalidate() {
	glInvalidateBufferData(pointer->ids[index]);
	log::message::minor << "A buffer was invalidated.";
}

engine::gl::buffer::detail::indexed::operator bool() const { return pointer && index < pointer->count; }
uint32_t engine::gl::buffer::detail::indexed::operator*() const { return pointer->ids[index]; }

engine::gl::buffer::detail::indexed::indexed(multiple *pointer, size_t index)
	: pointer(pointer), index(index) {
	if (!*this) {
		log::error::major << "Attempting to access an out of bounds buffer.";
		log::info::major << "Index is " << index << '.';
		log::info::major << "Count is " << pointer->count << '.';
	}
}

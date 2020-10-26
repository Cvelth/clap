#pragma once
#include <cstdint>
#include <ostream>
#include <set>
#include <utility>

#include "gl/interface.hpp"
#include "essential/guard.hpp"
#include "essential/stack.hpp"

typedef unsigned int GLenum;
namespace clap::gl::detail {
	class state;
}
namespace clap::gl::buffer::detail {
	class indexed;
}
namespace clap::gl::shader::variable {
	class attribute;
}

namespace clap::gl::vertex {
	class buffer;
	enum class buffer_target {
		array = 0, atomic_counter = 1, copy_read = 2, copy_write = 3,
		indirect_dispatch = 4, indirect_draw = 5, element_array = 6,
		pixel_pack = 7, pixel_unpack = 8, query = 9, shader_storage = 10,
		texture = 11, transform_feedback = 12, uniform = 13, LAST = uniform
	};

	namespace detail {
		struct bind_buffer_callable {
			buffer const &buffer_ref;
			buffer_target const target;
			essential::stack<clap::gl::vertex::buffer const *>::iterator operator()();
		};
		struct unbind_buffer_callable {
			buffer const &buffer_ref;
			buffer_target const target;
			void operator()(essential::stack<clap::gl::vertex::buffer const *>::iterator);
		};

		class buffer_guard : essential::simple_guard<bind_buffer_callable, unbind_buffer_callable> {
		public:
			buffer_guard(buffer const &buffer_ref, buffer_target const target) :
				essential::simple_guard<bind_buffer_callable, unbind_buffer_callable>(
					detail::bind_buffer_callable{ buffer_ref, target },
					detail::unbind_buffer_callable{ buffer_ref, target }) {}
		};
	}

	class buffer : public gl::detail::object_interface {
		friend detail::bind_buffer_callable;
		friend detail::unbind_buffer_callable;
	public:
		enum class access {
			read_only, write_only, read_write
		};
		enum class usage {
			stream_draw, static_draw, dynamic_draw,
			stream_read, static_read, dynamic_read,
			stream_copy, static_copy, dynamic_copy
		};

		explicit buffer();
		~buffer();

		buffer(buffer const &another) = delete;
		buffer(buffer &&another) noexcept : id(another.id), size(size) { another.id = 0; }

		[[nodiscard]] inline detail::buffer_guard bind(buffer_target target = buffer_target::array) const {
			return detail::buffer_guard(*this, target);
		}
		void copy_from(buffer const &another, size_t size, size_t read_offset = 0, size_t write_offset = 0);
		void invalidate(size_t size, size_t offset = 0);
		void invalidate();

		inline void data(size_t _size, usage usage, buffer_target target = buffer_target::array) {
			data(nullptr, _size, usage, target);
		}
		template <typename T>
		inline void data(T *_data, size_t _size, usage usage, buffer_target target = buffer_target::array) {
			data((void *) _data, _size, usage, target);
		}
		template <typename T>
		inline void subdata(T *data, size_t _size, size_t offset = 0,
							buffer_target target = buffer_target::array) {
			subdata((void *) data, _size, offset, target);
		}

		friend inline std::ostream &operator<<(std::ostream &stream, buffer const &object) {
			return stream << "vertex buffer object (vbo, with id = " << object.id << ")";
		}

	protected:
		void data(void *data, size_t _size, usage usage, buffer_target target);
		void subdata(void *data, size_t _size, size_t offset, buffer_target target);

	private:
		uint32_t id;
		size_t size;
	};

	class array;
	namespace detail {
		struct bind_array_callable {
			array const &array_ref;
			essential::stack<clap::gl::vertex::array const *>::iterator operator()();
		};
		struct unbind_array_callable {
			array const &array_ref;
			void operator()(essential::stack<clap::gl::vertex::array const *>::iterator);
		};

		class array_guard : essential::simple_guard<bind_array_callable, unbind_array_callable> {
		public:
			array_guard(array const &array_ref) :
				essential::simple_guard<bind_array_callable, unbind_array_callable>(
					detail::bind_array_callable{ array_ref },
					detail::unbind_array_callable{ array_ref }) {}
		};
	}

	class array : public gl::detail::object_interface {
		friend detail::bind_array_callable;
		friend detail::unbind_array_callable;

		friend buffer;
	public:
		enum class connection {
			points,
			lines, lines_w_adjacency, line_strip, line_strip_w_adjacency, line_loop,
			triangles, triangles_w_adjacency, triangle_strip, triangle_strip_w_adjacency, triangle_fan,
			patches
		};
		enum class index_type {
			unsigned_byte = 1, unsigned_short = 2, unsigned_int = 4
		};

		explicit array();
		~array();

		array(array const &another) = delete;
		array(array &&another) noexcept : id(another.id) { another.id = 0; }

		[[nodiscard]] inline detail::array_guard bind() const {
			return detail::array_guard(*this);
		}
		void attribute_pointer(vertex::buffer &buffer, shader::variable::attribute const &variable,
							   size_t stride = 0, size_t shift = 0);
		void attribute_divisor(shader::variable::attribute const &variable, size_t divisor);
		void attribute_pointer(vertex::buffer &buffer, shader::variable::attribute const &variable,
							   size_t stride, size_t shift, size_t divisor);
		void attribute_pointer(vertex::buffer &buffer,
							   std::initializer_list<shader::variable::attribute> const &variables);

		void draw(connection connection, size_t count, size_t first = 0);
		void draw_indexed(connection connection, size_t count, size_t first = 0,
						  int base_vertex = 0, index_type type = index_type::unsigned_short);
		void draw_indexed(connection connection, size_t start, size_t end, size_t count,
						  size_t first = 0, int base_vertex = 0,
						  index_type type = index_type::unsigned_short);
		void draw_indirect(connection connection, size_t byte_offset = 0);
		void draw_indexed_indirect(connection connection, size_t byte_offset = 0,
								   index_type type = index_type::unsigned_short);
		void draw_multiple(connection connection, size_t drawcount,
						   int const *counts, int const *firsts);
		void draw_multiple_indexed(connection connection, size_t drawcount,
								   int const *counts, int const *byte_offsets,
								   index_type type = index_type::unsigned_short);
		void draw_multiple_indexed(connection connection, size_t drawcount,
								   int const *counts, int const *byte_offsets,
								   int const *base_vertices,
								   index_type type = index_type::unsigned_short);
		void draw_multiple_indirect(connection connection, size_t drawcount,
									size_t byte_offset = 0, size_t stride = 0);
		void draw_multiple_indexed_indirect(connection connection, size_t drawcount,
											size_t byte_offset = 0, size_t stride = 0,
											index_type type = index_type::unsigned_short);

		void draw_instanced(connection connection, size_t count, size_t draw_count,
							size_t first = 0, int base_instance = 0);
		void draw_instanced_indexed(connection connection, size_t count, size_t draw_count,
									size_t first = 0, int base_vertex = 0, int base_instance = 0,
									index_type type = index_type::unsigned_short);

		friend inline std::ostream &operator<<(std::ostream &stream, array const &object) {
			return stream << "vertex array object (vao, with id = " << object.id << ")";
		}

	private:
		uint32_t id;
	};
}

namespace clap::gl::detail::convert {
	GLenum to_gl(clap::gl::vertex::buffer_target v);
	clap::gl::vertex::buffer_target to_buffer_target(GLenum v);

	GLenum to_gl(clap::gl::vertex::buffer::access v);
	clap::gl::vertex::buffer::access to_access(GLenum v);

	GLenum to_gl(clap::gl::vertex::buffer::usage v);
	clap::gl::vertex::buffer::usage to_usage(GLenum v);

	GLenum to_gl(clap::gl::vertex::array::connection v);
	clap::gl::vertex::array::connection to_connection_type(GLenum v);

	GLenum to_gl(clap::gl::vertex::array::index_type v);
	clap::gl::vertex::array::index_type to_index_type(GLenum v);
}

#include <ostream>
std::ostream &operator<<(std::ostream &stream, clap::gl::vertex::buffer_target target);
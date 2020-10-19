#pragma once
#include <cstdint>
#include <set>
#include <utility>

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

namespace clap::gl::buffer {
	enum class target {
		array, atomic_counter, copy_read, copy_write,
		indirect_dispatch, indirect_draw, element_array,
		pixel_pack, pixel_unpack, query, shader_storage,
		texture, transform_feedback, uniform
	};
	enum class access {
		read_only, write_only, read_write
	};
	enum class usage {
		stream_draw, static_draw, dynamic_draw,
		stream_read, static_read, dynamic_read,
		stream_copy, static_copy, dynamic_copy
	};
	class multiple;
	class single;

	namespace detail {
		class indexed {
			friend multiple;
			friend single;
			friend gl::detail::state;
		public:
			indexed(indexed const &other) = delete;
			inline indexed(indexed &&other) noexcept : indexed(other.pointer, other.index) {}

			indexed &operator=(indexed const &other) = delete;
			inline indexed &operator=(indexed &&other) noexcept {
				pointer = other.pointer;
				index = other.index;
				return *this;
			}

			void bind(target target = target::array);
			void *map(access access, target target = target::array);
			void unmap(target target = target::array);
			void copy(indexed &source, size_t size, size_t read_offset = 0, size_t write_offset = 0);
			void invalidate(size_t size, size_t offset = 0);
			void invalidate();

			void data(size_t size, usage usage, target target = target::array);
			template <typename T>
			inline void data(T *_data, size_t size, usage usage, target target = target::array) {
				data((void *) _data, size, usage, target);
			}
			template <typename T>
			inline void subdata(T *data, size_t size, size_t offset = 0,
								target target = target::array) {
				subdata((void *) data, size, offset, target);
			}

			operator bool() const;

		protected:
			explicit indexed(multiple *pointer = nullptr, size_t index = -1);
			uint32_t operator *() const;

			void data(void *data, size_t size, usage usage, target target = target::array);
			void subdata(void *data, size_t size, size_t offset = 0, target target = target::array);

		private:
			multiple *pointer;
			size_t index;
		};
	}

	class multiple {
		friend detail::indexed;
	public:
		explicit multiple(size_t count);
		virtual ~multiple();

		multiple(multiple const &other) = delete;
		multiple(multiple &&other) noexcept
			: multiple(other.count, other.ids,
					   other.currently_mapped_id, other.currently_mapped_pointer) {}

		detail::indexed id(size_t index);
		inline detail::indexed operator[](size_t index) {
			return id(index);
		}

	private:
		multiple(size_t count, uint32_t *ids,
				 size_t currently_mapped_id, void *currently_mapped_pointer);

	private:
		size_t const count;
		uint32_t *ids;

		size_t currently_mapped_id;
		void *currently_mapped_pointer;
	};

	class single : private multiple {
	public:
		inline single() : multiple(1) {}
		inline virtual ~single() {}

		single(single const &other) = delete;
		single(single &&other) noexcept
			: multiple(std::move(other)) {}

		inline operator detail::indexed() {
			return detail::indexed(this, 0);
		}

		inline void bind(target target = target::array) {
			detail::indexed(this, 0).bind(target);
		}
		inline void *map(access access, target target = target::array) {
			return detail::indexed(this, 0).map(access, target);
		}
		inline void unmap(target target = target::array) {
			detail::indexed(this, 0).unmap(target);
		}
		inline void copy(detail::indexed &source, size_t size,
						 size_t read_offset = 0, size_t write_offset = 0) {
			detail::indexed(this, 0).copy(source, size, read_offset, write_offset);
		}
		inline void invalidate(size_t size, size_t offset = 0) {
			detail::indexed(this, 0).invalidate(size, offset);
		}
		inline void invalidate() {
			detail::indexed(this, 0).invalidate();
		}

		inline void data(size_t size, usage usage, target target = target::array) {
			detail::indexed(this, 0).data(size, usage, target);
		}
		template <typename T>
		inline void data(T *data, size_t size, usage usage, target target = target::array) {
			detail::indexed(this, 0).data(data, size, usage, target);
		}
		template <typename T>
		inline void subdata(T *data, size_t size, size_t offset = 0, target target = target::array) {
			detail::indexed(this, 0).subdata((void *) data, size, offset, target);
		}
	};

}
namespace clap::gl::detail::convert {
	GLenum to_gl(clap::gl::buffer::target v);
	clap::gl::buffer::target to_buffer_target(GLenum v);

	GLenum to_gl(clap::gl::buffer::access v);
	clap::gl::buffer::access to_access(GLenum v);

	GLenum to_gl(clap::gl::buffer::usage v);
	clap::gl::buffer::usage to_usage(GLenum v);
}

namespace clap::gl::vertex_array {
	enum class connection {
		points,
		lines, lines_w_adjacency, line_strip, line_strip_w_adjacency, line_loop,
		triangles, triangles_w_adjacency, triangle_strip, triangle_strip_w_adjacency, triangle_fan,
		patches
	};
	enum class index_type {
		unsigned_byte = 1, unsigned_short = 2, unsigned_int = 4
	};
	class multiple;
	class single;

	namespace detail {
		class indexed {
			friend multiple;
			friend single;
			friend gl::detail::state;
		public:
			indexed(indexed const &other) = delete;
			indexed(indexed &&other) noexcept : indexed(other.pointer, other.index) {}

			indexed &operator=(indexed const &other) = delete;
			inline indexed &operator=(indexed &&other) noexcept {
				pointer = other.pointer;
				index = other.index;
				return *this;
			}

			void bind();
			void attribute_pointer(buffer::detail::indexed &&buffer,
								   shader::variable::attribute const &variable,
								   size_t stride, size_t shift);
			inline void attribute_pointer(buffer::detail::indexed &&buffer,
								   shader::variable::attribute const &variable) {
				attribute_pointer(std::move(buffer), variable, 0, 0);
			}
			void attribute_divisor(shader::variable::attribute const &variable, size_t divisor);
			void attribute_pointer(buffer::detail::indexed &&buffer,
								   shader::variable::attribute const &variable,
								   size_t stride, size_t shift, size_t divisor);
			void attribute_pointer(buffer::detail::indexed &&buffer,
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

			operator bool() const;

		protected:
			explicit indexed(multiple *pointer = nullptr, size_t index = -1);
			uint32_t operator *() const;

		private:
			multiple *pointer;
			size_t index;
		};
	}

	class multiple {
		friend detail::indexed;
	public:
		explicit multiple(size_t count);
		virtual ~multiple();

		multiple(multiple const &other) = delete;
		multiple(multiple &&other) noexcept
			: multiple(other.count, other.ids) {}

		detail::indexed id(size_t index);
		inline detail::indexed operator[](size_t index) {
			return id(index);
		}

	protected:
		multiple(size_t count, uint32_t *ids);

	private:
		size_t const count;
		uint32_t *ids;
	};

	class single : private multiple {
	public:
		inline single() : multiple(1) {}
		inline virtual ~single() {}

		single(single const &other) = delete;
		single(single &&other) noexcept
			: multiple(std::move(other)) {}

		inline operator detail::indexed() {
			return detail::indexed(this, 0u);
		}

		inline void bind() {
			return detail::indexed(this, 0u).bind();
		}
		inline void attribute_pointer(buffer::detail::indexed &&buffer,
									  shader::variable::attribute const &variable) {
			return detail::indexed(this, 0u).attribute_pointer(std::move(buffer), variable);
		}
		inline void attribute_pointer(buffer::detail::indexed &&buffer,
									  shader::variable::attribute const &variable,
									  size_t stride, size_t shift) {
			return detail::indexed(this, 0u).attribute_pointer(std::move(buffer),
															   variable, stride, shift);
		}
		inline void attribute_divisor(shader::variable::attribute const &variable, size_t divisor) {
			return detail::indexed(this, 0u).attribute_divisor(variable, divisor);
		}
		inline void attribute_pointer(buffer::detail::indexed &&buffer,
									  shader::variable::attribute const &variable,
									  size_t stride, size_t shift, size_t divisor) {
			return detail::indexed(this, 0u).attribute_pointer(std::move(buffer),
															   variable, stride, shift, divisor);
		}
		inline void attribute_pointer(buffer::detail::indexed &&buffer,
									  std::initializer_list<shader::variable::attribute> const &variables) {
			return detail::indexed(this, 0u).attribute_pointer(std::move(buffer), variables);
		}

		inline void draw(connection connection, size_t _count, size_t first = 0) {
			return detail::indexed(this, 0u).draw(connection, _count, first);
		}
		inline void draw_indexed(connection connection, size_t _count, size_t first = 0,
								 int base_vertex = 0, index_type type = index_type::unsigned_short) {
			return detail::indexed(this, 0u).draw_indexed(connection, _count, first,
														  base_vertex, type);
		}
		inline void draw_indexed(connection connection, size_t start, size_t end, size_t _count,
								 size_t first = 0, int base_vertex = 0,
								 index_type type = index_type::unsigned_short) {
			return detail::indexed(this, 0u).draw_indexed(connection, start, end, _count, first,
														  base_vertex, type);
		}
		inline void draw_indirect(connection connection, size_t byte_offset = 0) {
			return detail::indexed(this, 0u).draw_indirect(connection, byte_offset);
		}
		inline void draw_indexed_indirect(connection connection, size_t byte_offset = 0,
										  index_type type = index_type::unsigned_short) {
			return detail::indexed(this, 0u).draw_indexed_indirect(connection, byte_offset, type);
		}
		inline void draw_multiple(connection connection, size_t drawcount,
								  int const *counts, int const *firsts) {
			return detail::indexed(this, 0u).draw_multiple(connection, drawcount, counts, firsts);
		}
		inline void draw_multiple_indexed(connection connection, size_t drawcount,
										  int const *counts, int const *byte_offsets,
										  index_type type = index_type::unsigned_short) {
			return detail::indexed(this, 0u).draw_multiple_indexed(connection, drawcount, counts,
																   byte_offsets, type);
		}
		inline void draw_multiple_indexed(connection connection, size_t drawcount,
										  int const *counts, int const *byte_offsets,
										  int const *base_vertices,
										  index_type type = index_type::unsigned_short) {
			return detail::indexed(this, 0u).draw_multiple_indexed(connection, drawcount, counts,
																   byte_offsets, base_vertices, type);
		}
		inline void draw_multiple_indirect(connection connection, size_t drawcount,
										   size_t byte_offset = 0, size_t stride = 0) {
			return detail::indexed(this, 0u).draw_multiple_indirect(connection, drawcount,
																	byte_offset, stride);
		}
		inline void draw_multiple_indexed_indirect(connection connection, size_t drawcount,
												   size_t byte_offset = 0, size_t stride = 0,
												   index_type type = index_type::unsigned_short) {
			return detail::indexed(this, 0u).draw_multiple_indexed_indirect(connection, drawcount,
																			byte_offset, stride, type);
		}

		inline void draw_instanced(connection connection, size_t _count, size_t draw_count,
								   size_t first = 0, int base_instance = 0) {
			return detail::indexed(this, 0u).draw_instanced(connection, _count, draw_count,
															first, base_instance);
		}
		inline void draw_instanced_indexed(connection connection, size_t _count, size_t draw_count,
										   size_t first = 0, int base_vertex = 0, int base_instance = 0,
										   index_type type = index_type::unsigned_short) {
			return detail::indexed(this, 0u).draw_instanced_indexed(connection, _count, draw_count,
																	first, base_vertex, base_instance, type);
		}
	};
}
namespace clap::gl::detail::convert {
	GLenum to_gl(clap::gl::vertex_array::connection v);
	clap::gl::vertex_array::connection to_connection_type(GLenum v);

	GLenum to_gl(clap::gl::vertex_array::index_type v);
	clap::gl::vertex_array::index_type to_index_type(GLenum v);
}

#include <ostream>
std::ostream &operator<<(std::ostream &stream, clap::gl::buffer::target target);
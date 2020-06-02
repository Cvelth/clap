#pragma once
#include <cstdint>
#include <set>
#include <utility>

typedef unsigned int GLenum;

namespace engine::gl {
	class state;
	namespace buffer::detail {
		class indexed;
	}
	namespace shader {
		class variable;
	}
	namespace vertex_array {
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
				friend state;
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
									   shader::variable const &variable,
									   size_t stride, size_t shift);
				void attribute_divisor(shader::variable const &variable, size_t divisor);
				void attribute_pointer(buffer::detail::indexed &&buffer,
									   shader::variable const &variable,
									   size_t stride, size_t shift, size_t divisor);
				void attribute_pointer(buffer::detail::indexed &&buffer,
									   std::set<shader::variable> const &variables);

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
			~multiple();

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
										  shader::variable const &variable,
										  size_t stride, size_t shift) {
				return detail::indexed(this, 0u).attribute_pointer(std::move(buffer), 
																   variable, stride, shift);
			}
			inline void attribute_divisor(shader::variable const &variable, size_t divisor) {
				return detail::indexed(this, 0u).attribute_divisor(variable, divisor);
			}
			inline void attribute_pointer(buffer::detail::indexed &&buffer,
										  shader::variable const &variable,
										  size_t stride, size_t shift, size_t divisor) {
				return detail::indexed(this, 0u).attribute_pointer(std::move(buffer), 
																   variable, stride, shift, divisor);
			}
			inline void attribute_pointer(buffer::detail::indexed &&buffer,
										  std::set<shader::variable> const &variables) {
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
	namespace detail::convert {
		GLenum to_gl(engine::gl::vertex_array::connection v);
		engine::gl::vertex_array::connection to_connection_type(GLenum v);

		GLenum to_gl(engine::gl::vertex_array::index_type v);
		engine::gl::vertex_array::index_type to_index_type(GLenum v);
	}
}

#pragma once
#include <cstdint>
#include <utility>

typedef unsigned int GLenum;

namespace engine::gl {
	class state;
	namespace buffer {
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
				friend state;
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
			~multiple();

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
	namespace detail::convert {
		GLenum to_gl(engine::gl::buffer::target v);
		engine::gl::buffer::target to_target(GLenum v);

		GLenum to_gl(engine::gl::buffer::access v);
		engine::gl::buffer::access to_access(GLenum v);

		GLenum to_gl(engine::gl::buffer::usage v);
		engine::gl::buffer::usage to_usage(GLenum v);
	}
}

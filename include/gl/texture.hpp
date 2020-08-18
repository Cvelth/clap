#pragma once
#include <cstdint>
#include <utility>

typedef unsigned int GLenum;
namespace clap::gl::detail {
	class state;
}

namespace clap::gl::texture {
	enum class target {
		_1d, _2d, _3d, _1d_array, _2d_array, 
		rectangle, cube_map, cube_map_array, buffer, 
		multisample, multisample_array
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
			void bind(target target);
			operator bool() const;

		protected:
			explicit indexed(multiple *pointer = nullptr, size_t index = -1);
			uint32_t operator *() const;

		private:
			multiple *pointer;
			size_t index;
		};
	};

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

		inline void bind(target target) {
			return detail::indexed(this, 0u).bind(target);
		}
	};
}

namespace clap::gl::detail::convert {
	GLenum to_gl(clap::gl::texture::target v);
	clap::gl::texture::target to_texture_target(GLenum v);
}

#include <ostream>
std::ostream &operator<<(std::ostream &stream, clap::gl::texture::target target);
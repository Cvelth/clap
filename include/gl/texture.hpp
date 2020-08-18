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
	enum class internal_format {
		depth_component, depth_stencil,

		red, rg, rgb, rgba,

		r8, r8_snorm, r16, r16_snorm,
		rg8, rg8_snorm, rg16, rg16_snorm,
		r3g3b2, 
		rgb4, rgb5, rgb8, rgb8_snorm,
		rgb10, rgb12, rgb16_snorm,
		rgba2, rgba4, rgb5a1, rgba8, rgba8_snorm,
		rgb10a2, rgb10a2ui, rgba12, rgba16,
		srgb8, srgba8,

		r16f, rg16f, rgb16f, rgba16f,
		r32f, rg32f, rgb32f, rgba32f, 
		r11fg1fb10f, rgb9e5,

		r8i, r8ui, r16i, r16ui, r32i, r32ui, 
		rg8i, rg8ui, rg16i, rg16ui, rg32i, rg32ui, 
		rgb8i, rgb8ui, rgb16i, rgb16ui, rgb32i, rgb32ui, 
		rgba8i, rgba8ui, rgba16i, rgba16ui, rgba32i, rgba32ui, 
	};
	enum class external_format {
		r, rg, rgb, bgr, rgba, bgra, 
		
		ri, rgi, rgbi, bgri, rgbai, bgrai,

		stencil_index, depth_component, depth_stencil,

		red = r,
		red_green = rg,
		red_green_blue = rgb,
		blue_green_red = bgr, 
		red_green_blue_alpha = rgba, 
		blue_green_red_alpha = bgra,

		red_int = ri,
		red_green_int = rgi,
		red_green_blue_int = rgbi,
		blue_green_red_int = bgri,
		red_green_blue_alpha_int = rgbai,
		blue_green_red_alpha_int = bgrai,

		r_int = ri, rg_int = rgi, rgb_int = rgbi, 
		brg_int = bgri, rgba_int = rgbai, bgra_int = bgrai
	};
	enum class type {
		unsigned_byte, byte, unsigned_short, _short, unsinged_int, _int, 
		half_float, _float, //half_double, _double,

		unsigned_byte_3_3_2, unsigned_byte_2_3_3_r,

		unsigned_short_5_6_5, unsigned_short_5_6_5_r,
		unsigned_short_4_4_4_4, unsigned_short_4_4_4_4_r,
		unsigned_short_5_5_5_1, unsigned_short_1_5_5_5_r,

		unsigned_int_8_8_8_8, unsigned_int_8_8_8_8_r,
		unsigned_int_10_10_10_2, unsigned_int_2_10_10_10_r
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

			void data(void *data, unsigned width, unsigned height, target target, 
					  bool should_compress = false,
					  internal_format internal_format = internal_format::rgba,
					  external_format external_format = external_format::rgba, 
					  type external_type = type::unsigned_byte, int level = 0);

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

		inline void data(void *data, unsigned width, unsigned height, target target,
						 bool should_compress = false,
						 internal_format internal_format = internal_format::rgba,
						 external_format external_format = external_format::rgba,
						 type external_type = type::unsigned_byte, int level = 0) {
			return detail::indexed(this, 0u).data(data, width, height, target, should_compress,
												  internal_format, external_format, external_type,
												  level);
		}
	};
}

namespace clap::gl::detail::convert {
	GLenum to_gl(clap::gl::texture::target v);
	clap::gl::texture::target to_texture_target(GLenum v);
}

#include <ostream>
std::ostream &operator<<(std::ostream &stream, clap::gl::texture::target target);
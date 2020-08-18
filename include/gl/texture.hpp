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
	enum class external_type {
		unsigned_byte, byte, unsigned_short, _short, unsinged_int, _int,

		half_float, _float,

		unsigned_byte_3_3_2, unsigned_byte_2_3_3_r,

		unsigned_short_5_6_5, unsigned_short_5_6_5_r,
		unsigned_short_4_4_4_4, unsigned_short_4_4_4_4_r,
		unsigned_short_5_5_5_1, unsigned_short_1_5_5_5_r,

		unsigned_int_8_8_8_8, unsigned_int_8_8_8_8_r,
		unsigned_int_10_10_10_2, unsigned_int_2_10_10_10_r
	};

	namespace detail {
		class interface {
			friend gl::detail::state;

		public:
			void bind();

		protected:
			interface(target target, internal_format internal_format = internal_format::rgba);
			virtual ~interface();

			uint32_t operator*() const { return id; }

		protected:
			target target;
			internal_format internal_format;

		private:
			uint32_t id;
		};
	}

	class _1d : public detail::interface {
	public:
		_1d(void *data, size_t width,
			texture::internal_format internal_format = internal_format::rgba,
			external_format external_format = external_format::rgba,
			external_type external_type = external_type::unsigned_byte)
			: _1d(target::_1d, data, width, internal_format, external_format, external_type) {}
		inline virtual ~_1d() {}

		void data(void *data, size_t offset, size_t width, int level = 0,
				  external_format external_format = external_format::rgba,
				  external_type external_type = external_type::unsigned_byte);
		inline void data(void *data,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte) {
			this->data(data, 0, width, 0, external_format, external_type);
		}
	protected:
		_1d(texture::target target, void *data, size_t width,
			texture::internal_format internal_format = internal_format::rgba,
			external_format external_format = external_format::rgba,
			external_type external_type = external_type::unsigned_byte);

	protected:
		const size_t width;
	};

	class _2d : public detail::interface {
	public:
		_2d(void *data, size_t width, size_t height,
			texture::internal_format internal_format = internal_format::rgba,
			external_format external_format = external_format::rgba,
			external_type external_type = external_type::unsigned_byte)
			: _2d(target::_2d, data, width, height, internal_format, external_format, external_type) {}
		inline virtual ~_2d() {}

		void data(void *data, size_t offset_x, size_t offset_y,
				  size_t width, size_t height, int level = 0,
				  external_format external_format = external_format::rgba,
				  external_type external_type = external_type::unsigned_byte);
		inline void data(void *data,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte) {
			this->data(data, 0, 0, width, height, 0, external_format, external_type);
		}
	protected:
		_2d(texture::target target, void *data, size_t width, size_t height,
			texture::internal_format internal_format = internal_format::rgba,
			external_format external_format = external_format::rgba,
			external_type external_type = external_type::unsigned_byte);

	protected:
		const size_t width;
		const size_t height;
	};

	class _3d : public detail::interface {
	public:
		_3d(void *data, size_t width, size_t height, size_t depth,
			texture::internal_format internal_format = internal_format::rgba,
			external_format external_format = external_format::rgba,
			external_type external_type = external_type::unsigned_byte)
			: _3d(target::_3d, data, width, height, depth, internal_format, external_format, external_type) {}
		inline virtual ~_3d() {}

		void data(void *data, size_t offset_x, size_t offset_y, size_t offset_z,
				  size_t width, size_t height, size_t depth, int level = 0,
				  external_format external_format = external_format::rgba,
				  external_type external_type = external_type::unsigned_byte);
		inline void data(void *data,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte) {
			this->data(data, 0, 0, 0, width, height, depth, 0, external_format, external_type);
		}
	protected:
		_3d(texture::target target, void *data, size_t width, size_t height, size_t depth,
			texture::internal_format internal_format = internal_format::rgba,
			external_format external_format = external_format::rgba,
			external_type external_type = external_type::unsigned_byte);

	protected:
		const size_t width;
		const size_t height;
		const size_t depth;
	};

	class _1d_array : public _2d {
	public:
		inline _1d_array(void *data, size_t width, size_t count,
						 texture::internal_format internal_format = internal_format::rgba,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte)
			: _2d(target::_1d_array, data, width, count, internal_format, external_format, external_type) {}
		inline virtual ~_1d_array() {}

		inline void data(void *data, size_t offset_x, size_t offset_c,
						 size_t _width, size_t count, int level = 0,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte) {
			_2d::data(data, offset_x, offset_c, _width, count, level, external_format, external_type);
		}
		inline void data(void *data,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte) {
			this->data(data, 0, 0, width, height, 0, external_format, external_type);
		}
	};

	class _2d_array : public _3d {
	public:
		inline _2d_array(void *data, size_t width, size_t height, size_t count,
						 texture::internal_format internal_format = internal_format::rgba,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte)
			: _3d(target::_2d_array, data, width, height, count, internal_format, external_format, external_type) {}
		inline virtual ~_2d_array() {}

		inline void data(void *data, size_t offset_x, size_t offset_y, size_t offset_c,
						 size_t _width, size_t _height, size_t count, int level = 0,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte) {
			_3d::data(data, offset_x, offset_y, offset_c, _width, _height, count,
					  level, external_format, external_type);
		}
		inline void data(void *data,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte) {
			this->data(data, 0, 0, 0, width, height, depth, 0, external_format, external_type);
		}
	};

	class rectangle : public _2d {
	public:
		inline rectangle(void *data, size_t width, size_t height,
						 texture::internal_format internal_format = internal_format::rgba,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte)
			: _2d(target::rectangle, data, width, height, internal_format, external_format, external_type) {}
		inline virtual ~rectangle() {}

		inline void data(void *data, size_t offset_x, size_t offset_y,
						 size_t _width, size_t _height, int level = 0,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte) {
			_2d::data(data, offset_x, offset_y, _width, _height, level, external_format, external_type);
		}
		inline void data(void *data,
						 external_format external_format = external_format::rgba,
						 external_type external_type = external_type::unsigned_byte) {
			this->data(data, 0, 0, width, height, 0, external_format, external_type);
		}
	};

	class multisample : public detail::interface {
	public:
		multisample(void *data, size_t sample_count, size_t width, size_t height,
					bool are_samples_fixed = false,
					texture::internal_format internal_format = internal_format::rgba);

	protected:
		const size_t width;
		const size_t height;
	};
	class multisample_array : public detail::interface {
		multisample_array(void *data, size_t sample_count, size_t width, size_t height, size_t depth,
						  bool are_samples_fixed = false,
						  texture::internal_format internal_format = internal_format::rgba);

	protected:
		const size_t width;
		const size_t height;
		const size_t depth;
	};

	//Unimplemented.
	class cube_map : public detail::interface {};
	class cube_map_array : public detail::interface {};
	class buffer : public detail::interface {};
}

namespace clap::gl::detail::convert {
	GLenum to_gl(clap::gl::texture::target v);
	clap::gl::texture::target to_texture_target(GLenum v);

	GLenum to_gl(clap::gl::texture::internal_format v);
	clap::gl::texture::internal_format to_internal_format(GLenum v);

	GLenum to_gl(clap::gl::texture::external_format v);
	clap::gl::texture::external_format to_external_format(GLenum v);

	GLenum to_gl(clap::gl::texture::external_type v);
	clap::gl::texture::external_type to_texture_type(GLenum v);
}

#include <ostream>
std::ostream &operator<<(std::ostream &stream, clap::gl::texture::target target);
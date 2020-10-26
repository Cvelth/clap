#pragma once
#include <cstdint>
#include <type_traits>
#include <utility>
#include <variant>

#include "gl/interface.hpp"
#include "essential/guard.hpp"
#include "essential/stack.hpp"

typedef unsigned int GLenum;

namespace clap::gl::texture {
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
	enum class depth_stencil_texture_mode {
		depth_component, stencil_index
	};
	//enum class compare_function {
	//	less_or_equal,
	//	greater_or_equal,
	//	less,
	//	greater,
	//	equal,
	//	not_equal,
	//	always,
	//	never
	//};
	//enum class compare_mode {
	//	compare_with_ref,
	//	none
	//};
	enum class min_filter {
		nearest,
		linear,
		nearest_mipmap_nearest,
		linear_mipmap_nearest,
		nearest_mipmap_linear,
		linear_mipmap_linear
	};
	enum class mag_filter {
		nearest, linear
	};
	enum class wrap {
		clamp_to_edge, clamp_to_border, mirrored_repeat, repeat, mirror_clamp_to_edge
	};

	namespace detail {
		enum class target {
			_1d = 0, _2d = 1, _3d = 2, _1d_array = 3, _2d_array = 4,
			rectangle = 5, cube_map = 6, cube_map_array = 7, buffer = 8,
			multisample = 9, multisample_array = 10,
			LAST = multisample_array
		};

		template<target texture_type>
		constexpr bool needs_width =
			(texture_type == target::_1d) ||
			(texture_type == target::_2d) ||
			(texture_type == target::_3d) ||
			(texture_type == target::_1d_array) ||
			(texture_type == target::_2d_array) ||
			(texture_type == target::rectangle) ||
			(texture_type == target::multisample) ||
			(texture_type == target::multisample_array);

		template<target texture_type>
		constexpr bool needs_height =
			(texture_type == target::_2d) ||
			(texture_type == target::_3d) ||
			(texture_type == target::_2d_array) ||
			(texture_type == target::rectangle) ||
			(texture_type == target::multisample) ||
			(texture_type == target::multisample_array);

		template<target texture_type>
		constexpr bool needs_depth =
			(texture_type == target::_3d);

		template<target texture_type>
		constexpr bool needs_count =
			(texture_type == target::_1d_array) ||
			(texture_type == target::_2d_array) ||
			(texture_type == target::multisample_array);

		template<target texture_type>
		constexpr bool needs_sample_count =
			(texture_type == target::multisample) ||
			(texture_type == target::multisample_array);

		template<target texture_type, bool condition = false> class has_width;
		template<target texture_type> class has_width<texture_type, true> {
		public:
			explicit has_width(size_t value) noexcept : width(value) {}
			size_t get_width() const { return width; }
		protected:
			const size_t width;
		};
		template<target texture_type> class has_width<texture_type, false> {};

		template<target texture_type, bool condition = false> class has_height;
		template<target texture_type> class has_height<texture_type, true> {
		public:
			explicit has_height(size_t value) noexcept : height(value) {}
			size_t get_height() const { return height; }
		protected:
			const size_t height;
		};
		template<target texture_type> class has_height<texture_type, false> {};

		template<target texture_type, bool condition = false> class has_depth;
		template<target texture_type> class has_depth<texture_type, true> {
		public:
			explicit has_depth(size_t value) noexcept : depth(value) {}
			size_t get_depth() const { return depth; }
		protected:
			const size_t depth;
		};
		template<target texture_type> class has_depth<texture_type, false> {};

		template<target texture_type, bool condition = false> class has_count;
		template<target texture_type> class has_count<texture_type, true> {
		public:
			explicit has_count(size_t value) noexcept : count(value) {}
			size_t get_count() const { return count; }
		protected:
			const size_t count;
		};
		template<target texture_type> class has_count<texture_type, false> {};

		template<target texture_type, bool condition = false> class has_sample_count;
		template<target texture_type> class has_sample_count<texture_type, true> {
		public:
			explicit has_sample_count(size_t value) noexcept : sample_count(value) {}
			size_t get_count() const { return sample_count; }
		protected:
			const size_t sample_count;
		};
		template<target texture_type> class has_sample_count<texture_type, false> {};

		template <target texture_type> struct bind_texture_callable;
		template <target texture_type> struct unbind_texture_callable;
		template <target texture_type> class texture_guard;

		template <target texture_type>
		class interface :
			public gl::detail::object_interface,
			public has_width<texture_type, needs_width<texture_type>>,
			public has_height<texture_type, needs_height<texture_type>>,
			public has_depth<texture_type, needs_depth<texture_type>>,
			public has_count<texture_type, needs_count<texture_type>>,
			public has_sample_count<texture_type, needs_sample_count<texture_type>> {

			friend bind_texture_callable<texture_type>;
			friend unbind_texture_callable<texture_type>;
		public:
			const target type = texture_type;

			template <target T = texture_type>
			interface(void *data, size_t width,
					  bool generate_mipmap = true,
					  texture::internal_format internal_format = internal_format::rgba,
					  external_format external_format = external_format::rgba,
					  external_type external_type = external_type::unsigned_byte
					  ) requires (T == target::_1d);
			template <target T = texture_type>
			interface(void *data, size_t width, size_t height,
					  bool generate_mipmap = true,
					  texture::internal_format internal_format = internal_format::rgba,
					  external_format external_format = external_format::rgba,
					  external_type external_type = external_type::unsigned_byte
					  ) requires (T == target::_2d);
			template <target T = texture_type>
			interface(void *data, size_t width, size_t height, size_t depth,
					  bool generate_mipmap = true,
					  texture::internal_format internal_format = internal_format::rgba,
					  external_format external_format = external_format::rgba,
					  external_type external_type = external_type::unsigned_byte
					  ) requires (T == target::_3d);
			template <target T = texture_type>
			interface(void *data, size_t width, size_t count,
					  bool generate_mipmap = true,
					  texture::internal_format internal_format = internal_format::rgba,
					  external_format external_format = external_format::rgba,
					  external_type external_type = external_type::unsigned_byte
					  ) requires (T == target::_1d_array);
			template <target T = texture_type>
			interface(void *data, size_t width, size_t height, size_t count,
					  bool generate_mipmap = true,
					  texture::internal_format internal_format = internal_format::rgba,
					  external_format external_format = external_format::rgba,
					  external_type external_type = external_type::unsigned_byte
					  ) requires (T == target::_2d_array);
			/*
			template <target T = texture_type>
			interface(size_t sample_count, size_t width, size_t height,
					  bool are_samples_fixed = false,
					  texture::internal_format internal_format = internal_format::rgba
					  ) requires (T == target::multisample);
			template <target T = texture_type>
			interface(void *data, size_t sample_count, size_t width, size_t height, size_t count,
					  bool are_samples_fixed = false,
					  texture::internal_format internal_format = internal_format::rgba
					  ) requires (T == target::multisample_array);
			*/

			~interface();

			interface(interface const &another) = delete;

			template <target T = texture_type>
			inline interface(interface &&another) noexcept
				requires (T == target::_1d) :
				id(another.id),
				has_width<texture_type, needs_width<texture_type>>(another.width) {
				another.id = 0;
			}
			template <target T = texture_type>
			inline interface(interface &&another) noexcept
				requires (T == target::_2d || T == target::rectangle) :
				id(another.id),
				has_width<texture_type, needs_width<texture_type>>(another.width),
				has_height<texture_type, needs_height<texture_type>>(another.height) {
				another.id = 0;
			}
			template <target T = texture_type>
			inline interface(interface &&another) noexcept
				requires (T == target::_3d) :
				id(another.id),
				has_width<texture_type, needs_width<texture_type>>(another.width),
				has_height<texture_type, needs_height<texture_type>>(another.height),
				has_depth<texture_type, needs_depth<texture_type>>(another.depth) {
				another.id = 0;
			}
			template <target T = texture_type>
			inline interface(interface &&another) noexcept
				requires (T == target::_1d_array) :
				id(another.id),
				has_width<texture_type, needs_width<texture_type>>(another.width),
				has_count<texture_type, needs_count<texture_type>>(another.count) {
				another.id = 0;
			}
			template <target T = texture_type>
			inline interface(interface &&another) noexcept
				requires (T == target::_2d_array) :
				id(another.id),
				has_width<texture_type, needs_width<texture_type>>(another.width),
				has_height<texture_type, needs_height<texture_type>>(another.height),
				has_count<texture_type, needs_count<texture_type>>(another.count) {
				another.id = 0;
			}
			/*
			template <target T = texture_type>
			inline interface(interface &&another) noexcept
				requires (T == target::multisample) :
				id(another.id),
				has_width<texture_type, needs_width<texture_type>>(another.width),
				has_height<texture_type, needs_height<texture_type>>(another.height),
				has_sample_count<texture_type, needs_sample_count<texture_type>>(another.sample_count) {
				another.id = 0;
			}
			template <target T = texture_type>
			inline interface(interface &&another) noexcept
				requires (T == target::multisample_array) :
				id(another.id),
				has_width<texture_type, needs_width<texture_type>>(another.width),
				has_height<texture_type, needs_height<texture_type>>(another.height),
				has_count<texture_type, needs_count<texture_type>>(another.count),
				has_sample_count<texture_type, needs_sample_count<texture_type>>(another.sample_count) {
				another.id = 0;
			}
			*/

			[[nodiscard]] inline texture_guard<texture_type> bind() const {
				return texture_guard<texture_type>(*this, 1 /* Temporary! To replace with unit id. */);
			}

			template <target T = texture_type>
			void data(void *data, size_t offset_x, size_t width,
					  bool generate_mipmap = true, int level = 0,
					  external_format external_format = external_format::rgba,
					  external_type external_type = external_type::unsigned_byte
			) requires (T == target::_1d);
			template <target T = texture_type>
			inline void data(void *data, bool generate_mipmap = true,
							 external_format external_format = external_format::rgba,
							 external_type external_type = external_type::unsigned_byte
			) requires (T == target::_1d) {
				this->data(data, 0, this->width, generate_mipmap, 0, external_format, external_type);
			}
			template <target T = texture_type>
			void data(void *data, size_t offset_x, size_t offset_y, size_t width, size_t height,
					  bool generate_mipmap = true, int level = 0,
					  external_format external_format = external_format::rgba,
					  external_type external_type = external_type::unsigned_byte
			) requires (T == target::_2d);
			template <target T = texture_type>
			inline void data(void *data, bool generate_mipmap = true,
							 external_format external_format = external_format::rgba,
							 external_type external_type = external_type::unsigned_byte
			) requires (T == target::_2d) {
				this->data(data, 0, 0, this->width, this->height, generate_mipmap, 0, external_format, external_type);
			}
			template <target T = texture_type>
			void data(void *data, size_t offset_x, size_t offset_y, size_t offset_z,
					  size_t width, size_t height, size_t depth,
					  bool generate_mipmap = true, int level = 0,
					  external_format external_format = external_format::rgba,
					  external_type external_type = external_type::unsigned_byte
			) requires (T == target::_3d);
			template <target T = texture_type>
			inline void data(void *data, bool generate_mipmap = true,
							 external_format external_format = external_format::rgba,
							 external_type external_type = external_type::unsigned_byte
			) requires (T == target::_3d) {
				this->data(data, 0, 0, 0, this->width, this->height, this->depth, generate_mipmap, 0, external_format, external_type);
			}
			template <target T = texture_type>
			void data(void *data, size_t offset_x, size_t offset_c, size_t width, size_t count,
					  bool generate_mipmap = true, int level = 0,
					  external_format external_format = external_format::rgba,
					  external_type external_type = external_type::unsigned_byte
			) requires (T == target::_1d_array);
			template <target T = texture_type>
			inline void data(void *data, bool generate_mipmap = true,
							 external_format external_format = external_format::rgba,
							 external_type external_type = external_type::unsigned_byte
			) requires (T == target::_1d_array) {
				this->data(data, 0, 0, this->width, this->count, generate_mipmap, 0, external_format, external_type);
			}
			template <target T = texture_type>
			void data(void *data, size_t offset_x, size_t offset_y, size_t offset_c,
					  size_t width, size_t height, size_t count,
					  bool generate_mipmap = true, int level = 0,
					  external_format external_format = external_format::rgba,
					  external_type external_type = external_type::unsigned_byte
			) requires (T == target::_2d_array);
			template <target T = texture_type>
			inline void data(void *data, bool generate_mipmap = true,
							 external_format external_format = external_format::rgba,
							 external_type external_type = external_type::unsigned_byte
			) requires (T == target::_2d_array) {
				this->data(data, 0, 0, 0, this->width, this->height, this->count, generate_mipmap, 0, external_format, external_type);
			}

			void set_depth_stencil_texture_mode(depth_stencil_texture_mode mode
												= depth_stencil_texture_mode::depth_component);
			void set_base_level(int level = 0);
			void set_border_color(float r = 0.f, float g = 0.f, float b = 0.f, float a = 0.f);
			//void set_compare_function(compare_function function = compare_function::greater);
			//void set_compare_mode(compare_mode mode = compare_mode::none);
			void set_lod_bias(float bias = 0.f);
			void set_min_filter(min_filter filter = min_filter::nearest_mipmap_linear);
			void set_mag_filter(mag_filter filter = mag_filter::linear);
			void set_min_lod(float value = -1000.f);
			void set_max_lod(float value = 1000.f);
			void set_max_level(int level = 1000);
			//void set_texture_swizzle_r(...);
			//void set_texture_swizzle_g(...);
			//void set_texture_swizzle_b(...);
			//void set_texture_swizzle_a(...);
			//void set_texture_swizzle_rgba(...);
			void set_texture_wrap_s(wrap wrap = wrap::repeat);
			void set_texture_wrap_t(wrap wrap = wrap::repeat);
			void set_texture_wrap_r(wrap wrap = wrap::repeat);

			template <target T = texture_type>
			static typename std::enable_if<needs_width<T>, size_t>::type maximum_width() {
				if constexpr (needs_depth<T>)
					return maximum_size_3d();
				else
					return maximum_size();
			}

			template <target T = texture_type>
			static typename std::enable_if<needs_height<T>, size_t>::type maximum_height() {
				if constexpr (needs_depth<T>)
					return maximum_size_3d();
				else
					return maximum_size();
			}

			template <target T = texture_type>
			static typename std::enable_if<needs_depth<T>, size_t>::type maximum_depth() {
				return maximum_size_3d();
			}

			template <target T = texture_type>
			static typename std::enable_if<needs_count<T>, size_t>::type maximum_count() {
				return maximum_layer_count();
			}

			friend inline std::ostream &operator<<(std::ostream &stream, interface const &object) {
				return stream << texture_type << " object (with id = " << object.id << ")";
			}

		protected:
			static size_t maximum_size();
			static size_t maximum_layer_count();
			static size_t maximum_size_3d();

		protected:
			internal_format internal_format;

		private:
			uint32_t id;
		};

	}

	class _1d : public detail::interface<detail::target::_1d> {
	public:
		using detail::interface<detail::target::_1d>::interface;
	};
	class _2d : public detail::interface<detail::target::_2d> {
	public:
		using detail::interface<detail::target::_2d>::interface;
	};
	class _3d : public detail::interface<detail::target::_3d> {
	public:
		using detail::interface<detail::target::_3d>::interface;
	};
	class _1d_array : public detail::interface<detail::target::_1d_array> {
	public:
		using detail::interface<detail::target::_1d_array>::interface;
	};
	class _2d_array : public detail::interface<detail::target::_2d_array> {
	public:
		using detail::interface<detail::target::_2d_array>::interface;
	};
	class rectangle : public detail::interface<detail::target::rectangle> {
	public:
		using detail::interface<detail::target::rectangle>::interface;
	};

	/* Unimplemented.
	class multisample : public detail::interface<detail::target::multisample> {
	public:
		using detail::interface<detail::target::multisample>::interface;
	};
	class multisample_array : public detail::interface<detail::target::multisample_array> {
	public:
		using detail::interface<detail::target::multisample_array>::interface;
	};
	class cube_map : public detail::interface<detail::target::cube_map> {
	public:
		using detail::interface<detail::target::cube_map>::interface;
	};
	class cube_map_array : public detail::interface<detail::target::cube_map_array> {
	public:
		using detail::interface<detail::target::cube_map_array>::interface;
	};
	class buffer : public detail::interface<detail::target::buffer> {
	public:
		using detail::interface<detail::target::buffer>::interface;
	};
	*/

	namespace detail {
		using generic_interface = std::variant<
			detail::interface<detail::target::_1d> const *,
			detail::interface<detail::target::_2d> const *,
			detail::interface<detail::target::_3d> const *,
			detail::interface<detail::target::_1d_array> const *,
			detail::interface<detail::target::_2d_array> const *,
			detail::interface<detail::target::cube_map> const *,
			detail::interface<detail::target::cube_map_array> const *,
			detail::interface<detail::target::rectangle> const *,
			detail::interface<detail::target::buffer> const *,
			detail::interface<detail::target::multisample> const *,
			detail::interface<detail::target::multisample_array> const *
		>;

		template <target texture_type> struct bind_texture_callable {
			interface<texture_type> const &texture_ref;
			size_t unit;
			typename essential::stack<generic_interface>::iterator operator()();
		};
		template <target texture_type> struct unbind_texture_callable {
			interface<texture_type> const &texture_ref;
			size_t unit;
			void operator()(typename essential::stack<generic_interface>::iterator);
		};

		template <target texture_type>
		class texture_guard
			: essential::simple_guard<bind_texture_callable<texture_type>, unbind_texture_callable<texture_type>> {
		public:
			texture_guard(interface<texture_type> const &texture_ref, size_t unit) :
				essential::simple_guard<bind_texture_callable<texture_type>, unbind_texture_callable<texture_type>>(
					detail::bind_texture_callable<texture_type>{ texture_ref, unit },
					detail::unbind_texture_callable<texture_type>{ texture_ref, unit }) {}
		};
	}
}

namespace clap::gl::detail::convert {
	GLenum to_gl(clap::gl::texture::detail::target v);
	clap::gl::texture::detail::target to_texture_target(GLenum v);

	GLenum to_gl(clap::gl::texture::internal_format v);
	clap::gl::texture::internal_format to_internal_format(GLenum v);

	GLenum to_gl(clap::gl::texture::external_format v);
	clap::gl::texture::external_format to_external_format(GLenum v);

	GLenum to_gl(clap::gl::texture::external_type v);
	clap::gl::texture::external_type to_texture_type(GLenum v);

	GLenum to_gl(clap::gl::texture::depth_stencil_texture_mode v);
	clap::gl::texture::depth_stencil_texture_mode to_depth_stencil_texture_mode(GLenum v);

	GLenum to_gl(clap::gl::texture::min_filter v);
	clap::gl::texture::min_filter to_min_filter(GLenum v);

	GLenum to_gl(clap::gl::texture::mag_filter v);
	clap::gl::texture::mag_filter to_mag_filter(GLenum v);

	GLenum to_gl(clap::gl::texture::wrap v);
	clap::gl::texture::wrap to_wrap(GLenum v);
}

#include <ostream>
std::ostream &operator<<(std::ostream &stream, clap::gl::texture::detail::target target);
#pragma once
namespace clap::gl::clear {
	void set_color(float r, float g, float b, float a);
	void set_depth(double d);
	void set_stencil(int s);

	namespace target_mask {
		inline const unsigned color_buffer = 0x1;
		inline const unsigned depth_buffer = 0x2;
		inline const unsigned stencil_buffer = 0x4;
	}

	void target(unsigned target_mask);
	void color();
	void depth();
	void stencil();
	void color_depth();
	void all();
}

namespace clap::gl::enable {
	void blend();
	void cull_face();
	void depth_clamp();
	void depth_test();
	void dither();
	void framebuffer_srgb();
	void line_smooth();
	void multisample();
	void polygon_smooth();
	void scissor_test();
	void stencil_test();
}
namespace clap::gl::disable {
	void blend();
	void cull_face();
	void depth_clamp();
	void depth_test();
	void dither();
	void framebuffer_srgb();
	void line_smooth();
	void multisample();
	void polygon_smooth();
	void scissor_test();
	void stencil_test();
}

namespace clap::gl {
	namespace detail {
		enum class blend_function {
			zero, one, source_color, one_minus_source_color,
			destination_color, one_minus_destination_color,
			source_alpha, one_minus_source_alpha,
			destination_alpha, one_minus_destination_alpha,
			constant_color, one_minus_constant_color,
			constant_alpha, one_minus_constant_alpha,
			source_alpha_saturate,
			source_1_color, one_minus_source_1_color,
			source_1_alpha, one_minus_source_1_alpha,
		};
	}

	enum class blend_function {
		zero = (int) detail::blend_function::zero,
		one = (int) detail::blend_function::one,
		source_color = (int) detail::blend_function::source_color,
		destination_color = (int) detail::blend_function::destination_color,
		source_alpha = (int) detail::blend_function::source_alpha,
		destination_alpha = (int) detail::blend_function::destination_alpha,
		constant_color = (int) detail::blend_function::constant_color,
		constant_alpha = (int) detail::blend_function::constant_alpha,
		source_alpha_saturate = (int) detail::blend_function::source_alpha_saturate,
		source_1_color = (int) detail::blend_function::source_1_color,
		source_1_alpha = (int) detail::blend_function::source_1_alpha,
	};

	inline detail::blend_function inverted(blend_function function) {
		switch (function) {
			case blend_function::zero: return detail::blend_function::one;
			case blend_function::one: return detail::blend_function::zero;
			case blend_function::source_color: return detail::blend_function::one_minus_source_color;
			case blend_function::destination_color: return detail::blend_function::one_minus_destination_color;
			case blend_function::source_alpha: return detail::blend_function::one_minus_source_alpha;
			case blend_function::destination_alpha: return detail::blend_function::one_minus_destination_alpha;
			case blend_function::constant_color: return detail::blend_function::one_minus_constant_color;
			case blend_function::constant_alpha: return detail::blend_function::one_minus_constant_alpha;
			case blend_function::source_alpha_saturate: return detail::blend_function::source_alpha_saturate;
			case blend_function::source_1_color: return detail::blend_function::one_minus_source_1_color;
			case blend_function::source_1_alpha: return detail::blend_function::one_minus_source_1_alpha;
			default: return detail::blend_function::zero;
		}
	}

	enum class face {
		front, back, both
	};

	enum class depth_function {
		never, less, equal, less_or_equal, greater, not_equal, greater_or_equal, always
	};
}

typedef unsigned int GLenum;
namespace clap::gl::detail::convert {
	GLenum to_gl(clap::gl::detail::blend_function v);
	clap::gl::detail::blend_function to_blend_function(GLenum v);
	inline GLenum to_gl(clap::gl::blend_function v) {
		return to_gl(clap::gl::detail::blend_function(v));
	}

	GLenum to_gl(clap::gl::face v);
	clap::gl::face to_face(GLenum v);

	GLenum to_gl(clap::gl::depth_function v);
	clap::gl::depth_function to_depth_function(GLenum v);
}
#include <ostream>
std::ostream &operator<<(std::ostream &s, clap::gl::detail::blend_function function);
std::ostream &operator<<(std::ostream &s, clap::gl::face face);
std::ostream &operator<<(std::ostream &s, clap::gl::depth_function face);

namespace clap::gl::enable {
	void blend(detail::blend_function source, detail::blend_function destination);
	inline void blend(blend_function source, detail::blend_function destination) {
		return blend(detail::blend_function(source), detail::blend_function(destination));
	}
	inline void blend(detail::blend_function source, blend_function destination) {
		return blend(detail::blend_function(source), detail::blend_function(destination));
	}
	inline void blend(blend_function source, blend_function destination) {
		return blend(detail::blend_function(source), detail::blend_function(destination));
	}
	inline void blend(blend_function function) {
		return blend(function, inverted(function));
	}

	void cull_face(face face);
	void depth_clamp(double near, double far);
	void depth_test(depth_function function);
	void scissor_test(signed x, signed y, unsigned width, unsigned height);
}

namespace clap::gl {
	void set_line_width(float width);

	void update_viewport(int offset_x, int offset_y, size_t width, size_t height);
	void update_viewport(size_t width, size_t height);
}

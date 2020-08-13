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
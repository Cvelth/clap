#pragma once
#include <string>

#include "gl/gl.hpp"

namespace clap::render {
	class font;

	class text {
	public:
		inline text(std::basic_string<char8_t> const &string, font &font, clap::gl::shader::program &program, size_t size = 16)
			: font_handle(font), height(size), program(program), count(0ull), uniforms(std::move(program.getUniforms())) {
			update(string);
		}
		inline text(std::string const &string, font &font, clap::gl::shader::program &program, size_t size = 16)
			: text((char8_t const *) string.c_str(), font, program, size) {}

		auto const &data() const { return string; }
		void update(std::basic_string<char32_t> const &string);
		void update(std::basic_string<char8_t> const &string);
		inline void update(std::string const &string) { update((char8_t const *) string.c_str()); }

		void render(size_t x = 0, size_t y = 0) const;
		void draw(size_t x = 0, size_t y = 0) const { render(x, y); }

		size_t size() const { return height; }
		void size(size_t value) { height = value; }

	private:
		font &font_handle;
		std::basic_string<char8_t> string;
		size_t height;

		clap::gl::shader::program &program;
		clap::gl::shader::variables uniforms;
		mutable clap::gl::vertex_array::single vertex_array;
		size_t count;
	};
}

namespace clap::render::settings {
	/**
	 * @brief Controls size of the font bitmap.
	 * 
	 * Real size is equal to `std::bit_ceil(font_bitmap_size_multiplier * text::size())` per dimention.
	 * 
	 * Default value is `32`.
	*/
	extern size_t font_bitmap_size_multiplier;
}
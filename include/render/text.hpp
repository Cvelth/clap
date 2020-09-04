#pragma once
#include <string>

#include "gl/gl.hpp"

namespace clap::render {
	class font;

	class text {
	public:
		text(std::basic_string<char8_t> const &string, font &font, clap::gl::shader::program &program, size_t size = 16);
		text(std::string const &string, font &font, clap::gl::shader::program &program, size_t size = 16);

		auto const &data() const { return string; }
		void update(std::basic_string<char32_t> const &string);
		void update(std::basic_string<char8_t> const &string);
		inline void update(std::string const &string) { update((char8_t const *) string.c_str()); }

		void move(size_t x, size_t y);

		void render() const;
		void draw() const { render(); }

		size_t size() const { return height; }
		void size(size_t value) { height = value; }

	private:
		font &font_handle;
		std::basic_string<char8_t> string;
		size_t height;

		clap::gl::shader::program &program;
		mutable clap::gl::vertex_array::single vertex_array;
		size_t count;
	};
}
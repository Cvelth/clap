#pragma once
#include <filesystem>
#include <unordered_map>
#include "gl/texture.hpp"

namespace clap::render {
	namespace detail {
		class font_file_handle;

		struct bitmap_coordinates {
			size_t top_left, top_right,
				bottom_left, bottom_right;
		};
	}

	class font {
	public:
		~font();

		static font load(std::filesystem::path const &filename);

	private:
		font(detail::font_file_handle *file_handle);

	private:
		detail::font_file_handle *file_handle;
		std::unordered_map<size_t, gl::texture::_2d> bitmaps;
		std::unordered_map<size_t, detail::bitmap_coordinates> coordinates;

		const size_t bitmap_width, bitmap_height;
	};
}
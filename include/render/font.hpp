#pragma once
#include <filesystem>
#include <unordered_map>
#include "gl/texture.hpp"

namespace clap::render {
	namespace detail {
		class font_file_handle;

		struct bitmap_coordinates {
			size_t from_x, from_y,
				to_x, to_y;
		};
		struct size_data {
			gl::texture::_2d bitmap;
			std::unordered_map<char32_t, detail::bitmap_coordinates> coordinates;
			size_t offset_x, offset_y;

			size_data(size_t bitmap_width, size_t bitmap_height);

			size_data(size_data const &) = delete;
			size_data(size_data &&other) noexcept 
				: bitmap(std::move(other.bitmap)), 
				coordinates(std::move(other.coordinates)),
				offset_x(other.offset_x), offset_y(other.offset_y){}
		};
	}
	class text;

	class font {
		friend text;
	public:
		~font();

		static font load(std::filesystem::path const &filename);

	private:
		font(detail::font_file_handle *file_handle, 
			 size_t bitmap_width = gl::texture::_2d::maximum_width(), 
			 size_t bitmap_height = gl::texture::_2d::maximum_height()) 
			: file_handle(file_handle), 
			bitmap_width(bitmap_width), bitmap_height(bitmap_height) {}

	private:
		detail::font_file_handle *file_handle;
		
		std::unordered_map<size_t, detail::size_data> data;

		const size_t bitmap_width, bitmap_height;
	};
}
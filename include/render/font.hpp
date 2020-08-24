#pragma once
#include <filesystem>
#include <unordered_map>
#include "gl/texture.hpp"

namespace clap::render {
	namespace detail {
		struct glyph_info {
			size_t msdf_index;
		};
	}
	class font {
	public:
		static font load(std::filesystem::path const &filename);

	private:
		font(gl::texture::_2d_array &&msdf) : data{}, msdf(msdf),
			msdf_width(msdf.maximum_width()), 
			msdf_height(msdf.maximum_height()),
			msdf_count(msdf.maximum_count()) {}

	private:
		gl::texture::_2d_array msdf;
		std::unordered_map<size_t, detail::glyph_info> data;

		const size_t msdf_width, msdf_height, msdf_count;
	};
}
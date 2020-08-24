#pragma once
#include <filesystem>
#include <unordered_map>
#include "gl/texture.hpp"

namespace clap::render {
	namespace detail {
		struct glyph_info {
			size_t msdf_index;
		};

		struct cooked_t {
		public:
			size_t width;
			size_t height;
			size_t count;
			size_t character;
			size_t color_count;
			std::vector<unsigned char> data_;

		public:
			inline auto* data() const { return data_.data(); }
			inline auto* data() { return data_.data(); }

			inline auto size() const { return data_.size(); }
			inline auto size() { return data_.size(); }

			inline auto number() const { return size() / character / character / color_count; }
			inline auto number() { return size() / character / character / color_count; }

			inline size_t expected_number() const { return height * width * count; }

			inline size_t expected_size() const { return expected_number() * character * character * color_count; }
			inline void update_size() { resize(expected_size()); }
			inline bool verify_size() const { 
				return size() == expected_size() && number() == expected_number();
			}

		private:
			inline void resize(size_t size) { data_.resize(size); }
		};
	}
	class font {
	public:
		static detail::cooked_t cook(std::filesystem::path const &filename);
		static font load(std::filesystem::path const &filename,
						 detail::cooked_t const &cooked);

	private:
		font(detail::cooked_t const &cooked, std::unordered_map<size_t, detail::glyph_info> &&data);

	private:
		gl::texture::_2d_array msdf;
		std::unordered_map<size_t, detail::glyph_info> data;

		const size_t msdf_width, msdf_height, msdf_count;
	};
}
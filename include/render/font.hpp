#pragma once
#include <string>

namespace clap::render {
	namespace detail {
		class font_face_t;
	}

	class font {
	public:
		font(detail::font_face_t&&) noexcept;
		~font();

		font(font const &) = delete;
		font(font &&) noexcept;

		static font load(std::string const &filename);

	private:
		detail::font_face_t *face;
	};
}
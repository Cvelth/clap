#pragma once
#include "ui/entity.hpp"

#include <memory>
#include <string>

namespace clap::ui {
	class zone;
	namespace detail {
		enum class resize_preference {
			fixed_minimum, prefer_minimum, free, prefer_maximum, fixed_maximum
		};
		struct size_constraits {
			friend zone;
		public:
			resize_preference width, height;
			size_t maximum_w, maximum_h;
			size_t minimum_w, minimum_h;

			size_t x() const { return current_x; }
			size_t y() const { return current_y; }
			size_t w() const { return current_w; }
			size_t h() const { return current_h; }
		protected:
			size_t current_x, current_y,
				current_w, current_h;

			size_constraits() noexcept;
			size_constraits(size_t width, size_t height) noexcept;
		};
	}
	namespace compound {
		class interface;
	}
	namespace detail {
		class state;
	}

	class zone : public entity {
		friend detail::state;
	public:
		explicit zone(compound::interface *owner) noexcept;
		zone(std::u8string name, size_t width, size_t height) noexcept;

	protected:
		std::u8string name;
		compound::interface *owner;

		detail::size_constraits size;
	};
}
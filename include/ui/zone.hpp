#pragma once
#include "essential/utility.hpp"

#include "ui/entity.hpp"

#include <functional>
#include <variant>

namespace clap::ui {
	class zone;

	namespace detail {
		enum class resize_attitude {
			any = 0,
			prefer_minimum = +1, always_minimum = +2,
			prefer_maximum = -1, always_maximum = -2
		};
		struct size_constraints {
			friend zone;
		protected:
			inline size_constraints(size_t x, size_t y, size_t width, size_t height) noexcept
				: width_attitude(resize_attitude::prefer_maximum)
				, height_attitude(resize_attitude::prefer_maximum)
				, maximum_width(std::numeric_limits<size_t>::max())
				, maximum_height(std::numeric_limits<size_t>::max())
				, minimum_width(std::numeric_limits<size_t>::min())
				, minimum_height(std::numeric_limits<size_t>::min())
				, current_x(x), current_y(y), current_w(width), current_h(height) {}
			inline size_constraints(size_t width, size_t height) noexcept
				: size_constraints(0u, 0u, width, height) {}
			inline size_constraints() noexcept
				: size_constraints(0u, 0u, 0u, 0u) {}
		public:
			resize_attitude width_attitude, height_attitude;
			size_t maximum_width, maximum_height;
			size_t minimum_width, minimum_height;

			[[nodiscard]] inline size_t x_position() const { return current_x; }
			[[nodiscard]] inline size_t y_position() const { return current_y; }
			[[nodiscard]] inline size_t width() const { return current_w; }
			[[nodiscard]] inline size_t height() const { return current_h; }

			[[nodiscard]] inline size_t x() const { return x_position(); }
			[[nodiscard]] inline size_t y() const { return y_position(); }
			[[nodiscard]] inline size_t w() const { return width(); }
			[[nodiscard]] inline size_t h() const { return height(); }

		protected:
			size_t current_x, current_y,
				current_w, current_h;
		};
	}

	namespace compound {
		class interface;
	}
	namespace detail {
		class manager;
	}

	class zone : public entity {
		friend detail::manager;
	public:
		zone(std::string_view name, size_t width, size_t height);
	protected:
		explicit zone(compound::interface &parent);

	public:
		[[nodiscard]] std::string name() const;
		[[nodiscard]] inline size_t width() const { return size.current_w; }
		[[nodiscard]] inline size_t height() const { return size.current_h; }

	public:
		std::function<void()> on_initialize;
		std::function<bool(utility::timestep)> on_update;

	protected:
		std::function<std::string_view()> get_identifier;

	protected:
		struct when_free {
			std::string name;
		};
		struct when_owned {
			compound::interface *owner;
		};

	protected:
		std::variant<when_free, when_owned> state;
		detail::size_constraints size;
	};
}
﻿#pragma once

#include "essential/utility.hpp"
#include "ui/entity.hpp"

#include <functional>
#include <memory>
#include <optional>
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
	namespace vulkan {
		struct window;
		struct window_view;
	}

	class zone : public entity {
		friend detail::manager;
	public:
		zone(std::string_view title, size_t width, size_t height);
	protected:
		explicit zone(std::shared_ptr<compound::interface> parent);

	public:
		[[nodiscard]] std::string name() const;
		[[nodiscard]] inline size_t width() const { return size.current_w; }
		[[nodiscard]] inline size_t height() const { return size.current_h; }
		[[nodiscard]] inline bool is_free() const {
			std::visit([]<typename T>(T &) {
				if constexpr (std::is_same<T, when_free>::value)
					return true;
				else
					return false;
			}, state);
		}

	protected:
		[[nodiscard]] std::optional<vulkan::window_view> window();
		[[nodiscard]] std::optional<vulkan::window_view const> window() const;

	private:
		void do_add();
		void do_remove();
		inline void do_initialize() { if (on_initialize) on_initialize(); }
		inline void do_update(utility::timestep const &ts) { if (on_update) on_update(ts); }
		void do_resize(vkfw::Window const &window, size_t new_width, size_t new_height);

	public:
		std::function<void()> on_initialize;
		std::function<void(utility::timestep const &)> on_update;
		std::function<bool(size_t, size_t)> on_resize;

	protected:
		std::function<std::string_view()> get_identifier;

	protected:
		struct when_free {
			std::string title;
			std::shared_ptr<vulkan::window> window;
		};
		struct when_owned {
			std::shared_ptr<compound::interface> owner;
			zone *root;
		};

	protected:
		std::variant<when_free, when_owned> state;
		detail::size_constraints size;
	};
}
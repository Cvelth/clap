#pragma once
namespace clap::ui::detail::event {
	enum class key {
		unknown, space, apostrophe, comma, minus, period, slash,
		n_1, n_2, n_3, n_4, n_5, n_6, n_7, n_8, n_9, n_0,
		semicolon, equal,
		a, b, c, d, e, f, g, h, i, j, k, l, m,
		n, o, p, q, r, s, t, u, v, w, x, y, z,
		bracket_left, backslash, bracket_right, grave_accent,
		world_1, world_2, escape, enter, tab, backspace,
		insert, delete_, right, left, down, up, page_up, page_down,
		home, end, caps_lock, scroll_lock, num_lock, print_screen,
		pause, f1, f2, f3, f4, f5, f6, f7, f8, f9, f10, f11, f12,
		f13, f14, f15, f16, f17, f18, f19, f20, f21, f22, f23, f24, f25,
		kp_1, kp_2, kp_3, kp_4, kp_5, kp_6, kp_7, kp_8, kp_9, kp_0,
		kp_point, kp_divide, kp_multiply,
		kp_subtract, kp_add, kp_enter, kp_equal,
		shift_left, control_left, alt_left, super_left,
		shift_right, control_right, alt_right, super_right,
		menu, last = menu,
	};
	enum class key_action {
		press, release, repeat
	};

	enum class mouse_button {
		b_1, b_2, b_3, b_4,
		b_5, b_6, b_7, b_8,

		last = b_8,
		left = b_1,
		right = b_2,
		middle = b_3
	};
	enum class mouse_button_action {
		press, release
	};

	enum class modificator_mask {
		none = 0x0, shift = 0x1,
		ctrl = 0x2, alt = 0x4,
		super = 0x8
	};
}
clap::ui::detail::event::modificator_mask operator|(clap::ui::detail::event::modificator_mask lhs,
													clap::ui::detail::event::modificator_mask rhs);
clap::ui::detail::event::modificator_mask operator|=(clap::ui::detail::event::modificator_mask &lhs,
													 clap::ui::detail::event::modificator_mask rhs);
clap::ui::detail::event::modificator_mask operator&(clap::ui::detail::event::modificator_mask lhs,
													clap::ui::detail::event::modificator_mask rhs);
clap::ui::detail::event::modificator_mask operator&=(clap::ui::detail::event::modificator_mask &lhs,
													 clap::ui::detail::event::modificator_mask rhs);
clap::ui::detail::event::modificator_mask operator^(clap::ui::detail::event::modificator_mask lhs,
													clap::ui::detail::event::modificator_mask rhs);
clap::ui::detail::event::modificator_mask operator^=(clap::ui::detail::event::modificator_mask &lhs,
													 clap::ui::detail::event::modificator_mask rhs);

#pragma warning(push)
#pragma warning(disable : 4100)

namespace clap::ui::detail::event {
	namespace detail {
		class impl;
	}

	class handler_interface {
		friend clap::ui::detail::event::detail::impl;
	protected:
		inline virtual bool on_key(event::key key, int scancode, event::key_action action,
								   event::modificator_mask modificators) {
			return false;
		}
		inline virtual bool on_character(char32_t codepoint) {
			return false;
		}
		inline virtual bool on_character(char32_t codepoint, event::modificator_mask modificators) {
			return false;
		}
		inline virtual bool on_mouse_button(event::mouse_button button, event::mouse_button_action action,
											event::modificator_mask modificators) {
			return false;
		}
		inline virtual bool on_mouse_move(double x, double y) {
			return false;
		}
		inline virtual bool on_mouse_entering() {
			return false;
		}
		inline virtual bool on_mouse_leaving() {
			return false;
		}
		inline virtual bool on_scroll(double dx, double dy) {
			return false;
		}
		inline virtual bool on_window_resize(size_t x, size_t y) {
			return false;
		}
		inline virtual bool on_file_drop(int count, const char8_t **paths) {
			return false;
		}

		inline virtual bool on_window_close() {
			return false;
		}
		inline virtual bool on_window_move(size_t x, size_t y) {
			return false;
		}
		inline virtual bool on_window_focus_gain() {
			return false;
		}
		inline virtual bool on_window_focus_loss() {
			return false;
		}
		inline virtual bool on_window_minimize() {
			return false;
		}
		inline virtual bool on_restore_minimized() {
			return false;
		}
		inline virtual bool on_window_maximize() {
			return false;
		}
		inline virtual bool on_restore_maximized() {
			return false;
		}
	};
}

#pragma warning(pop)

namespace clap::ui::detail::event::convert {
	int to_glfw(key v);
	key to_key(int v);
	int to_glfw(key_action v);
	key_action to_key_action(int v);

	int to_glfw(mouse_button v);
	mouse_button to_mouse_button(int v);
	int to_glfw(mouse_button_action v);
	mouse_button_action to_mouse_button_action(int v);

	int to_glfw(modificator_mask v);
	modificator_mask to_modificator_mask(int v);
}

#include <ostream>
std::ostream &operator<<(std::ostream &s, clap::ui::detail::event::key v);
std::ostream &operator<<(std::ostream &s, clap::ui::detail::event::key_action v);
std::ostream &operator<<(std::ostream &s, clap::ui::detail::event::mouse_button v);
std::ostream &operator<<(std::ostream &s, clap::ui::detail::event::mouse_button_action v);
std::ostream &operator<<(std::ostream &s, clap::ui::detail::event::modificator_mask v);
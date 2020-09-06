#pragma once
namespace clap::event {
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
	enum class mouse_hover_action {
		enter, leave
	};

	enum class modificator_mask {
		none = 0x0, shift = 0x1,
		ctrl = 0x2, alt = 0x4,
		super = 0x8
	};
}
clap::event::modificator_mask operator|(clap::event::modificator_mask lhs, clap::event::modificator_mask rhs);
clap::event::modificator_mask operator|=(clap::event::modificator_mask &lhs, clap::event::modificator_mask rhs);
clap::event::modificator_mask operator&(clap::event::modificator_mask lhs, clap::event::modificator_mask rhs);
clap::event::modificator_mask operator&=(clap::event::modificator_mask &lhs, clap::event::modificator_mask rhs);
clap::event::modificator_mask operator^(clap::event::modificator_mask lhs, clap::event::modificator_mask rhs);
clap::event::modificator_mask operator^=(clap::event::modificator_mask &lhs, clap::event::modificator_mask rhs);
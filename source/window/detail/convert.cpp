#include "window/detail/glfw.hpp"

#include "glfw/glfw3.h"

#include "essential/log.hpp"

using underlying = std::underlying_type_t<clap::event::modificator_mask>;
clap::event::modificator_mask operator|(clap::event::modificator_mask lhs, clap::event::modificator_mask rhs) {
	return static_cast<clap::event::modificator_mask>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}
clap::event::modificator_mask operator|=(clap::event::modificator_mask &lhs, clap::event::modificator_mask rhs) {
	return lhs = lhs | rhs;
}
clap::event::modificator_mask operator&(clap::event::modificator_mask lhs, clap::event::modificator_mask rhs) {
	return static_cast<clap::event::modificator_mask>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}
clap::event::modificator_mask operator&=(clap::event::modificator_mask &lhs, clap::event::modificator_mask rhs) {
	return lhs = lhs & rhs;
}
clap::event::modificator_mask operator^(clap::event::modificator_mask lhs, clap::event::modificator_mask rhs) {
	return static_cast<clap::event::modificator_mask>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}
clap::event::modificator_mask operator^=(clap::event::modificator_mask &lhs, clap::event::modificator_mask rhs) {
	return lhs = lhs ^ rhs;
}


int clap::event::detail::convert::to_glfw(clap::event::key v) {
	switch (v) {
		case clap::event::key::unknown: return GLFW_KEY_UNKNOWN;
		case clap::event::key::space: return GLFW_KEY_SPACE;
		case clap::event::key::apostrophe: return GLFW_KEY_APOSTROPHE;
		case clap::event::key::comma: return GLFW_KEY_COMMA;
		case clap::event::key::minus: return GLFW_KEY_MINUS;
		case clap::event::key::period: return GLFW_KEY_PERIOD;
		case clap::event::key::slash: return GLFW_KEY_SLASH;
		case clap::event::key::n_0: return GLFW_KEY_0;
		case clap::event::key::n_1: return GLFW_KEY_1;
		case clap::event::key::n_2: return GLFW_KEY_2;
		case clap::event::key::n_3: return GLFW_KEY_3;
		case clap::event::key::n_4: return GLFW_KEY_4;
		case clap::event::key::n_5: return GLFW_KEY_5;
		case clap::event::key::n_6: return GLFW_KEY_6;
		case clap::event::key::n_7: return GLFW_KEY_7;
		case clap::event::key::n_8: return GLFW_KEY_8;
		case clap::event::key::n_9: return GLFW_KEY_9;
		case clap::event::key::semicolon: return GLFW_KEY_SEMICOLON;
		case clap::event::key::equal: return GLFW_KEY_EQUAL;
		case clap::event::key::a: return GLFW_KEY_A;
		case clap::event::key::b: return GLFW_KEY_B;
		case clap::event::key::c: return GLFW_KEY_C;
		case clap::event::key::d: return GLFW_KEY_D;
		case clap::event::key::e: return GLFW_KEY_E;
		case clap::event::key::f: return GLFW_KEY_F;
		case clap::event::key::g: return GLFW_KEY_G;
		case clap::event::key::h: return GLFW_KEY_H;
		case clap::event::key::i: return GLFW_KEY_I;
		case clap::event::key::j: return GLFW_KEY_J;
		case clap::event::key::k: return GLFW_KEY_K;
		case clap::event::key::l: return GLFW_KEY_L;
		case clap::event::key::m: return GLFW_KEY_M;
		case clap::event::key::n: return GLFW_KEY_N;
		case clap::event::key::o: return GLFW_KEY_O;
		case clap::event::key::p: return GLFW_KEY_P;
		case clap::event::key::q: return GLFW_KEY_Q;
		case clap::event::key::r: return GLFW_KEY_R;
		case clap::event::key::s: return GLFW_KEY_S;
		case clap::event::key::t: return GLFW_KEY_T;
		case clap::event::key::u: return GLFW_KEY_U;
		case clap::event::key::v: return GLFW_KEY_V;
		case clap::event::key::w: return GLFW_KEY_W;
		case clap::event::key::x: return GLFW_KEY_X;
		case clap::event::key::y: return GLFW_KEY_Y;
		case clap::event::key::z: return GLFW_KEY_Z;
		case clap::event::key::bracket_left: return GLFW_KEY_LEFT_BRACKET;
		case clap::event::key::backslash: return GLFW_KEY_BACKSLASH;
		case clap::event::key::bracket_right: return GLFW_KEY_RIGHT_BRACKET;
		case clap::event::key::grave_accent: return GLFW_KEY_GRAVE_ACCENT;
		case clap::event::key::world_1: return GLFW_KEY_WORLD_1;
		case clap::event::key::world_2: return GLFW_KEY_WORLD_2;
		case clap::event::key::escape: return GLFW_KEY_ESCAPE;
		case clap::event::key::enter: return GLFW_KEY_ENTER;
		case clap::event::key::tab: return GLFW_KEY_TAB;
		case clap::event::key::backspace: return GLFW_KEY_BACKSPACE;
		case clap::event::key::insert: return GLFW_KEY_INSERT;
		case clap::event::key::delete_: return GLFW_KEY_DELETE;
		case clap::event::key::right: return GLFW_KEY_RIGHT;
		case clap::event::key::left: return GLFW_KEY_LEFT;
		case clap::event::key::down: return GLFW_KEY_DOWN;
		case clap::event::key::up: return GLFW_KEY_UP;
		case clap::event::key::page_up: return GLFW_KEY_PAGE_UP;
		case clap::event::key::page_down: return GLFW_KEY_PAGE_DOWN;
		case clap::event::key::home: return GLFW_KEY_HOME;
		case clap::event::key::end: return GLFW_KEY_END;
		case clap::event::key::caps_lock: return GLFW_KEY_CAPS_LOCK;
		case clap::event::key::scroll_lock: return GLFW_KEY_SCROLL_LOCK;
		case clap::event::key::num_lock: return GLFW_KEY_NUM_LOCK;
		case clap::event::key::print_screen: return GLFW_KEY_PRINT_SCREEN;
		case clap::event::key::pause: return GLFW_KEY_PAUSE;
		case clap::event::key::f1: return GLFW_KEY_F1;
		case clap::event::key::f2: return GLFW_KEY_F2;
		case clap::event::key::f3: return GLFW_KEY_F3;
		case clap::event::key::f4: return GLFW_KEY_F4;
		case clap::event::key::f5: return GLFW_KEY_F5;
		case clap::event::key::f6: return GLFW_KEY_F6;
		case clap::event::key::f7: return GLFW_KEY_F7;
		case clap::event::key::f8: return GLFW_KEY_F8;
		case clap::event::key::f9: return GLFW_KEY_F9;
		case clap::event::key::f10: return GLFW_KEY_F10;
		case clap::event::key::f11: return GLFW_KEY_F11;
		case clap::event::key::f12: return GLFW_KEY_F12;
		case clap::event::key::f13: return GLFW_KEY_F13;
		case clap::event::key::f14: return GLFW_KEY_F14;
		case clap::event::key::f15: return GLFW_KEY_F15;
		case clap::event::key::f16: return GLFW_KEY_F16;
		case clap::event::key::f17: return GLFW_KEY_F17;
		case clap::event::key::f18: return GLFW_KEY_F18;
		case clap::event::key::f19: return GLFW_KEY_F19;
		case clap::event::key::f20: return GLFW_KEY_F20;
		case clap::event::key::f21: return GLFW_KEY_F21;
		case clap::event::key::f22: return GLFW_KEY_F22;
		case clap::event::key::f23: return GLFW_KEY_F23;
		case clap::event::key::f24: return GLFW_KEY_F24;
		case clap::event::key::f25: return GLFW_KEY_F25;
		case clap::event::key::kp_0: return GLFW_KEY_KP_0;
		case clap::event::key::kp_1: return GLFW_KEY_KP_1;
		case clap::event::key::kp_2: return GLFW_KEY_KP_2;
		case clap::event::key::kp_3: return GLFW_KEY_KP_3;
		case clap::event::key::kp_4: return GLFW_KEY_KP_4;
		case clap::event::key::kp_5: return GLFW_KEY_KP_5;
		case clap::event::key::kp_6: return GLFW_KEY_KP_6;
		case clap::event::key::kp_7: return GLFW_KEY_KP_7;
		case clap::event::key::kp_8: return GLFW_KEY_KP_8;
		case clap::event::key::kp_9: return GLFW_KEY_KP_9;
		case clap::event::key::kp_point: return GLFW_KEY_KP_DECIMAL;
		case clap::event::key::kp_divide: return GLFW_KEY_KP_DIVIDE;
		case clap::event::key::kp_multiply: return GLFW_KEY_KP_MULTIPLY;
		case clap::event::key::kp_subtract: return GLFW_KEY_KP_SUBTRACT;
		case clap::event::key::kp_add: return GLFW_KEY_KP_ADD;
		case clap::event::key::kp_enter: return GLFW_KEY_KP_ENTER;
		case clap::event::key::kp_equal: return GLFW_KEY_KP_EQUAL;
		case clap::event::key::shift_left: return GLFW_KEY_LEFT_SHIFT;
		case clap::event::key::control_left: return GLFW_KEY_LEFT_CONTROL;
		case clap::event::key::alt_left: return GLFW_KEY_LEFT_ALT;
		case clap::event::key::super_left: return GLFW_KEY_LEFT_SUPER;
		case clap::event::key::shift_right: return GLFW_KEY_RIGHT_SHIFT;
		case clap::event::key::control_right: return GLFW_KEY_RIGHT_CONTROL;
		case clap::event::key::alt_right: return GLFW_KEY_RIGHT_ALT;
		case clap::event::key::super_right: return GLFW_KEY_RIGHT_SUPER;
		case clap::event::key::menu: return GLFW_KEY_MENU;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::event::key clap::event::detail::convert::to_key(int v) {
	switch (v) {
		case GLFW_KEY_UNKNOWN: return clap::event::key::unknown;
		case GLFW_KEY_SPACE: return clap::event::key::space;
		case GLFW_KEY_APOSTROPHE: return clap::event::key::apostrophe;
		case GLFW_KEY_COMMA: return clap::event::key::comma;
		case GLFW_KEY_MINUS: return clap::event::key::minus;
		case GLFW_KEY_PERIOD: return clap::event::key::period;
		case GLFW_KEY_SLASH: return clap::event::key::slash;
		case GLFW_KEY_0: return clap::event::key::n_0;
		case GLFW_KEY_1: return clap::event::key::n_1;
		case GLFW_KEY_2: return clap::event::key::n_2;
		case GLFW_KEY_3: return clap::event::key::n_3;
		case GLFW_KEY_4: return clap::event::key::n_4;
		case GLFW_KEY_5: return clap::event::key::n_5;
		case GLFW_KEY_6: return clap::event::key::n_6;
		case GLFW_KEY_7: return clap::event::key::n_7;
		case GLFW_KEY_8: return clap::event::key::n_8;
		case GLFW_KEY_9: return clap::event::key::n_9;
		case GLFW_KEY_SEMICOLON: return clap::event::key::semicolon;
		case GLFW_KEY_EQUAL: return clap::event::key::equal;
		case GLFW_KEY_A: return clap::event::key::a;
		case GLFW_KEY_B: return clap::event::key::b;
		case GLFW_KEY_C: return clap::event::key::c;
		case GLFW_KEY_D: return clap::event::key::d;
		case GLFW_KEY_E: return clap::event::key::e;
		case GLFW_KEY_F: return clap::event::key::f;
		case GLFW_KEY_G: return clap::event::key::g;
		case GLFW_KEY_H: return clap::event::key::h;
		case GLFW_KEY_I: return clap::event::key::i;
		case GLFW_KEY_J: return clap::event::key::j;
		case GLFW_KEY_K: return clap::event::key::k;
		case GLFW_KEY_L: return clap::event::key::l;
		case GLFW_KEY_M: return clap::event::key::m;
		case GLFW_KEY_N: return clap::event::key::n;
		case GLFW_KEY_O: return clap::event::key::o;
		case GLFW_KEY_P: return clap::event::key::p;
		case GLFW_KEY_Q: return clap::event::key::q;
		case GLFW_KEY_R: return clap::event::key::r;
		case GLFW_KEY_S: return clap::event::key::s;
		case GLFW_KEY_T: return clap::event::key::t;
		case GLFW_KEY_U: return clap::event::key::u;
		case GLFW_KEY_V: return clap::event::key::v;
		case GLFW_KEY_W: return clap::event::key::w;
		case GLFW_KEY_X: return clap::event::key::x;
		case GLFW_KEY_Y: return clap::event::key::y;
		case GLFW_KEY_Z: return clap::event::key::z;
		case GLFW_KEY_LEFT_BRACKET: return clap::event::key::bracket_left;
		case GLFW_KEY_BACKSLASH: return clap::event::key::backslash;
		case GLFW_KEY_RIGHT_BRACKET: return clap::event::key::bracket_right;
		case GLFW_KEY_GRAVE_ACCENT: return clap::event::key::grave_accent;
		case GLFW_KEY_WORLD_1: return clap::event::key::world_1;
		case GLFW_KEY_WORLD_2: return clap::event::key::world_2;
		case GLFW_KEY_ESCAPE: return clap::event::key::escape;
		case GLFW_KEY_ENTER: return clap::event::key::enter;
		case GLFW_KEY_TAB: return clap::event::key::tab;
		case GLFW_KEY_BACKSPACE: return clap::event::key::backspace;
		case GLFW_KEY_INSERT: return clap::event::key::insert;
		case GLFW_KEY_DELETE: return clap::event::key::delete_;
		case GLFW_KEY_RIGHT: return clap::event::key::right;
		case GLFW_KEY_LEFT: return clap::event::key::left;
		case GLFW_KEY_DOWN: return clap::event::key::down;
		case GLFW_KEY_UP: return clap::event::key::up;
		case GLFW_KEY_PAGE_UP: return clap::event::key::page_up;
		case GLFW_KEY_PAGE_DOWN: return clap::event::key::page_down;
		case GLFW_KEY_HOME: return clap::event::key::home;
		case GLFW_KEY_END: return clap::event::key::end;
		case GLFW_KEY_CAPS_LOCK: return clap::event::key::caps_lock;
		case GLFW_KEY_SCROLL_LOCK: return clap::event::key::scroll_lock;
		case GLFW_KEY_NUM_LOCK: return clap::event::key::num_lock;
		case GLFW_KEY_PRINT_SCREEN: return clap::event::key::print_screen;
		case GLFW_KEY_PAUSE: return clap::event::key::pause;
		case GLFW_KEY_F1: return clap::event::key::f1;
		case GLFW_KEY_F2: return clap::event::key::f2;
		case GLFW_KEY_F3: return clap::event::key::f3;
		case GLFW_KEY_F4: return clap::event::key::f4;
		case GLFW_KEY_F5: return clap::event::key::f5;
		case GLFW_KEY_F6: return clap::event::key::f6;
		case GLFW_KEY_F7: return clap::event::key::f7;
		case GLFW_KEY_F8: return clap::event::key::f8;
		case GLFW_KEY_F9: return clap::event::key::f9;
		case GLFW_KEY_F10: return clap::event::key::f10;
		case GLFW_KEY_F11: return clap::event::key::f11;
		case GLFW_KEY_F12: return clap::event::key::f12;
		case GLFW_KEY_F13: return clap::event::key::f13;
		case GLFW_KEY_F14: return clap::event::key::f14;
		case GLFW_KEY_F15: return clap::event::key::f15;
		case GLFW_KEY_F16: return clap::event::key::f16;
		case GLFW_KEY_F17: return clap::event::key::f17;
		case GLFW_KEY_F18: return clap::event::key::f18;
		case GLFW_KEY_F19: return clap::event::key::f19;
		case GLFW_KEY_F20: return clap::event::key::f20;
		case GLFW_KEY_F21: return clap::event::key::f21;
		case GLFW_KEY_F22: return clap::event::key::f22;
		case GLFW_KEY_F23: return clap::event::key::f23;
		case GLFW_KEY_F24: return clap::event::key::f24;
		case GLFW_KEY_F25: return clap::event::key::f25;
		case GLFW_KEY_KP_0: return clap::event::key::kp_0;
		case GLFW_KEY_KP_1: return clap::event::key::kp_1;
		case GLFW_KEY_KP_2: return clap::event::key::kp_2;
		case GLFW_KEY_KP_3: return clap::event::key::kp_3;
		case GLFW_KEY_KP_4: return clap::event::key::kp_4;
		case GLFW_KEY_KP_5: return clap::event::key::kp_5;
		case GLFW_KEY_KP_6: return clap::event::key::kp_6;
		case GLFW_KEY_KP_7: return clap::event::key::kp_7;
		case GLFW_KEY_KP_8: return clap::event::key::kp_8;
		case GLFW_KEY_KP_9: return clap::event::key::kp_9;
		case GLFW_KEY_KP_DECIMAL: return clap::event::key::kp_point;
		case GLFW_KEY_KP_DIVIDE: return clap::event::key::kp_divide;
		case GLFW_KEY_KP_MULTIPLY: return clap::event::key::kp_multiply;
		case GLFW_KEY_KP_SUBTRACT: return clap::event::key::kp_subtract;
		case GLFW_KEY_KP_ADD: return clap::event::key::kp_add;
		case GLFW_KEY_KP_ENTER: return clap::event::key::kp_enter;
		case GLFW_KEY_KP_EQUAL: return clap::event::key::kp_equal;
		case GLFW_KEY_LEFT_SHIFT: return clap::event::key::shift_left;
		case GLFW_KEY_LEFT_CONTROL: return clap::event::key::control_left;
		case GLFW_KEY_LEFT_ALT: return clap::event::key::alt_left;
		case GLFW_KEY_LEFT_SUPER: return clap::event::key::super_left;
		case GLFW_KEY_RIGHT_SHIFT: return clap::event::key::shift_right;
		case GLFW_KEY_RIGHT_CONTROL: return clap::event::key::control_right;
		case GLFW_KEY_RIGHT_ALT: return clap::event::key::alt_right;
		case GLFW_KEY_RIGHT_SUPER: return clap::event::key::super_right;
		case GLFW_KEY_MENU: return clap::event::key::menu;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

int clap::event::detail::convert::to_glfw(clap::event::key_action v) {
	switch (v) {
		case clap::event::key_action::press: return GLFW_PRESS;
		case clap::event::key_action::release: return GLFW_RELEASE;
		case clap::event::key_action::repeat: return GLFW_REPEAT;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::event::key_action clap::event::detail::convert::to_key_action(int v) {
	switch (v) {
		case GLFW_PRESS: return clap::event::key_action::press;
		case GLFW_RELEASE: return clap::event::key_action::release;
		case GLFW_REPEAT: return clap::event::key_action::repeat;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

int clap::event::detail::convert::to_glfw(mouse_button v) {
	switch (v) {
		case clap::event::mouse_button::b_1: return GLFW_MOUSE_BUTTON_1;
		case clap::event::mouse_button::b_2: return GLFW_MOUSE_BUTTON_2;
		case clap::event::mouse_button::b_3: return GLFW_MOUSE_BUTTON_3;
		case clap::event::mouse_button::b_4: return GLFW_MOUSE_BUTTON_4;
		case clap::event::mouse_button::b_5: return GLFW_MOUSE_BUTTON_5;
		case clap::event::mouse_button::b_6: return GLFW_MOUSE_BUTTON_6;
		case clap::event::mouse_button::b_7: return GLFW_MOUSE_BUTTON_7;
		case clap::event::mouse_button::b_8: return GLFW_MOUSE_BUTTON_8;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::event::mouse_button clap::event::detail::convert::to_mouse_button(int v) {
	switch (v) {
		case GLFW_MOUSE_BUTTON_1: return clap::event::mouse_button::b_1;
		case GLFW_MOUSE_BUTTON_2: return clap::event::mouse_button::b_2;
		case GLFW_MOUSE_BUTTON_3: return clap::event::mouse_button::b_3;
		case GLFW_MOUSE_BUTTON_4: return clap::event::mouse_button::b_4;
		case GLFW_MOUSE_BUTTON_5: return clap::event::mouse_button::b_5;
		case GLFW_MOUSE_BUTTON_6: return clap::event::mouse_button::b_6;
		case GLFW_MOUSE_BUTTON_7: return clap::event::mouse_button::b_7;
		case GLFW_MOUSE_BUTTON_8: return clap::event::mouse_button::b_8;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

int clap::event::detail::convert::to_glfw(mouse_button_action v) {
	switch (v) {
		case clap::event::mouse_button_action::press: return GLFW_PRESS;
		case clap::event::mouse_button_action::release: return GLFW_RELEASE;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::event::mouse_button_action clap::event::detail::convert::to_mouse_button_action(int v) {
	switch (v) {
		case GLFW_PRESS: return clap::event::mouse_button_action::press;
		case GLFW_RELEASE: return clap::event::mouse_button_action::release;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

int clap::event::detail::convert::to_glfw(clap::event::mouse_hover_action v) {
	switch (v) {
		case clap::event::mouse_hover_action::enter: return GLFW_TRUE;
		case clap::event::mouse_hover_action::leave: return GLFW_FALSE;
	}
	clap::log::error::critical << "Unsupported enum value.";

}
clap::event::mouse_hover_action clap::event::detail::convert::to_mouse_hover_action(int v) {
	switch (v) {
		case GLFW_TRUE: return clap::event::mouse_hover_action::enter;
		case GLFW_FALSE: return clap::event::mouse_hover_action::leave;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

int clap::event::detail::convert::to_glfw(clap::event::modificator_mask v) {
	return static_cast<int>(static_cast<underlying>(v));
}
clap::event::modificator_mask clap::event::detail::convert::to_modificator_mask(int v) {
	return static_cast<clap::event::modificator_mask>(static_cast<underlying>(v));
}


std::ostream &operator<<(std::ostream &s, clap::event::key v) {
	switch (v) {
		case clap::event::key::unknown: return s << "unknown";
		case clap::event::key::space: return s << "space";
		case clap::event::key::apostrophe: return s << "apostrophe";
		case clap::event::key::comma: return s << "comma";
		case clap::event::key::minus: return s << "minus";
		case clap::event::key::period: return s << "period";
		case clap::event::key::slash: return s << "slash";
		case clap::event::key::n_0: return s << "0";
		case clap::event::key::n_1: return s << "1";
		case clap::event::key::n_2: return s << "2";
		case clap::event::key::n_3: return s << "3";
		case clap::event::key::n_4: return s << "4";
		case clap::event::key::n_5: return s << "5";
		case clap::event::key::n_6: return s << "6";
		case clap::event::key::n_7: return s << "7";
		case clap::event::key::n_8: return s << "8";
		case clap::event::key::n_9: return s << "9";
		case clap::event::key::semicolon: return s << "semicolon";
		case clap::event::key::equal: return s << "equal";
		case clap::event::key::a: return s << "a";
		case clap::event::key::b: return s << "b";
		case clap::event::key::c: return s << "c";
		case clap::event::key::d: return s << "d";
		case clap::event::key::e: return s << "e";
		case clap::event::key::f: return s << "f";
		case clap::event::key::g: return s << "g";
		case clap::event::key::h: return s << "h";
		case clap::event::key::i: return s << "i";
		case clap::event::key::j: return s << "j";
		case clap::event::key::k: return s << "k";
		case clap::event::key::l: return s << "l";
		case clap::event::key::m: return s << "m";
		case clap::event::key::n: return s << "n";
		case clap::event::key::o: return s << "o";
		case clap::event::key::p: return s << "p";
		case clap::event::key::q: return s << "q";
		case clap::event::key::r: return s << "r";
		case clap::event::key::s: return s << "s";
		case clap::event::key::t: return s << "t";
		case clap::event::key::u: return s << "u";
		case clap::event::key::v: return s << "v";
		case clap::event::key::w: return s << "w";
		case clap::event::key::x: return s << "x";
		case clap::event::key::y: return s << "y";
		case clap::event::key::z: return s << "z";
		case clap::event::key::bracket_left: return s << "left bracket";
		case clap::event::key::backslash: return s << "backslash";
		case clap::event::key::bracket_right: return s << "rigth bracket";
		case clap::event::key::grave_accent: return s << "grave accent";
		case clap::event::key::world_1: return s << "world 1";
		case clap::event::key::world_2: return s << "world 2";
		case clap::event::key::escape: return s << "escape";
		case clap::event::key::enter: return s << "enter";
		case clap::event::key::tab: return s << "tab";
		case clap::event::key::backspace: return s << "backspace";
		case clap::event::key::insert: return s << "insert";
		case clap::event::key::delete_: return s << "delete";
		case clap::event::key::right: return s << "right arrow";
		case clap::event::key::left: return s << "left arrow";
		case clap::event::key::down: return s << "down arrow";
		case clap::event::key::up: return s << "up arrow";
		case clap::event::key::page_up: return s << "page up";
		case clap::event::key::page_down: return s << "page down";
		case clap::event::key::home: return s << "home";
		case clap::event::key::end: return s << "end";
		case clap::event::key::caps_lock: return s << "caps lock";
		case clap::event::key::scroll_lock: return s << "scroll lock";
		case clap::event::key::num_lock: return s << "num lock";
		case clap::event::key::print_screen: return s << "print screen";
		case clap::event::key::pause: return s << "pause";
		case clap::event::key::f1: return s << "f1";
		case clap::event::key::f2: return s << "f2";
		case clap::event::key::f3: return s << "f3";
		case clap::event::key::f4: return s << "f4";
		case clap::event::key::f5: return s << "f5";
		case clap::event::key::f6: return s << "f6";
		case clap::event::key::f7: return s << "f7";
		case clap::event::key::f8: return s << "f8";
		case clap::event::key::f9: return s << "f9";
		case clap::event::key::f10: return s << "f10";
		case clap::event::key::f11: return s << "f11";
		case clap::event::key::f12: return s << "f12";
		case clap::event::key::f13: return s << "f13";
		case clap::event::key::f14: return s << "f14";
		case clap::event::key::f15: return s << "f15";
		case clap::event::key::f16: return s << "f16";
		case clap::event::key::f17: return s << "f17";
		case clap::event::key::f18: return s << "f18";
		case clap::event::key::f19: return s << "f19";
		case clap::event::key::f20: return s << "f20";
		case clap::event::key::f21: return s << "f21";
		case clap::event::key::f22: return s << "f22";
		case clap::event::key::f23: return s << "f23";
		case clap::event::key::f24: return s << "f24";
		case clap::event::key::f25: return s << "f25";
		case clap::event::key::kp_0: return s << "keypad 0";
		case clap::event::key::kp_1: return s << "keypad 1";
		case clap::event::key::kp_2: return s << "keypad 2";
		case clap::event::key::kp_3: return s << "keypad 3";
		case clap::event::key::kp_4: return s << "keypad 4";
		case clap::event::key::kp_5: return s << "keypad 5";
		case clap::event::key::kp_6: return s << "keypad 6";
		case clap::event::key::kp_7: return s << "keypad 7";
		case clap::event::key::kp_8: return s << "keypad 8";
		case clap::event::key::kp_9: return s << "keypad 9";
		case clap::event::key::kp_point: return s << "keypad point";
		case clap::event::key::kp_divide: return s << "keypad divide";
		case clap::event::key::kp_multiply: return s << "keypad multiply";
		case clap::event::key::kp_subtract: return s << "keypad subtract";
		case clap::event::key::kp_add: return s << "keypad add";
		case clap::event::key::kp_enter: return s << "keypad enter";
		case clap::event::key::kp_equal: return s << "keypad equal";
		case clap::event::key::shift_left: return s << "left shift";
		case clap::event::key::control_left: return s << "left control";
		case clap::event::key::alt_left: return s << "left alt";
		case clap::event::key::super_left: return s << "left super";
		case clap::event::key::shift_right: return s << "right shift";
		case clap::event::key::control_right: return s << "right control";
		case clap::event::key::alt_right: return s << "right alt";
		case clap::event::key::super_right: return s << "right super";
		case clap::event::key::menu: return s << "menu";
	}
	clap::log::warning::critical << "Unsupported enum value.";
	return s;
}
std::ostream &operator<<(std::ostream &s, clap::event::key_action v) {
	switch (v) {
		case clap::event::key_action::press: return s << "pressed";
		case clap::event::key_action::release: return s << "released";
		case clap::event::key_action::repeat: return s << "repeated";
	}
	clap::log::warning::critical << "Unsupported enum value.";
	return s;
}
std::ostream &operator<<(std::ostream &s, clap::event::mouse_button v) {
	switch (v) {
		case clap::event::mouse_button::b_1: return s << "first (left) mouse button";
		case clap::event::mouse_button::b_2: return s << "second (right) mouse button";
		case clap::event::mouse_button::b_3: return s << "third (middle) mouse button";
		case clap::event::mouse_button::b_4: return s << "fourth (xButton1) mouse button";
		case clap::event::mouse_button::b_5: return s << "fifth (xButton2) mouse button";
		case clap::event::mouse_button::b_6: return s << "sixth mouse button";
		case clap::event::mouse_button::b_7: return s << "seventh mouse button";
		case clap::event::mouse_button::b_8: return s << "eighth mouse button";
	}
	clap::log::warning::critical << "Unsupported enum value.";
	return s;
}
std::ostream &operator<<(std::ostream &s, clap::event::mouse_button_action v) {
	switch (v) {
		case clap::event::mouse_button_action::press: return s << "pressed";
		case clap::event::mouse_button_action::release: return s << "released";
	}
	clap::log::warning::critical << "Unsupported enum value.";
	return s;
}
std::ostream &operator<<(std::ostream &s, clap::event::mouse_hover_action v) {
	switch (v) {
		case clap::event::mouse_hover_action::enter: return s << "entered";
		case clap::event::mouse_hover_action::leave: return s << "left";
	}
	clap::log::warning::critical << "Unsupported enum value.";
	return s;
}
std::ostream &operator<<(std::ostream &s, clap::event::modificator_mask v) {
	if (static_cast<underlying>(v & clap::event::modificator_mask::super))
		s << "Super +";
	if (static_cast<underlying>(v & clap::event::modificator_mask::alt))
		s << "Alt +";
	if (static_cast<underlying>(v & clap::event::modificator_mask::ctrl))
		s << "Ctrl +";
	if (static_cast<underlying>(v & clap::event::modificator_mask::shift))
		s << "Shift +";
	return s;
}
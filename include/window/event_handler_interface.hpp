#pragma once
#include "window/event_type.hpp"

#pragma warning(push)
#pragma warning(disable : 4100)

namespace clap::event {
	namespace detail {
		class impl;
	}

	class handler_interface {
		friend clap::event::detail::impl;
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
		inline virtual bool on_mouse_hover(event::mouse_hover_action action) {
			return false;
		}
		inline virtual bool on_scroll(double dx, double dy) {
			return false;
		}
		inline virtual bool on_resize(size_t x, size_t y) {
			return false;
		}
		inline virtual bool on_file_drop(int count, const char8_t **paths) {
			return false;
		}

	};
}

#pragma warning(pop)
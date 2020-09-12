#pragma once
#include <map>

#include "ui/zone.hpp"
#include "ui/detail/glfw.hpp"

namespace clap::ui::detail {
	class state {
	public:
		static std::map<ui::zone *, glfw::window_handle> zone_window_map;
		static glfw::window_handle &get(ui::zone *zone_ptr);
		static glfw::window_handle &add(ui::zone *zone_ptr);
		static bool should_close();
	};
}
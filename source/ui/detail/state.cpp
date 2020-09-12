#include "ui/detail/state.hpp"

#include "essential/log.hpp"
#include "gl/detail/state.hpp"

std::map<clap::ui::zone *, clap::ui::detail::glfw::window_handle> clap::ui::detail::state::zone_window_map;

clap::ui::detail::glfw::window_handle &clap::ui::detail::state::get(ui::zone *zone_ptr) {
	auto ret_iterator = zone_window_map.find(zone_ptr);
	if (ret_iterator != zone_window_map.end())
		return ret_iterator->second;
	else
		clap::log::error::critical << "Trying to get a window for unregistered zone.";
}

clap::ui::detail::glfw::window_handle &clap::ui::detail::state::add(ui::zone *zone_ptr) {
	if (zone_ptr) {
		if (!zone_ptr->owner) {
			auto iterator_pair = zone_window_map.insert(
				std::pair(
					zone_ptr,
					clap::ui::detail::glfw::create_window_windowed(zone_ptr->name,
																   zone_ptr->size.w(),
																   zone_ptr->size.h())
				));
			if (iterator_pair.second)
				return iterator_pair.first->second;
			else
				clap::log::warning::critical << "Cannot add created window to the map. 'std::map::insert()' has returned false";
		} else {
			clap::log::error::critical << "Cannot create window for an owned zone.";
			clap::log::info::minor << "Owner: " << zone_ptr->owner << ".";
		}
	} else
		clap::log::error::critical << "Cannot created window for a 'nulltpr' zone.";
}

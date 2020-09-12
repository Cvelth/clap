#include "ui/detail/state.hpp"
#include "ui/detail/glfw.hpp"

#include "essential/log.hpp"
#include "gl/detail/state.hpp"

std::map<clap::ui::zone *, clap::ui::detail::glfw::window_handle> clap::ui::detail::state::zone_window_map;
std::shared_mutex clap::ui::detail::state::mutex;

std::vector<clap::ui::zone *> adding_queue;
std::mutex adding_mutex;
std::vector<clap::ui::zone *> removing_queue;
std::mutex removing_mutex;

clap::ui::detail::glfw::window_handle &clap::ui::detail::state::get(ui::zone *zone_ptr) {
	std::shared_lock guard(mutex);
	auto ret_iterator = zone_window_map.find(zone_ptr);
	if (ret_iterator != zone_window_map.end())
		return ret_iterator->second;
	else
		clap::log::error::critical << "Trying to get a window for a zone without one.";
}

clap::ui::detail::glfw::window_handle &clap::ui::detail::state::add(ui::zone *zone_ptr) {
	if (zone_ptr) {
		if (!zone_ptr->owner) {
			std::unique_lock guard(mutex);
			auto iterator_pair = zone_window_map.insert(
				std::pair(
					zone_ptr,
					clap::ui::detail::glfw::create_window_windowed(zone_ptr->name,
																   zone_ptr->size.w(),
																   zone_ptr->size.h())
				));
			if (iterator_pair.second) {
				std::lock_guard guard(adding_mutex);
				adding_queue.push_back(zone_ptr);
				return iterator_pair.first->second;
			} else
				clap::log::warning::critical << "Cannot add created window to the map. 'std::map::insert()' has returned false";
		} else {
			clap::log::error::critical << "Cannot create window for an owned zone.";
			clap::log::info::minor << "Owner: " << zone_ptr->owner << ".";
		}
	} else
		clap::log::error::critical << "Cannot created window for a 'nulltpr' zone.";
}

void clap::ui::detail::state::remove(ui::zone *zone_ptr) {
	std::lock_guard guard(removing_mutex);
	removing_queue.push_back(zone_ptr);
}

void clap::ui::detail::state::update_additions(std::function<void(ui::zone *, glfw::window_handle &)> on_added) {
	std::lock_guard guard(adding_mutex);
	while (!adding_queue.empty()) {
		mutex.lock_shared();
		auto iterator = zone_window_map.find(adding_queue.back());
		bool found = (iterator != zone_window_map.end());
		mutex.unlock_shared();

		if (found)
			on_added(iterator->first, iterator->second);
		else
			clap::log::warning::critical << "Requested zone wasn't found in the map.";

		adding_queue.pop_back();
	}
}
void clap::ui::detail::state::update_removals() {
	std::lock_guard guard(removing_mutex);
	while (!removing_queue.empty()) {
		mutex.lock_shared();
		auto iterator = zone_window_map.find(removing_queue.back());
		bool found = (iterator != zone_window_map.end());
		mutex.unlock_shared();

		if (found) 
			zone_window_map.erase(iterator);
		else
			clap::log::warning::critical << "Requested zone wasn't found in the map.";

		removing_queue.pop_back();
	}
}

bool clap::ui::detail::state::should_close() {
	std::shared_lock guard(mutex);
	for (auto &pair : zone_window_map)
		if (!pair.second.should_close())
			return false;
	return true;
}

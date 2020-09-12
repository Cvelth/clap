#pragma once
#include <functional>
#include <map>
#include <shared_mutex>

#include "ui/zone.hpp"
#include "ui/detail/glfw.hpp"

namespace clap::ui::detail {
	class state {
	public:
		static std::map<ui::zone *, glfw::window_handle> zone_window_map;
		static glfw::window_handle &get(ui::zone *zone_ptr);
		static glfw::window_handle &add(ui::zone *zone_ptr);
		static void remove(ui::zone *zone_ptr);
		static bool should_close();
		
		static void update(std::function<void(ui::zone *, glfw::window_handle &)> on_added) {
			update_removals();
			update_additions(on_added);
		}

		static void lock_shared() {
			mutex.lock_shared();
		}
		static void unlock_shared() {
			mutex.unlock_shared();
		}
		static auto guard() {
			return std::shared_lock(mutex);
		}
	protected:
		static void update_additions(std::function<void(ui::zone *, glfw::window_handle &)> on_added);
		static void update_removals();
	private:
		static std::shared_mutex mutex;
	};
}
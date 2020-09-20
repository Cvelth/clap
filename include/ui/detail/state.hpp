#pragma once
#include "ui/zone.hpp"

#include "gl/detail/context.hpp"

#include <functional>
#include <map>
#include <shared_mutex>
#include <set>

namespace clap::gl::shader {
	class program;
}

namespace clap::ui::detail {
	class state {
	public:
		static gl::detail::context &get(ui::zone *zone_ptr);
		static gl::detail::context &add(ui::zone *zone_ptr);
		static void remove(ui::zone *zone_ptr);
		static bool should_close();
		
		static void update(std::function<void(ui::zone *, gl::detail::context &)> on_added) {
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
		static void update_additions(std::function<void(ui::zone *, gl::detail::context &)> on_added);
		static void update_removals();
	protected:
		static std::map<ui::zone *, gl::detail::context> zone_context_map;
	private:
		static std::shared_mutex mutex;
	};
}
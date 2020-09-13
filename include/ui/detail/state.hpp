#pragma once
#include <functional>
#include <map>
#include <shared_mutex>
#include <set>

#include "ui/zone.hpp"
#include "ui/detail/glfw.hpp"

namespace clap::gl::shader {
	class program;
}

namespace clap::ui::detail {
	class shader_program_pool : protected std::set<std::shared_ptr<gl::shader::program>> {

	};
	struct context {
		glfw::window_handle window;
		shader_program_pool programs;
	public:
		context(std::u8string name, size_t width, size_t height);
	};

	class state {
	public:
		static context &get(ui::zone *zone_ptr);
		static context &add(ui::zone *zone_ptr);
		static void remove(ui::zone *zone_ptr);
		static bool should_close();
		
		static void update(std::function<void(ui::zone *, context &)> on_added) {
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
		static void update_additions(std::function<void(ui::zone *, context &)> on_added);
		static void update_removals();
	protected:
		static std::map<ui::zone *, context> zone_context_map;
	private:
		static std::shared_mutex mutex;
	};
}
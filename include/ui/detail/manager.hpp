#pragma once
#include <shared_mutex>

namespace vkfw {
	class Window;
}

namespace clap::ui {
	class zone;
	namespace detail {
		class manager {
		public:
			static vkfw::Window *get(ui::zone &zone_ref);
			static vkfw::Window *add(ui::zone &zone_ref);
			static void remove(ui::zone &zone_ref);
			static bool empty();

			static void update();

		public:
			static std::function<void(zone*, vkfw::Window*)> zone_loop;

		private:
			static std::shared_mutex mutex;
		};
	}
}
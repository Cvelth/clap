#pragma once
#include <optional>
#include <shared_mutex>

namespace vkfw {
	class Window;
}
namespace vk {
	class SurfaceKHR;
}

namespace clap::ui {
	class zone;
	namespace detail {
		struct window_handle {
			::vkfw::Window &window;
			::vk::SurfaceKHR &surface;

			operator bool() const { return window && surface; }
		};

		class manager {
		public:
			static std::optional<window_handle> get(ui::zone &zone_ref);
			static std::optional<window_handle> add(ui::zone &zone_ref);
			static void remove(ui::zone &zone_ref);
			static bool empty();

			static void update();

		public:
			static std::function<void(zone*, window_handle)> zone_loop;

		private:
			static std::shared_mutex mutex;
		};
	}
}
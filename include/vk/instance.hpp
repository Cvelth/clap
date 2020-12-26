#pragma once
#include "precompiled/vk.hpp"

namespace clap::vk {
	namespace detail {
		class instance_t;
	}
	detail::instance_t &instance();

	namespace detail {
		class instance_t {
			friend instance_t &clap::vk::instance();
		public:
			vkfw::UniqueInstance vkfw;
			::vk::UniqueInstance vk;
			::vk::PhysicalDevice physical_device;
			::vk::UniqueDevice device;

			::vk::DispatchLoaderDynamic dynamic_loader;

#ifndef NDEBUG
			::vk::UniqueHandle<::vk::DebugUtilsMessengerEXT, decltype(dynamic_loader)> debug_messenger;
#endif
		protected:
			instance_t();
		};
	}
}

[[nodiscard]] inline clap::vk::detail::instance_t &clap::vk::instance() {
	static detail::instance_t instance;
	return instance;
}
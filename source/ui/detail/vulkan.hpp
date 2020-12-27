#pragma once
#include "precompiled/ui.hpp"

namespace clap::ui::vulkan {
	namespace detail {
		vkfw::UniqueInstance create_vkfw_instance();
		vk::UniqueInstance create_instance();
		vk::PhysicalDevice choose_device();
		vk::UniqueDevice initialize_device();
		vk::Queue initialize_queue();
		vk::DispatchLoaderDynamic initialize_loader();

#ifndef NDEBUG
		vk::UniqueHandle<vk::DebugUtilsMessengerEXT, decltype(initialize_loader())>
			initialize_debug_messenger();
#endif
	}

	[[nodiscard]] inline vkfw::Instance &vkfw() {
		static auto instance = detail::create_vkfw_instance();
		return *instance;
	}
	[[nodiscard]] inline vk::Instance &instance() {
		static auto instance = detail::create_instance();
		return *instance;
	}
	[[nodiscard]] inline vk::PhysicalDevice &physical_device() {
		static auto device = detail::choose_device();
		return device;
	}
	[[nodiscard]] inline vk::Device &device() {
		static auto device = detail::initialize_device();
		return *device;
	}
	[[nodiscard]] inline vk::Queue &queue() {
		static auto queue = detail::initialize_queue();
		return queue;
	}
	[[nodiscard]] inline vk::DispatchLoaderDynamic &loader() {
		static auto loader = detail::initialize_loader();
		return loader;
	}

#ifndef NDEBUG
	[[nodiscard]] inline vk::DebugUtilsMessengerEXT &debug_messenger() {
		static auto messenger = detail::initialize_debug_messenger();
		return *messenger;
	}
#endif
}
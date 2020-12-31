#pragma once
#include "precompiled/ui.hpp"

namespace clap::ui::vulkan {
	struct window_view {
		vkfw::Window vkfw;
		vk::SurfaceKHR surface;
		vk::SwapchainKHR swapchain;

		vk::Format &swapchain_image_format;
		vk::Extent2D &swapchain_extent;
	};

	struct window {
		vkfw::UniqueWindow vkfw;
		vk::UniqueSurfaceKHR surface;

		vk::UniqueSwapchainKHR swapchain;
		vk::Format swapchain_image_format;
		vk::Extent2D swapchain_extent;

		std::vector<vk::Image> swapchain_images;
		std::vector<vk::UniqueImageView> swapchain_image_views;

	public:
		explicit window(size_t width, size_t height,
						std::string_view title,
						vkfw::WindowHints hints = {});
		operator bool() const { return vkfw && surface && swapchain; }
		operator window_view() {
			return window_view {
				vkfw ? *vkfw : vkfw::Window{},
				surface ? *surface : vk::SurfaceKHR{},
				swapchain ? *swapchain : vk::SwapchainKHR{},
				swapchain_image_format,
				swapchain_extent
			};
		}
	};
}
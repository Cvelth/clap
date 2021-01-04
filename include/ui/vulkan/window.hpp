#pragma once
#include "precompiled/ui.hpp"

namespace clap::ui::vulkan {
	struct window_view {
		vkfw::Window vkfw;
		vk::SurfaceKHR surface;
		vk::SwapchainKHR swapchain;
		vk::RenderPass render_pass;
		std::span<vk::UniqueFramebuffer> framebuffers;

		vk::Format &image_format;
		vk::Extent2D &extent;
	};

	struct window {
		vkfw::UniqueWindow vkfw;
		vk::UniqueSurfaceKHR surface;
		vk::SurfaceFormatKHR surface_format;
		vk::PresentModeKHR present_mode;
		vk::Extent2D extent;

		vk::UniqueSwapchainKHR swapchain;

		std::vector<vk::UniqueImageView> swapchain_image_views;
		vk::UniqueRenderPass render_pass;
		std::vector<vk::UniqueFramebuffer> framebuffers;

	public:
		explicit window(size_t width, size_t height,
						std::string_view title,
						vkfw::WindowHints hints = {});
		void do_resize(size_t new_width, size_t new_height);
		operator bool() const { 
			return vkfw && surface && swapchain && render_pass && !framebuffers.empty(); 
		}
		operator window_view() {
			return window_view {
				vkfw ? *vkfw : vkfw::Window{},
				surface ? *surface : vk::SurfaceKHR{},
				swapchain ? *swapchain : vk::SwapchainKHR{},
				render_pass ? *render_pass : vk::RenderPass{},
				{ framebuffers.data(), framebuffers.size() },
				surface_format.format,
				extent
			};
		}
	};
}
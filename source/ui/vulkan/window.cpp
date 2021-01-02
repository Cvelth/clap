#include "precompiled/ui.hpp"

#include "resource/resource.hpp"
#include "ui/vulkan/window.hpp"
#include "ui/vulkan/core.hpp"
#include "ui/zone.hpp"

void log(vk::SurfaceCapabilitiesKHR const &capabilities,
		 std::vector<vk::SurfaceFormatKHR> const &formats,
		 std::vector<vk::PresentModeKHR> const &modes) {
	auto logger_stream = clap::log.add_entry();
	logger_stream << cL::message << cL::negligible << "clap"_tag << "ui"_tag << "manager"_tag << "swapchain"_tag;

	logger_stream << "Surface capabilities:"
		<< "\n    Image count: " << capabilities.minImageCount << " to "
		<< capabilities.maxImageCount
		<< "\n    Image extent: {" << capabilities.currentExtent.width << ", "
		<< capabilities.currentExtent.height << "} ({"
		<< capabilities.minImageExtent.width << ", "
		<< capabilities.minImageExtent.height << "} to {"
		<< capabilities.maxImageExtent.width << ", "
		<< capabilities.maxImageExtent.height << "})"
		<< "\n    Image array layers: " << capabilities.maxImageArrayLayers;

	if (!formats.empty()) {
		logger_stream << "\nAvailable surface formats (" << formats.size() << "):";
		for (auto const &format : formats)
			logger_stream << "\n    " << vk::to_string(format.format) << " (color space: "
				<< vk::to_string(format.colorSpace) << ")";
	} else
		logger_stream << "\nNo available surface formats.";

	if (!modes.empty()) {
		logger_stream << "\nAvailable surface modes (" << modes.size() << "):";
		for (auto const &mode : modes)
			logger_stream << "\n    " << vk::to_string(mode);
	} else
		logger_stream << "\nNo available surface modes.";
}

clap::ui::vulkan::window::window(size_t width, size_t height,
								 std::string_view title, 
								 vkfw::WindowHints hints) {
	resource_manager::identify();

	if (vulkan::vkfw())
		vkfw = vkfw::createWindowUnique(
			width, height, title.data(),
			hints, nullptr, nullptr, false
		);
	if (auto &instance = vulkan::instance(); instance)
		surface = vkfw::createWindowSurfaceUnique(
			instance, *vkfw
		);
	if (auto &physical_device = vulkan::physical_device(); physical_device && surface && vkfw)
		if (auto &queue = vulkan::queue(); queue) {
			auto support = physical_device.getSurfaceSupportKHR(vulkan::queue_family_id(), *surface);
			if (!support)
				clap::log << cL::error << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag << "swapchain"_tag
					<< "The Vulkan surface does not support chosen queue family.";
			else {
				auto capabilities = physical_device.getSurfaceCapabilitiesKHR(*surface);
				auto formats = physical_device.getSurfaceFormatsKHR(*surface);
				auto modes = physical_device.getSurfacePresentModesKHR(*surface);
				::log(capabilities, formats, modes);
				if (auto &device = vulkan::device(); device && !formats.empty() && !modes.empty()) {
					auto chosen_format = formats.front();
					for (auto const &format : formats)
						if (format.format == vk::Format::eB8G8R8A8Srgb
							&& format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {

							chosen_format = format;
							break;
						}
					auto chosen_mode = modes.front();
					for (auto const &mode : modes)
						if (mode == vk::PresentModeKHR::eMailbox) {
							chosen_mode = mode;
							break;
						}
					auto chosen_extent = capabilities.currentExtent;
					if (chosen_extent.width == std::numeric_limits<uint32_t>::max())
						chosen_extent = decltype(chosen_extent) {
							std::clamp(static_cast<uint32_t>(vkfw->getFramebufferWidth()),
									   capabilities.minImageExtent.width,
									   capabilities.maxImageExtent.width),
							std::clamp(static_cast<uint32_t>(vkfw->getFramebufferHeight()),
									   capabilities.minImageExtent.height,
									   capabilities.maxImageExtent.height)
					};
					auto chosen_image_count = std::clamp(capabilities.minImageCount + 1,
														 capabilities.minImageCount,
														 (capabilities.maxImageCount == 0
														  ? ~0U 
														  : capabilities.maxImageCount));

					clap::log << cL::message << cL::minor << "clap"_tag << "ui"_tag << "manager"_tag << "swapchain"_tag
						<< "Create a swapchain with parameters: "
						<< "\n    Image count: " << chosen_image_count
						<< "\n    Image format: " << vk::to_string(chosen_format.format)
						<< "\n    Image color space: " << vk::to_string(chosen_format.colorSpace)
						<< "\n    Image extent: {" << capabilities.currentExtent.width << ", "
						<< capabilities.currentExtent.height << "}"
						<< "\n    Image array layer count: 1"
						<< "\n    Image usage: " << vk::to_string(vk::ImageUsageFlagBits::eColorAttachment)
						<< "\n    Image sharing mode: " << vk::to_string(vk::SharingMode::eExclusive)
						<< "\n    Composite alpha: " << vk::to_string(vk::CompositeAlphaFlagBitsKHR::eOpaque)
						<< "\n    Present mode: " << vk::to_string(chosen_mode)
						<< "\n    Clipped: true";

					vk::SwapchainCreateInfoKHR swapchain_info = {
						.surface = *surface,
						.minImageCount = chosen_image_count,
						.imageFormat = chosen_format.format,
						.imageColorSpace = chosen_format.colorSpace,
						.imageExtent = chosen_extent,
						.imageArrayLayers = 1,
						.imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
						.imageSharingMode = vk::SharingMode::eExclusive,
						.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
						.presentMode = chosen_mode,
						.clipped = true
					};
					swapchain = device.createSwapchainKHRUnique(swapchain_info);
					if (swapchain) {
						swapchain_image_format = chosen_format.format;
						swapchain_extent = chosen_extent;
						swapchain_images = device.getSwapchainImagesKHR(*swapchain);

						vk::ImageViewCreateInfo image_view_info = {
							//.image = &swapchain_images[image_index]
							.viewType = vk::ImageViewType::e2D,
							.format = swapchain_image_format,
							.subresourceRange = vk::ImageSubresourceRange {
								.aspectMask = vk::ImageAspectFlagBits::eColor,
								.baseMipLevel = 0,
								.levelCount = 1,
								.baseArrayLayer = 0,
								.layerCount = 1
							}
						};
						swapchain_image_views.reserve(swapchain_images.size());
						std::ranges::transform(
							swapchain_images, std::back_inserter(swapchain_image_views),
							[&image_view_info, &device](auto &image) {
								image_view_info.image = image;
								return device.createImageViewUnique(image_view_info);
							}
						);

						vk::AttachmentDescription color_attachment = {
							.format = swapchain_image_format,
							.samples = vk::SampleCountFlagBits::e1,
							.loadOp = vk::AttachmentLoadOp::eClear,
							.storeOp = vk::AttachmentStoreOp::eStore,
							.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
							.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
							.initialLayout = vk::ImageLayout::eUndefined,
							.finalLayout = vk::ImageLayout::ePresentSrcKHR
						};
						vk::AttachmentReference color_attachment_ref = {
							.attachment = 0,
							.layout = vk::ImageLayout::eColorAttachmentOptimal
						};
						vk::SubpassDescription subpass = {
							.pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
							.colorAttachmentCount = 1,
							.pColorAttachments = &color_attachment_ref
						};
						vk::SubpassDependency dependency = {
							.srcSubpass = VK_SUBPASS_EXTERNAL,
							.dstSubpass = 0,
							.srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
							.dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput,
							.srcAccessMask = vk::AccessFlags(0),
							.dstAccessMask = vk::AccessFlagBits::eColorAttachmentWrite
						};
						vk::RenderPassCreateInfo render_pass_info = {
							.attachmentCount = 1,
							.pAttachments = &color_attachment,
							.subpassCount = 1,
							.pSubpasses = &subpass,
							.dependencyCount = 1,
							.pDependencies = &dependency
						};
						render_pass = device.createRenderPassUnique(render_pass_info);
						if (render_pass) {
							vk::FramebufferCreateInfo framebuffer_info = {
								.renderPass = *render_pass,
								.attachmentCount = 1,
								//.pAttachments = &swapchain_image_views[framebuffer_index]
								.width = swapchain_extent.width,
								.height = swapchain_extent.height,
								.layers = 1
							};
							framebuffers.reserve(swapchain_images.size());
							std::ranges::transform(
								swapchain_image_views, std::back_inserter(framebuffers),
								[&framebuffer_info, &device](auto &image_view) -> vk::UniqueFramebuffer {
									if (image_view) {
										framebuffer_info.pAttachments = &*image_view;
										return device.createFramebufferUnique(framebuffer_info);
									} else
										return {};
								}
							);
						}
					}
				}
			}
		}
}

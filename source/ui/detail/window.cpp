#include "precompiled/ui.hpp"

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

clap::ui::detail::window::window(size_t width, size_t height, 
								 std::string_view title, 
								 vkfw::WindowHints hints) {
	resource::identify();

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
														 capabilities.maxImageCount);

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

					vk::SwapchainCreateInfoKHR swapchain_info{
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

						vk::ImageViewCreateInfo image_view_info{
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
					}
				}
			}
		}
}

#include "precompiled/ui.hpp"

#include <unordered_map>

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

struct owning_window_handle {
	vkfw::UniqueWindow window;
	vk::UniqueSurfaceKHR surface;
	vk::UniqueSwapchainKHR swapchain;

	explicit owning_window_handle(clap::ui::zone &zone_ref, std::string_view title) {
		using namespace clap::ui;
		if (vulkan::vkfw())
			window = vkfw::createWindowUnique(
				zone_ref.width(), zone_ref.height(), title.data(),
				vkfw::WindowHints{}, nullptr, nullptr, false
			);
		if (auto &instance = vulkan::instance(); instance)
			surface = vkfw::createWindowSurfaceUnique(
				instance, *window
			);
		if (auto &physical_device = vulkan::physical_device(); physical_device && surface && window)
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
								std::clamp(static_cast<uint32_t>(window->getFramebufferWidth()),
										   capabilities.minImageExtent.width,
										   capabilities.maxImageExtent.width),
								std::clamp(static_cast<uint32_t>(window->getFramebufferHeight()),
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
					}
				}
			}
	}
	operator bool() const { return window && surface && swapchain; }
	operator clap::ui::detail::window_handle() {
		return clap::ui::detail::window_handle{ *window, *surface, *swapchain };
	}
};


static std::unordered_map<clap::ui::zone *, owning_window_handle> zone_registry;
static std::vector<clap::ui::zone *> added_queue, removed_queue;
static std::mutex local_mutex;

std::function<void(clap::ui::zone *, clap::ui::detail::window_handle)> clap::ui::detail::manager::zone_loop;
std::shared_mutex clap::ui::detail::manager::mutex;

std::optional<clap::ui::detail::window_handle> clap::ui::detail::manager::get(ui::zone &zone_ref) {
	std::shared_lock guard(manager::mutex);
	if (auto iterator = zone_registry.find(&zone_ref); iterator != zone_registry.end())
		if (iterator->second)
			return iterator->second;
		else {
			clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
				<< "Fail to obtain a 'vkfw::Window' object for " << zone_ref.name()
				<< cL::extra << "'vkfw::WindowUnique->operator bool()' has returned 'false'.";
			manager::remove(zone_ref);
		}
	else
		clap::log << cL::warning << cL::major << "clap"_tag << "ui"_tag << "manager"_tag
			<< "Fail to obtain a 'vkfw::Window' object for " << zone_ref.name()
			<< cL::extra << "It's possible the zone wasn't registered or was already destroyed.";
	return std::nullopt;
}

std::optional<clap::ui::detail::window_handle> clap::ui::detail::manager::add(ui::zone &zone_ref) {
	return std::visit(
		utility::overload{
			[&zone_ref](zone::when_free &state) -> std::optional<window_handle> {
				std::unique_lock guard(manager::mutex);
				auto [iterator, success] = zone_registry.try_emplace(&zone_ref, zone_ref, state.name);
				if (success) {
					if (iterator->second) {
						std::lock_guard local_guard(local_mutex);
						added_queue.emplace_back(&zone_ref);
						vkfw::postEmptyEvent();
						return iterator->second;
					} else {
						clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
							<< "Fail to create a window handle for " << zone_ref.name()
							<< cL::extra << "'owning_window_handle::operator bool()' has returned 'false'.";
						manager::remove(zone_ref);
					}
				} else
					clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
						<< "Fail to add " << zone_ref.name() << " to the registry. "
							"'std::unordered_map::emplace()' has returned false.";
				return std::nullopt;
			},
			[&zone_ref](zone::when_owned &) -> std::optional<window_handle> {
				clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
					<< "Fail to add " << zone_ref.name() << " to the registry. "
						"Only free zones can be added.";
				return std::nullopt;
			}
		}, zone_ref.state
	);
}

void clap::ui::detail::manager::remove(ui::zone &zone_ref) {
	std::lock_guard guard(local_mutex);
	removed_queue.emplace_back(&zone_ref);
	vkfw::postEmptyEvent();
}

bool clap::ui::detail::manager::empty() {
	return zone_registry.empty();
}

void clap::ui::detail::manager::update() {
	std::lock_guard local_guard(local_mutex);
	while (!added_queue.empty()) {
		std::unique_lock guard(manager::mutex);
		if (auto iterator = zone_registry.find(added_queue.back()); iterator != zone_registry.end())
			if (manager::zone_loop) {
				if (iterator->second) {
					manager::zone_loop(iterator->first, iterator->second);
					clap::log << cL::message << cL::minor << "clap"_tag << "ui"_tag << "manager"_tag
						<< "Add " << added_queue.back()->name() << " to the registry.";
				} else
					clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
						<< "Fail to update a 'vkfw::Window' object."
						<< cL::extra << "'vkfw::WindowUnique->operator bool()' has returned 'false'.";
			} else
				clap::log << cL::warning << cL::major << "clap"_tag << "ui"_tag << "manager"_tag
					<< "Add a zone without running 'manager::zone_loop(...)'."
					<< cL::extra << "The function is empty.";
		else
			clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
				<< "Fail to add " << added_queue.back()->name() << " to the registry.";
		added_queue.pop_back();
	}
	while (!removed_queue.empty()) {
		std::unique_lock guard(manager::mutex);
		if (auto iterator = zone_registry.find(removed_queue.back()); iterator != zone_registry.end()) {
			zone_registry.erase(iterator);
			clap::log << cL::message << cL::minor << "clap"_tag << "ui"_tag << "manager"_tag
				<< "Remove " << removed_queue.back()->name() << " from the registry.";
		} else
			clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
				<< "Fail to remove " << removed_queue.back()->name() << " from the registry. "
					"It was either never added or already removed.";
		removed_queue.pop_back();
	}
}

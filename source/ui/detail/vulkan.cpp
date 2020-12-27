#include "precompiled/ui.hpp"

template <typename value_t>
concept Printable = requires (std::ostream &stream, value_t value) {
	{ stream << value } -> std::convertible_to<std::ostream &>;
};
template <typename extension_t>
concept VulkanExtension = requires (extension_t extension) {
	{ extension.extensionName } -> Printable;
	{ extension.specVersion } -> Printable;
};
template <typename layer_t>
concept VulkanLayer = requires (layer_t layer) {
	{ layer.layerName } -> Printable;
	{ layer.specVersion } -> Printable;
};

template <clap::utility::iteratable container_t>
inline void log(container_t const &container, std::string_view description, std::string_view when_empty) {
	if (!container.empty()) {
		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::message << cL::insignificant << "clap"_tag << "ui"_tag << "vulkan"_tag << "initialize"_tag
			<< description << " (" << container.size() << "):";
		for (auto const &element : container)
			if constexpr (VulkanExtension<decltype(element)>)
				logger_stream << "\n    " << element.extensionName
					<< " (version: " << element.specVersion << ")";
			else if constexpr (VulkanLayer<decltype(element)>)
				logger_stream << "\n    " << element.layerName << " (version: "
					<< VK_VERSION_MAJOR(element.specVersion) << "."
					<< VK_VERSION_MINOR(element.specVersion) << "."
					<< VK_VERSION_PATCH(element.specVersion) << ")";
			else
				logger_stream << "\n    " << element;
	} else
		clap::log << cL::message << cL::insignificant << "clap"_tag << "ui"_tag << "vulkan"_tag << "initialize"_tag
		<< when_empty;
}
static vk::PhysicalDeviceFeatures log(vk::PhysicalDeviceFeatures device_features) {
	clap::log << cL::message << cL::negligible << "clap"_tag << "ui"_tag << "vulkan"_tag << "initialize"_tag
		<< "Features available to the chosen physical device:"
		<< "\n    " << "robustBufferAccess: " << (device_features.robustBufferAccess ? "true" : "false")
		<< "\n    " << "fullDrawIndexUint32: " << (device_features.fullDrawIndexUint32 ? "true" : "false")
		<< "\n    " << "imageCubeArray: " << (device_features.imageCubeArray ? "true" : "false")
		<< "\n    " << "independentBlend: " << (device_features.independentBlend ? "true" : "false")
		<< "\n    " << "geometryShader: " << (device_features.geometryShader ? "true" : "false")
		<< "\n    " << "tessellationShader: " << (device_features.tessellationShader ? "true" : "false")
		<< "\n    " << "sampleRateShading: " << (device_features.sampleRateShading ? "true" : "false")
		<< "\n    " << "dualSrcBlend: " << (device_features.dualSrcBlend ? "true" : "false")
		<< "\n    " << "logicOp: " << (device_features.logicOp ? "true" : "false")
		<< "\n    " << "multiDrawIndirect: " << (device_features.multiDrawIndirect ? "true" : "false")
		<< "\n    " << "drawIndirectFirstInstance: " << (device_features.drawIndirectFirstInstance ? "true" : "false")
		<< "\n    " << "depthClamp: " << (device_features.depthClamp ? "true" : "false")
		<< "\n    " << "depthBiasClamp: " << (device_features.depthBiasClamp ? "true" : "false")
		<< "\n    " << "fillModeNonSolid: " << (device_features.fillModeNonSolid ? "true" : "false")
		<< "\n    " << "depthBounds: " << (device_features.depthBounds ? "true" : "false")
		<< "\n    " << "wideLines: " << (device_features.wideLines ? "true" : "false")
		<< "\n    " << "largePoints: " << (device_features.largePoints ? "true" : "false")
		<< "\n    " << "alphaToOne: " << (device_features.alphaToOne ? "true" : "false")
		<< "\n    " << "multiViewport: " << (device_features.multiViewport ? "true" : "false")
		<< "\n    " << "samplerAnisotropy: " << (device_features.samplerAnisotropy ? "true" : "false")
		<< "\n    " << "textureCompressionETC2: " << (device_features.textureCompressionETC2 ? "true" : "false")
		<< "\n    " << "textureCompressionASTC_LDR: " << (device_features.textureCompressionASTC_LDR ? "true" : "false")
		<< "\n    " << "textureCompressionBC: " << (device_features.textureCompressionBC ? "true" : "false")
		<< "\n    " << "occlusionQueryPrecise: " << (device_features.occlusionQueryPrecise ? "true" : "false")
		<< "\n    " << "pipelineStatisticsQuery: " << (device_features.pipelineStatisticsQuery ? "true" : "false")
		<< "\n    " << "vertexPipelineStoresAndAtomics: " << (device_features.vertexPipelineStoresAndAtomics ? "true" : "false")
		<< "\n    " << "fragmentStoresAndAtomics: " << (device_features.fragmentStoresAndAtomics ? "true" : "false")
		<< "\n    " << "shaderTessellationAndGeometryPointSize: " << (device_features.shaderTessellationAndGeometryPointSize ? "true" : "false")
		<< "\n    " << "shaderImageGatherExtended: " << (device_features.shaderImageGatherExtended ? "true" : "false")
		<< "\n    " << "shaderStorageImageExtendedFormats: " << (device_features.shaderStorageImageExtendedFormats ? "true" : "false")
		<< "\n    " << "shaderStorageImageMultisample: " << (device_features.shaderStorageImageMultisample ? "true" : "false")
		<< "\n    " << "shaderStorageImageReadWithoutFormat: " << (device_features.shaderStorageImageReadWithoutFormat ? "true" : "false")
		<< "\n    " << "shaderStorageImageWriteWithoutFormat: " << (device_features.shaderStorageImageWriteWithoutFormat ? "true" : "false")
		<< "\n    " << "shaderUniformBufferArrayDynamicIndexing: " << (device_features.shaderUniformBufferArrayDynamicIndexing ? "true" : "false")
		<< "\n    " << "shaderSampledImageArrayDynamicIndexing: " << (device_features.shaderSampledImageArrayDynamicIndexing ? "true" : "false")
		<< "\n    " << "shaderStorageBufferArrayDynamicIndexing: " << (device_features.shaderStorageBufferArrayDynamicIndexing ? "true" : "false")
		<< "\n    " << "shaderStorageImageArrayDynamicIndexing: " << (device_features.shaderStorageImageArrayDynamicIndexing ? "true" : "false")
		<< "\n    " << "shaderClipDistance: " << (device_features.shaderClipDistance ? "true" : "false")
		<< "\n    " << "shaderCullDistance: " << (device_features.shaderCullDistance ? "true" : "false")
		<< "\n    " << "shaderFloat64: " << (device_features.shaderFloat64 ? "true" : "false")
		<< "\n    " << "shaderInt64: " << (device_features.shaderInt64 ? "true" : "false")
		<< "\n    " << "shaderInt16: " << (device_features.shaderInt16 ? "true" : "false")
		<< "\n    " << "shaderResourceResidency: " << (device_features.shaderResourceResidency ? "true" : "false")
		<< "\n    " << "shaderResourceMinLod: " << (device_features.shaderResourceMinLod ? "true" : "false")
		<< "\n    " << "sparseBinding: " << (device_features.sparseBinding ? "true" : "false")
		<< "\n    " << "sparseResidencyBuffer: " << (device_features.sparseResidencyBuffer ? "true" : "false")
		<< "\n    " << "sparseResidencyImage2D: " << (device_features.sparseResidencyImage2D ? "true" : "false")
		<< "\n    " << "sparseResidencyImage3D: " << (device_features.sparseResidencyImage3D ? "true" : "false")
		<< "\n    " << "sparseResidency2Samples: " << (device_features.sparseResidency2Samples ? "true" : "false")
		<< "\n    " << "sparseResidency4Samples: " << (device_features.sparseResidency4Samples ? "true" : "false")
		<< "\n    " << "sparseResidency8Samples: " << (device_features.sparseResidency8Samples ? "true" : "false")
		<< "\n    " << "sparseResidency16Samples: " << (device_features.sparseResidency16Samples ? "true" : "false")
		<< "\n    " << "sparseResidencyAliased: " << (device_features.sparseResidencyAliased ? "true" : "false")
		<< "\n    " << "variableMultisampleRate: " << (device_features.variableMultisampleRate ? "true" : "false")
		<< "\n    " << "inheritedQueries: " << (device_features.inheritedQueries ? "true" : "false");
	return device_features;
}

vkfw::UniqueInstance clap::ui::vulkan::detail::create_vkfw_instance() {
	auto output = vkfw::initUnique();
	if (output)
		clap::log << cL::message << cL::minor << "clap"_tag << "ui"_tag << "vulkan"_tag << "initialize"_tag
			<< "Create vkfw instance." << cL::extra
			<< "    VKFW version: " << VKFW_VERSION_MAJOR << "."
			<< VKFW_VERSION_MINOR << "." << VKFW_VERSION_PATCH << "\n"
			<< "    GLFW version: " << GLFW_VERSION_MAJOR << "."
			<< GLFW_VERSION_MINOR << "." << GLFW_VERSION_REVISION;
	else
		clap::log << cL::error << cL::important << "clap"_tag << "ui"_tag << "vulkan"_tag << "initialize"_tag
			<< "Fail to initialize vkfw.";
	return output;
}
vk::UniqueInstance clap::ui::vulkan::detail::create_instance() {
	resource::wait();

	clap::log << cL::message << cL::major << "clap"_tag << "ui"_tag << "vulkan"_tag << "initialize"_tag
		<< "Start '" << clap::configuration::application_name << "' (v"
		<< *clap::configuration::application_version_major << "."
		<< *clap::configuration::application_version_minor << "."
		<< *clap::configuration::application_version_patch << ").";

	const vk::ApplicationInfo application_info{
		.pApplicationName = clap::configuration::application_name->data(),
		.applicationVersion = VK_MAKE_VERSION(clap::configuration::application_version_major,
											  clap::configuration::application_version_minor,
											  clap::configuration::application_version_patch),
		.pEngineName = CLAP_NAME,
		.engineVersion = VK_MAKE_VERSION(CLAP_VERSION_MAJOR,
										 CLAP_VERSION_MINOR,
										 CLAP_VERSION_PATCH),
		.apiVersion = VK_MAKE_VERSION(1, 0, 0)
	};

	if (auto &vkfw_instance = vulkan::vkfw(); vkfw_instance) {
		resource::detail::update_instance_extensions(vkfw::getRequiredInstanceExtensions());
		::log(clap::configuration::instance_extensions,
			  "Required Vulkan instance extensions",
			  "No required instance extensions.");
		::log(vk::enumerateInstanceExtensionProperties(),
			  "Available Vulkan instance extensions",
			  "No available instance extensions.");

		::log(clap::configuration::instance_layers,
			  "Required Vulkan instance layers",
			  "No required instance layers.");
		::log(vk::enumerateInstanceLayerProperties(),
			  "Available Vulkan instance layers",
			  "No available instance layers.");

		std::vector<char const *> continuous_required_extensions;
		continuous_required_extensions.reserve(clap::configuration::instance_extensions.size());
		std::ranges::transform(clap::configuration::instance_extensions,
							   std::back_inserter(continuous_required_extensions),
							   [](std::string_view view) -> char const * { return view.data(); });
		std::vector<char const *> continuous_required_layers;
		continuous_required_layers.reserve(clap::configuration::instance_layers.size());
		std::ranges::transform(clap::configuration::instance_layers, 
							   std::back_inserter(continuous_required_layers),
							   [](std::string_view view) -> char const * { return view.data(); });

		vk::InstanceCreateInfo instance_info{
			.pApplicationInfo = &application_info,
			.enabledLayerCount = static_cast<uint32_t>(continuous_required_layers.size()),
			.ppEnabledLayerNames = continuous_required_layers.data(),
			.enabledExtensionCount = static_cast<uint32_t>(continuous_required_extensions.size()),
			.ppEnabledExtensionNames = continuous_required_extensions.data()
		};
		auto output = vk::createInstanceUnique(instance_info);
		if (output)
			clap::log << cL::message << cL::minor << "clap"_tag << "ui"_tag << "vulkan"_tag << "initialize"_tag
				<< "Initialize Vulkan.";
		else
			clap::log << cL::error << cL::important << "clap"_tag << "ui"_tag << "vulkan"_tag << "initialize"_tag
				<< "Fail to initialize Vulkan.";
		return output;
	}
	return {};
}
vk::PhysicalDevice clap::ui::vulkan::detail::choose_device() {
	if (auto &instance = vulkan::instance(); instance) {
		auto physical_devices = instance.enumeratePhysicalDevices();
		if (!physical_devices.empty()) {
			auto logger_stream = clap::log.add_entry();
			logger_stream << cL::message << cL::negligible << "clap"_tag << "ui"_tag << "instance"_tag << "initialize"_tag
				<< "Available physical devices with Vulkan support (" << physical_devices.size() << "):";
			for (auto const &available_device : physical_devices)
				logger_stream << "\n    " << available_device.getProperties().deviceName
					<< " (" << ::vk::to_string(available_device.getProperties().deviceType) << ")";
			logger_stream << "\nChoose the first one: " 
				<< physical_devices.front().getProperties().deviceName;
			return physical_devices.front();
		} else
			clap::log << cL::error << cL::critical << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
				<< "Unable to find a physical device with Vulkan support.";
	}
	return {};
}
vk::UniqueDevice clap::ui::vulkan::detail::initialize_device() {
	if (auto &physical_device = vulkan::physical_device(); physical_device) {
		auto queue_families = physical_device.getQueueFamilyProperties();
		if (!queue_families.empty()) {
			auto logger_stream = clap::log.add_entry();
			logger_stream << cL::message << cL::minor << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
				<< "Queue families available to the physical device (" << queue_families.size() << "):";
			for (auto &family : queue_families) {
				logger_stream << "\n    x" << family.queueCount << ": "
					<< (family.queueFlags & ::vk::QueueFlagBits::eGraphics ? "Graphics " : "")
					<< (family.queueFlags & ::vk::QueueFlagBits::eCompute ? "Compute " : "")
					<< (family.queueFlags & ::vk::QueueFlagBits::eTransfer ? "Transfer " : "")
					<< (family.queueFlags & ::vk::QueueFlagBits::eSparseBinding ? "SparseBinding " : "")
					<< (family.queueFlags & ::vk::QueueFlagBits::eProtected ? "Protected " : "")
					<< "(Glanularity: {"
					<< family.minImageTransferGranularity.width << ", "
					<< family.minImageTransferGranularity.height << ", "
					<< family.minImageTransferGranularity.depth << "})";
			}
			logger_stream << "\nChoose the first one.";
		} else
			clap::log << cL::warning << cL::major << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
				<< "No queue families available.";

		float priority = 1.f;
		::vk::DeviceQueueCreateInfo queue_info{
			.queueFamilyIndex = 0,
			.queueCount = 1,
			.pQueuePriorities = &priority
		};

		::log(configuration::device_extensions,
			  "Required Vulkan device extensions",
			  "No required device extensions.");
		::log(physical_device.enumerateDeviceExtensionProperties(),
			  "Available Vulkan device extensions",
			  "No available device extensions.");
		::log(configuration::device_layers,
			  "Required Vulkan device layers",
			  "No required device layers.");
		::log(physical_device.enumerateDeviceLayerProperties(),
			  "Available Vulkan device layers",
			  "No available device layers.");

		std::vector<char const *> continuous_required_extensions;
		continuous_required_extensions.reserve(clap::configuration::device_extensions->size());
		std::ranges::transform(clap::configuration::device_extensions,
							   std::back_inserter(continuous_required_extensions),
							   [](std::string const &view) -> char const * { return view.data(); });
		std::vector<char const *> continuous_required_layers;
		continuous_required_layers.reserve(clap::configuration::device_layers->size());
		std::ranges::transform(clap::configuration::device_layers,
							   std::back_inserter(continuous_required_layers),
							   [](std::string const &view) -> char const * { return view.data(); });

		auto device_features = ::log(physical_device.getFeatures());
		::vk::DeviceCreateInfo device_info = {
			.queueCreateInfoCount = 1,
			.pQueueCreateInfos = &queue_info,
			.enabledLayerCount = static_cast<uint32_t>(continuous_required_layers.size()),
			.ppEnabledLayerNames = continuous_required_layers.data(),
			.enabledExtensionCount = static_cast<uint32_t>(continuous_required_extensions.size()),
			.ppEnabledExtensionNames = continuous_required_extensions.data(),
			.pEnabledFeatures = &device_features
		};
		auto output = physical_device.createDeviceUnique(device_info);
		if (output)
			clap::log << cL::message << cL::minor << "clap"_tag << "ui"_tag << "vulkan"_tag << "initialize"_tag
			<< "Initialize a logical Vulkan device.";
		else
			clap::log << cL::error << cL::important << "clap"_tag << "ui"_tag << "vulkan"_tag << "initialize"_tag
			<< "Fail to initialize a logical Vulkan device.";
		return output;
	}
	return {};
}
vk::Queue clap::ui::vulkan::detail::initialize_queue() {
	if (auto &device = vulkan::device(); device)
		return device.getQueue(0, 0);
	return {};
}
vk::DispatchLoaderDynamic clap::ui::vulkan::detail::initialize_loader() {
	if (auto &instance = vulkan::instance(); instance)
		if (auto &device = vulkan::device(); device) {
			vk::DispatchLoaderDynamic output;
			output.init(instance, device);
			return output;
		}
	return {};
}

#ifndef NDEBUG
vk::UniqueHandle<vk::DebugUtilsMessengerEXT, decltype(clap::ui::vulkan::detail::initialize_loader())>
clap::ui::vulkan::detail::initialize_debug_messenger() {
	if (auto &instance = vulkan::instance(); instance) {
		using enum ::vk::DebugUtilsMessageSeverityFlagBitsEXT;
		using enum ::vk::DebugUtilsMessageTypeFlagBitsEXT;
		const ::vk::DebugUtilsMessengerCreateInfoEXT debug_utils_create_info = {
			.messageSeverity = eInfo | eWarning | eError | eVerbose,
			.messageType = eGeneral | ePerformance | eValidation,
			.pfnUserCallback = [](VkDebugUtilsMessageSeverityFlagBitsEXT,
								  VkDebugUtilsMessageTypeFlagsEXT,
								  VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
								  void *) -> VkBool32 {
				clap::log << cL::warning << cL::major << "clap"_tag << "ui"_tag << "vulkan"_tag << "validation_layer"_tag
					<< "validation_layer: " << pCallbackData->pMessage;
				return false;
			}
		};
		clap::log << cL::message << cL::major << "clap"_tag << "ui"_tag << "vulkan"_tag << "initialize"_tag
			<< "Setup Vulkan validation layer.";
		return instance.createDebugUtilsMessengerEXTUnique(debug_utils_create_info, nullptr, loader());
	}
	return {};
}
#endif

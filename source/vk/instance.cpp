#include "precompiled/vk.hpp"

#include "resource/resource.hpp"

using namespace std::literals;

static vk::PhysicalDeviceFeatures print(vk::PhysicalDeviceFeatures device_features) {
	clap::log << cL::message << cL::negligible << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
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

[[nodiscard]] inline static vkfw::UniqueInstance initialize_vkfw() {
	return vkfw::initUnique();
}
[[nodiscard]] inline static vk::UniqueInstance initialize_vulkan() {
	clap::log << cL::message << cL::major << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
		<< "Initialize '" << clap::configuration::application_name << "' (v"
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

	std::set<std::string_view> required_extensions = {
		vkfw::getRequiredInstanceExtensions().begin(),
		vkfw::getRequiredInstanceExtensions().end()
};
#ifndef NDEBUG
	required_extensions.emplace(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
	for (auto &extension : clap::configuration::instance_extensions)
		required_extensions.emplace(extension);
	if (!required_extensions.empty()) {
		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::message << cL::insignificant << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "Required Vulkan instance extensions (" << required_extensions.size() << "):";
		for (auto const &extension : required_extensions)
			logger_stream << "\n    " << extension;
	} else
		clap::log << cL::message << cL::insignificant << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "No instance extensions are required.";

	auto available_extensions = vk::enumerateInstanceExtensionProperties();
	if (!available_extensions.empty()) {
		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::message << cL::negligible << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "Available Vulkan instance extensions (" << available_extensions.size() << "):";
		for (auto const &extension : available_extensions)
			logger_stream << "\n    " << extension.extensionName << " (version: " << extension.specVersion << ")";
	} else
		clap::log << cL::message << cL::negligible << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "No instance extensions are available.";

	std::set<std::string_view> required_layers = {};
#ifndef NDEBUG 
	required_layers.emplace("VK_LAYER_KHRONOS_validation");
#endif
	for (auto &layer : clap::configuration::instance_layers)
		required_layers.emplace(layer);
	if (!required_layers.empty()) {
		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::message << cL::insignificant << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "Required Vulkan instance layers (" << required_layers.size() << "):";
		for (auto const &layer : required_layers)
			logger_stream << "\n    " << layer;
	} else
		clap::log << cL::message << cL::insignificant << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "No instance layers are required.";

	const auto available_layers = vk::enumerateInstanceLayerProperties();
	if (!available_layers.empty()) {
		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::message << cL::negligible << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "Available Vulkan instance layers (" << available_layers.size() << "):";
		for (auto const &layer : available_layers)
			logger_stream << "\n    " << layer.layerName << " (version: " 
				<< VK_VERSION_MAJOR(layer.specVersion) << "." 
				<< VK_VERSION_MINOR(layer.specVersion) << "." 
				<< VK_VERSION_PATCH(layer.specVersion) << ")";
	} else
		clap::log << cL::message << cL::negligible << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "No instance layers are available.";

	std::vector<char const *> continuous_required_extensions;
	continuous_required_extensions.reserve(required_extensions.size());
	std::ranges::transform(required_extensions, std::back_inserter(continuous_required_extensions),
						   [](std::string_view view) -> char const * { return view.data(); });
	std::vector<char const *> continuous_required_layers;
	continuous_required_layers.reserve(required_layers.size());
	std::ranges::transform(required_layers, std::back_inserter(continuous_required_layers),
						   [](std::string_view view) -> char const * { return view.data(); });

	vk::InstanceCreateInfo instance_info{
		.pApplicationInfo = &application_info,
		.enabledLayerCount = static_cast<uint32_t>(continuous_required_layers.size()),
		.ppEnabledLayerNames = continuous_required_layers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(continuous_required_extensions.size()),
		.ppEnabledExtensionNames = continuous_required_extensions.data()
	};
	return vk::createInstanceUnique(instance_info);
}
[[nodiscard]] inline static vk::UniqueDevice create_device(vk::PhysicalDevice const &physical_device) {
	if (!clap::configuration::device_extensions->empty()) {
		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::message << cL::insignificant << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "Required Vulkan device extensions (" << clap::configuration::device_extensions->size() << "):\n";
		for (auto const &extension : clap::configuration::device_extensions)
			logger_stream << "\n    " << extension;
	} else
		clap::log << cL::message << cL::insignificant << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
		<< "No device extensions are required.";

	auto available_extensions = ::vk::enumerateInstanceExtensionProperties();
	if (!available_extensions.empty()) {
		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::message << cL::negligible << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "Available Vulkan device extensions (" << available_extensions.size() << "):";
		for (auto const &extension : available_extensions)
			logger_stream << "\n    " << extension.extensionName << " (version: " << extension.specVersion << ")";
	} else
		clap::log << cL::message << cL::negligible << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
		<< "No device extensions are available.";

	if (!clap::configuration::device_layers->empty()) {
		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::message << cL::insignificant << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "Required Vulkan device layers (" << clap::configuration::device_layers->size() << "):";
		for (auto const &layer : clap::configuration::device_layers)
			logger_stream << "\n    " << layer;
	} else
		clap::log << cL::message << cL::insignificant << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
		<< "No device layers are required.";

	const auto available_layers = ::vk::enumerateInstanceLayerProperties();
	if (!available_layers.empty()) {
		auto logger_stream = clap::log.add_entry();
		logger_stream << cL::message << cL::negligible << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "Available Vulkan device layers (" << available_layers.size() << "):";
		for (auto const &layer : available_layers)
			logger_stream << "\n    " << layer.layerName << " (version: "
				<< VK_VERSION_MAJOR(layer.specVersion) << "."
				<< VK_VERSION_MINOR(layer.specVersion) << "."
				<< VK_VERSION_PATCH(layer.specVersion) << ")";
	} else
		clap::log << cL::message << cL::negligible << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
		<< "No device layers are available.";

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

	auto device_features = print(physical_device.getFeatures());

	::vk::DeviceCreateInfo device_info = {
		.queueCreateInfoCount = 0,
		.pQueueCreateInfos = nullptr,
		.enabledLayerCount = static_cast<uint32_t>(continuous_required_layers.size()),
		.ppEnabledLayerNames = continuous_required_layers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(continuous_required_extensions.size()),
		.ppEnabledExtensionNames = continuous_required_extensions.data(),
		.pEnabledFeatures = &device_features
	};
	return physical_device.createDeviceUnique(device_info);
}

clap::vk::detail::instance_t::instance_t() {
	clap::log << cL::message << cL::important << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
		<< "Initialize Vulkan.";

	vkfw = initialize_vkfw();
	clap::log << cL::message << cL::minor << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
		<< "Create vkfw instance." << cL::extra
		<< "    VKFW version: " << VKFW_VERSION_MAJOR << "." 
		<< VKFW_VERSION_MINOR << "." << VKFW_VERSION_PATCH << "\n"
		<< "    GLFW version: " << GLFW_VERSION_MAJOR << "." 
		<< GLFW_VERSION_MINOR << "." << GLFW_VERSION_REVISION;

	resource::wait();
	vk = initialize_vulkan();
	clap::log << cL::message << cL::minor << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
		<< "Create Vulkan instance.";

	auto physical_devices = vk->enumeratePhysicalDevices();
	if (physical_devices.empty())
		clap::log << cL::error << cL::critical << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "Unable to find a physical device with Vulkan support.";
	else {
		{
			auto logger_stream = clap::log.add_entry();
			logger_stream << cL::message << cL::negligible << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
				<< "Available physical devices with Vulkan support (" << physical_devices.size() << "):";
			for (auto const &available_device : physical_devices)
				logger_stream << "\n    " << available_device.getProperties().deviceName
				<< " (" << ::vk::to_string(available_device.getProperties().deviceType) << ")";
			physical_device = physical_devices.front();
			logger_stream << "Choose the first one: " << physical_device.getProperties().deviceName;
		}
		device = create_device(physical_device);
		clap::log << cL::message << cL::minor << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "Create Vulkan device.";
		dynamic_loader.init(*vk, *device);

#ifndef NDEBUG
		using enum ::vk::DebugUtilsMessageSeverityFlagBitsEXT;
		using enum ::vk::DebugUtilsMessageTypeFlagBitsEXT;
		const ::vk::DebugUtilsMessengerCreateInfoEXT debug_utils_create_info = {
			.messageSeverity = eInfo | eWarning | eError | eVerbose,
			.messageType = eGeneral | ePerformance | eValidation,
			.pfnUserCallback = [](VkDebugUtilsMessageSeverityFlagBitsEXT,
								  VkDebugUtilsMessageTypeFlagsEXT,
								  VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
								  void *) -> VkBool32 {
				clap::log << cL::warning << cL::major << "clap"_tag << "vk"_tag << "validation_layer"_tag
					<< "validation_layer: " << pCallbackData->pMessage;
				return false;
			}
		};
		debug_messenger = vk->createDebugUtilsMessengerEXTUnique(debug_utils_create_info, nullptr, dynamic_loader);
		clap::log << cL::message << cL::minor << "clap"_tag << "vk"_tag << "instance"_tag << "initialize"_tag
			<< "Setup Vulkan validation layer.";
#endif
	}
}
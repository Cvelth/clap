#pragma once
#include "essential/utility.hpp"

#include <filesystem>
#include <list>
#include <set>
#include <span>
#include <string>

namespace clap::resource_manager::detail {
	class configuration_loader;
	template<typename T>
	class value {
		friend configuration_loader;
	public:
		value(T &&default_value) : underlying(std::move(default_value)) {}
		T const *operator->() const { return &underlying; }
		T const &operator*() const { return underlying; }
		operator T const &() const { return underlying; }

		decltype(auto) begin() const requires utility::iteratable<T> { return underlying.begin(); }
		decltype(auto) end() const requires utility::iteratable<T> { return underlying.end(); }
		size_t size() const requires utility::iteratable<T> { return underlying.size(); }
		bool empty() const requires utility::iteratable<T> { return underlying.empty(); }

		template <typename another_T, typename = 
			typename std::enable_if<std::is_convertible<T const &, another_T>::value>::type>
		operator another_T() const { return static_cast<another_T>(underlying); }

		using underlying_t = T;
	private:
		T underlying;
	};

	void update_instance_extensions(std::span<char const *> extensions);
}
namespace clap::configuration {
	inline resource_manager::detail::value<std::string> application_name("Unnamed clap application");
	inline resource_manager::detail::value<uint32_t> application_version_major(0u);
	inline resource_manager::detail::value<uint32_t> application_version_minor(0u);
	inline resource_manager::detail::value<uint32_t> application_version_patch(0u);

	inline resource_manager::detail::value<std::set<std::string>> instance_extensions({});
	inline resource_manager::detail::value<std::set<std::string>> instance_layers({
#ifndef NDEBUG
		"VK_LAYER_KHRONOS_validation"
#endif
	});
	inline resource_manager::detail::value<std::set<std::string>> device_extensions({
		"VK_KHR_swapchain"
	});
	inline resource_manager::detail::value<std::set<std::string>> device_layers({
#ifndef NDEBUG
		"VK_LAYER_KHRONOS_validation"
#endif
	});

	inline resource_manager::detail::value<std::list<std::filesystem::path>> resource_paths({
		"resource",
		"../resource",
		"../../resource",
		"clap/resource",
		"../clap/resource",
		"../../clap/resource",
		"engine/resource",
		"../engine/resource",
		"../../engine/resource"
	});
	inline resource_manager::detail::value<std::set<std::string>> configuration_filenames({
		"configuration.yml"
	});
	inline resource_manager::detail::value<std::set<std::string>> ignored_resource_filenames({
		"license",
		"LICENSE",
		"license.md",
		"LICENSE.MD"
	});
}
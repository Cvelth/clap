﻿#pragma once
#include <filesystem>
#include <iterator>
#include <list>
#include <set>
#include <string>

namespace clap::resource {
	namespace detail {
		namespace concepts {
			template <typename T>
			concept iteratable = requires(T a) {
				{ a.begin() } -> std::input_iterator<>;
				{ a.end() } -> std::input_iterator<>;
			};
		}
		class configuration_loader;
		template<typename T>
		class value {
			friend configuration_loader;
		public:
			value(T &&default_value) : underlying(std::move(default_value)) {}
			T const *operator->() const { return &underlying; }
			T const &operator*() const { return underlying; }
			operator T const &() const { return underlying; }

			decltype(auto) begin() const requires concepts::iteratable<T> { return underlying.begin(); }
			decltype(auto) end() const requires concepts::iteratable<T> { return underlying.end(); }

			template <typename another_T, typename = 
				typename std::enable_if<std::is_convertible<T const &, another_T>::value>::type>
			operator another_T() const { return static_cast<another_T>(underlying); }

			using underlying_t = T;
		private:
			T underlying;
		};
	}
}
namespace clap::configuration {
	inline resource::detail::value<std::string> application_name("Unnamed clap application");
	inline resource::detail::value<uint32_t> application_version_major(0u);
	inline resource::detail::value<uint32_t> application_version_minor(0u);
	inline resource::detail::value<uint32_t> application_version_patch(0u);

	inline resource::detail::value<std::set<std::string>> instance_extensions({});
	inline resource::detail::value<std::set<std::string>> instance_layers({});
	inline resource::detail::value<std::set<std::string>> device_extensions({});
	inline resource::detail::value<std::set<std::string>> device_layers({});

	inline resource::detail::value<std::list<std::filesystem::path>> const default_resourse_paths({
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
	inline resource::detail::value<std::set<std::string>> const default_configuration_filenames({
		"configuration.yml"
	});
	inline resource::detail::value<std::set<std::string>> const default_ignored_resource_filenames({
		"license",
		"LICENSE",
		"license.md",
		"LICENSE.MD"
	});

	inline resource::detail::value<std::list<std::filesystem::path>> additional_resourse_paths({});
	inline resource::detail::value<std::set<std::string>> additional_configuration_filenames({});
	inline resource::detail::value<std::set<std::string>> ignored_resource_filenames({});
}
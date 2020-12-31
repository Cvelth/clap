#pragma once
#include "resource/configuration.hpp"

namespace clap::resource_manager {
	// The function is non-blocking. The configuration is loaded on a separate thread.
	// As such, it's recommended to call 'resource_manager::wait()' function before actually 
	//	   using the configuration values
	void load_configuration();
	void wait();

	// Implicitly calls 'resource_manager::load_configuration()'.
	void identify();
	bool were_identified();

	namespace detail {
		template <typename tag_t> class storage {
		public:
			static std::filesystem::directory_entry *get(std::string_view const &identificator);
			static std::filesystem::directory_entry *try_get(std::string_view const &identificator);
			inline decltype(auto) operator[](std::string_view const &identificator) {
				return get(identificator);
			}
		};

		class vertex; class fragment;
		class tesselation_control;
		class tesselation_evaluation;
		class geometry; class compute;

		class unknown;
	}
}

namespace clap::resource {
	namespace shader {
		extern resource_manager::detail::storage<resource_manager::detail::vertex> vertex;
		extern resource_manager::detail::storage<resource_manager::detail::fragment> fragment;
		extern resource_manager::detail::storage<resource_manager::detail::tesselation_control> tesselation_control;
		extern resource_manager::detail::storage<resource_manager::detail::tesselation_evaluation> tesselation_evaluation;
		extern resource_manager::detail::storage<resource_manager::detail::geometry> geometry;
		extern resource_manager::detail::storage<resource_manager::detail::compute> compute;
	}
	extern resource_manager::detail::storage<resource_manager::detail::unknown> unknown;
}
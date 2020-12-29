#pragma once
#include "resource/configuration.hpp"

namespace clap::resource {
	// The function is non-blocking with identification itself done on a separate thread.
	// As such, it's recommended to call 'resource::wait()' function before actually using resources 
	// and, especially, configuration.
	void identify();
	void wait();
	bool were_identified();
}

namespace std::filesystem {
	class directory_entry;
}
namespace clap::resource {
	namespace detail {
		template <typename tag_t>
		class storage {
		public:
			std::filesystem::directory_entry *get(std::string_view const &identificator);
			std::filesystem::directory_entry *try_get(std::string_view const &identificator);
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

	namespace shader {
		extern detail::storage<detail::vertex> vertex;
		extern detail::storage<detail::fragment> fragment;
		extern detail::storage<detail::tesselation_control> tesselation_control;
		extern detail::storage<detail::tesselation_evaluation> tesselation_evaluation;
		extern detail::storage<detail::geometry> geometry;
		extern detail::storage<detail::compute> compute;
	}
	extern detail::storage<detail::unknown> unknown;
}
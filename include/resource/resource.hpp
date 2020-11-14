#pragma once

// impromptu
// *********
#include <optional>
#include <string>

namespace std::filesystem {
	class directory_entry;
}
namespace clap::gl {
	namespace texture {
		class _2d;
	};
}

namespace clap::impromptu::resource {
	void identify();
	void clean_up();

	namespace detail {
		template <typename contained_t>
		class storage {
		public:
			std::optional<contained_t> get(std::u8string const &identificator);
			std::optional<contained_t> try_get(std::u8string const &identificator);
			
			inline auto operator[](std::u8string const &identificator) {
				return get(identificator);
			}
			inline auto operator[](std::string const &identificator) {
				return get((char8_t *) identificator.c_str());
			}
		};
	}

	//extern detail::storage<gl::texture::_2d> texture;
	extern detail::storage<std::filesystem::directory_entry> unknown;
}

// origin
// ******
#include <memory>
#include <optional>
#include <string>

namespace std::filesystem {
	class directory_entry;
}
namespace clap::gl {
	namespace shader::detail {
		class object;
	}
	namespace texture {
		class _2d;
	};
}
namespace clap::render {
	class font;
}

namespace clap::resource {
	void identify();
	void clear();

	namespace detail {
		template <typename return_t, typename param_t>
		class storage {
		public:
			return_t operator[](std::u8string const &identificator);
			inline auto operator[](std::string const &identificator) {
				return operator[]((char8_t *) identificator.c_str());
			}
		};
		class fragment_shader_t;
		class vertex_shader_t;
		class geometry_shader_t;
		class compute_shader_t;
		class tesselation_control_shader_t;
		class tesselation_evaluation_shader_t;
		class texture_t;
		class font_t;
		class unknown_t;
	}

	namespace shader {
		extern detail::storage<std::optional<gl::shader::detail::object>, detail::fragment_shader_t> fragment;
		extern detail::storage<std::optional<gl::shader::detail::object>, detail::vertex_shader_t> vertex;
		extern detail::storage<std::optional<gl::shader::detail::object>, detail::geometry_shader_t> geometry;
		extern detail::storage<std::optional<gl::shader::detail::object>, detail::compute_shader_t> compute;
		extern detail::storage<std::optional<gl::shader::detail::object>, detail::tesselation_control_shader_t> tesselation_control;
		extern detail::storage<std::optional<gl::shader::detail::object>, detail::tesselation_evaluation_shader_t> tesselation_evaluation;
	}
	extern detail::storage<std::shared_ptr<gl::texture::_2d>, detail::texture_t> texture;
	extern detail::storage<std::shared_ptr<render::font>, detail::font_t> font;
	extern detail::storage<std::filesystem::directory_entry const *, detail::unknown_t> unknown;
}

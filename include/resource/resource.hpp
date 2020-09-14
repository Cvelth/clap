#pragma once
#include <memory>
#include <string>

namespace std::filesystem {
	class directory_entry;
}
namespace clap::gl {
	namespace shader {
		class program;
	};
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
		class shader_storage {
		public:
			std::shared_ptr<clap::gl::shader::program> operator[](std::u8string const &identificator);
			inline auto operator[](std::string const &identificator) {
				return operator[]((char8_t *) identificator.c_str());
			}
		};
		class texture_storage {
		public:
			std::shared_ptr<clap::gl::texture::_2d> operator[](std::u8string const &identificator);
			inline auto operator[](std::string const &identificator) {
				return operator[]((char8_t *) identificator.c_str());
			}
		};
		class font_storage {
		public:
			std::shared_ptr<clap::render::font> operator[](std::u8string const &identificator);
			inline auto operator[](std::string const &identificator) {
				return operator[]((char8_t *) identificator.c_str());
			}
		};
		class unknown_storage {
		public:
			std::filesystem::directory_entry const* operator[](std::u8string const &identificator);
			inline auto const* operator[](std::string const &identificator) {
				return operator[]((char8_t *) identificator.c_str());
			}
		};
	}

	extern detail::shader_storage shader;
	extern detail::texture_storage texture;
	extern detail::font_storage font;
	extern detail::unknown_storage unknown;
}

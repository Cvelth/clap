#pragma once
#include <cstdint>
#include <string>
#include <string_view>

typedef unsigned int GLenum;

namespace engine::gl {
	class state;

	namespace shader {
		enum class type {
			fragment, vertex, geometry, compute, tesselation_control, tesselation_evaluation
		};
		class program;

		namespace detail {
			class object;
		}
		detail::object from_source(type type, std::string source);

		namespace detail {
			class object {
				friend program;
			public:
				object(type type);
				~object();

				object(object const &other) = delete;
				object(object &&other) noexcept : object(other.id) {}

				friend detail::object shader::from_source(type type, std::string source);

			protected:
				object(type type, std::string source);

			private:
				object(uint32_t id);

			private:
				uint32_t id;
			};
		}

		detail::object from_source(type type, std::string source);
		detail::object from_source(type type, std::string_view source);
		detail::object from_source(type type, char const *source);

		detail::object from_file(type type, std::string filename);
		detail::object from_file(type type, std::string_view filename);
		detail::object from_file(type type, char const *filename);

		class program {
			friend state;
		public:
			program();
			~program();

			template <typename ...Ts>
			program(Ts &&...ts) : program() {
				link(ts...);
			}

			program(program const &other) = delete;
			program(program &&other) noexcept : program(other.id) {}

			void add(detail::object &&object);
			template <typename ...Ts>
			void add(detail::object &&object, Ts &&...ts) {
				add(std::move(object));
				add(std::move(ts)...);
			}

			void link();

			template <typename ...Ts>
			void link(Ts &&...ts) {
				add(ts...);
				link();
			}

			void use();

		private:
			program(uint32_t id);

		private:
			uint32_t id;
			mutable bool needs_linking;
		};
	}

	namespace detail::convert {
		GLenum to_gl(engine::gl::shader::type v);
		engine::gl::shader::type to_shader_type(GLenum v);
	}
}

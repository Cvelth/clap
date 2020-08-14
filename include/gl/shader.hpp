#pragma once
#include <cstdint>
#include <map>
#include <string>

typedef unsigned int GLenum;

namespace clap::gl::detail {
	class state;
}
namespace clap::gl::vertex_array::detail {
	class indexed;
}
namespace clap::gl::shader {
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

	namespace detail {
		namespace variable_type_t {
			enum class storage {
				attribute, uniform
			};
			enum class structure {
				data, sampler, shadow_sampler, image
			};
			enum class datatype {
				_float, _double, _int, _unsigned, _bool
			};
			enum class specific {
				none, cube, rect, array, buffer, multisample, multisample_array,
			};
			struct dimentions {
				size_t x, y;
			};
		}

		struct variable_type {
			variable_type_t::structure const structure;
			variable_type_t::datatype const datatype;
			variable_type_t::specific const specific;
			variable_type_t::dimentions const dimentions;

			explicit variable_type(variable_type_t::structure const structure,
								   variable_type_t::datatype const datatype,
								   variable_type_t::specific const specific,
								   variable_type_t::dimentions const dimentions) :
				structure(structure), datatype(datatype),
				specific(specific), dimentions(dimentions) {}
		};

		class variable {
			friend program;
			friend vertex_array::detail::indexed;
		public:

			std::string const name;

			bool operator<(variable const &other) const { return location < other.location; }
			size_t size() const;

		private:
			explicit variable(std::string const &name, uint32_t const &location,
							  variable_type_t::storage const storage, variable_type const &type);

		private:
			uint32_t const location;
			variable_type_t::storage const storage;
			variable_type const type;
		};
	}

	class variables : private std::map<std::string, detail::variable> {
		friend program;
	public:
		detail::variable const &operator[](std::string name);

		using std::map<std::string, detail::variable>::begin;
		using std::map<std::string, detail::variable>::end;
		using std::map<std::string, detail::variable>::rbegin;
		using std::map<std::string, detail::variable>::rend;

		using std::map<std::string, detail::variable>::cbegin;
		using std::map<std::string, detail::variable>::cend;
		using std::map<std::string, detail::variable>::crbegin;
		using std::map<std::string, detail::variable>::crend;
	private:
		variables() = default;
		variables(variables const &) = default;
		variables(variables &&) noexcept = default;
	};

	class program {
		friend gl::detail::state;
	public:
		program();
		~program();

		template <typename ...Ts>
		program(Ts &&...ts) : program() {
			link(ts...);
		}

		program(program const &other) = delete;
		program(program &&other) noexcept : program(other.id) {}

		void add(detail::object const &object);
		template <typename ...Ts>
		void add(detail::object const &object, Ts const &...ts) {
			add(std::move(object));
			add(std::move(ts)...);
		}

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

		variables getUniforms();
		variables getAttributes();
		variables getVariables();

	private:
		program(uint32_t id);

	private:
		uint32_t id;
		mutable bool needs_linking;
	};
}

namespace clap::gl::detail::convert {
	GLenum to_gl(shader::type v);
	shader::type to_shader_type(GLenum v);
	shader::type to_shader_type_from_string(std::string const &v);

	GLenum to_gl(shader::detail::variable_type const &type);
	shader::detail::variable_type to_variable_type(GLenum v);

	GLenum to_gl(shader::detail::variable_type_t::datatype datatype);
	size_t to_size(shader::detail::variable_type_t::datatype datatype);
}

#include <ostream>
std::ostream &operator<<(std::ostream &stream, clap::gl::shader::type target);
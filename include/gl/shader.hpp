#pragma once
#include <filesystem>
#include <memory>
#include <optional>
#include <ostream>
#include <string_view>
#include <unordered_map>
#include <variant>

#include "essential/guard.hpp"
#include "essential/stack.hpp"

#include "gl/detail/context.hpp"
#include "gl/interface.hpp"

typedef unsigned int GLenum;
namespace clap::gl::vertex {
	class array;
}

namespace clap::gl::shader {
	namespace detail {
		class object;
		class linker;
	}
	class program;
	namespace variable {
		class attribute;
		class uniform;
	}
}
std::ostream &operator<<(std::ostream &stream, clap::gl::shader::variable::attribute const &variable);
std::ostream &operator<<(std::ostream &stream, clap::gl::shader::variable::uniform const &variable);

namespace clap::gl::shader {
	enum class type {
		fragment, vertex, geometry, compute, tesselation_control, tesselation_evaluation
	};
	std::optional<detail::object> from_source(type type, std::string_view source);
	std::optional<detail::object> from_file(type type, std::filesystem::path path);

	namespace detail {
		class object : public gl::detail::object_interface {
			friend std::optional<object> shader::from_source(type type, std::string_view source);
			friend linker;
		public:
			~object();

			object(object const &another) = delete;
			object(object &&another) noexcept : id(another.id) { another.id = 0; }

			friend std::ostream &operator<<(std::ostream &stream, object const &object) {
				return stream << "shader object (with id = " << object.id << ")";
			}
		private:
			object(type type);
		private:
			unsigned id;
		};

		class linker : public gl::detail::object_interface {
		public:
			linker();

			template <typename ...Ts>
			linker(Ts &&...ts) : linker() {
				add(ts...);
			}
			~linker();

			linker(linker const &another) = delete;
			linker(linker &&another) noexcept : id(another.id) { another.id = 0; }

			void add(detail::object const &object);
			template <typename ...Ts>
			void add(detail::object const &object, Ts const &...ts) {
				add(object);
				add(ts...);
			}

			void add(std::optional<detail::object> const &object) {
				if (object) add(*object);
			}
			template <typename ...Ts>
			void add(std::optional<detail::object> const &object, Ts const &...ts) {
				add(object);
				add(ts...);
			}

			void add(std::shared_ptr<detail::object> object) {
				if (object) add(*object);
			}
			template <typename ...Ts>
			void add(std::shared_ptr<detail::object> object, Ts ...ts) {
				add(object);
				add(ts...);
			}

			operator program();

			friend std::ostream &operator<<(std::ostream &stream, linker const &object) {
				return stream << "shader program (unlinked, with id = " << object.id << ")";
			}
		private:
			unsigned id;
		};
	}

	namespace variable {
		namespace type {
			namespace detail {
				template<typename T, size_t _size> struct vec {
					using type = T;
					//constexpr size_t size() { return _size; }
				};
				template<typename T, size_t _width, size_t _height> struct mat {
					using type = T;
					//constexpr size_t width() { return _width; }
					//constexpr size_t height() { return _height; }
				};

				template<typename T> using vec2 = vec<T, 2>;
				template<typename T> using vec3 = vec<T, 3>;
				template<typename T> using vec4 = vec<T, 4>;

				template<typename T> using mat2 = mat<T, 2, 2>;
				template<typename T> using mat3 = mat<T, 3, 3>;
				template<typename T> using mat4 = mat<T, 4, 4>;

				template<typename T> using mat2x3 = mat<T, 2, 3>;
				template<typename T> using mat2x4 = mat<T, 2, 4>;
				template<typename T> using mat3x2 = mat<T, 3, 2>;
				template<typename T> using mat3x4 = mat<T, 3, 4>;
				template<typename T> using mat4x2 = mat<T, 4, 2>;
				template<typename T> using mat4x3 = mat<T, 4, 3>;

				namespace helper {
					struct cube {};
					struct array {};
					struct multisample {};
					struct multisample_array {};
					struct buffer {};
					struct rect {};
				}
				template<typename T, size_t _dimentions, typename D = void, bool is_shadow = false>
				struct sampler {
					using type = T;
					using detail = D;
					//constexpr size_t dimentions() { return _dimentions; }
				};
				template<typename T, size_t _dimentions, typename D = void>
				struct image {
					using type = T;
					using detail = D;
					//constexpr size_t dimentions() { return _dimentions; }
				};


				template<typename T> using sampler_1d = sampler<T, 1>;
				template<typename T> using sampler_2d = sampler<T, 2>;
				template<typename T> using sampler_3d = sampler<T, 3>;
				template<typename T> using sampler_cube = sampler<T, 0, helper::cube>;
				template<typename T> using sampler_cube_shadow = sampler<T, 0, helper::cube, true>;
				template<typename T> using sampler_1d_shadow = sampler<T, 1, void, true>;
				template<typename T> using sampler_2d_shadow = sampler<T, 2, void, true>;
				template<typename T> using sampler_1d_array = sampler<T, 1, helper::array>;
				template<typename T> using sampler_2d_array = sampler<T, 2, helper::array>;
				template<typename T> using sampler_1d_array_shadow = sampler<T, 1, helper::array, true>;
				template<typename T> using sampler_2d_array_shadow = sampler<T, 2, helper::array, true>;
				template<typename T> using sampler_2d_multisample = sampler<T, 2, helper::multisample>;
				template<typename T> using sampler_2d_multisample_array = sampler<T, 2, helper::multisample_array>;
				template<typename T> using sampler_buffer = sampler<T, 0, helper::buffer>;
				template<typename T> using sampler_rect = sampler<T, 2, helper::rect>;
				template<typename T> using sampler_rect_shadow = sampler<T, 2, helper::rect, true>;

				template<typename T> using image_1d = image<T, 1>;
				template<typename T> using image_2d = image<T, 2>;
				template<typename T> using image_3d = image<T, 3>;
				template<typename T> using image_rect = image<T, 2, helper::rect>;
				template<typename T> using image_cube = image<T, 0, helper::cube>;
				template<typename T> using image_buffer = image<T, 0, helper::buffer>;
				template<typename T> using image_1d_array = image<T, 1, helper::array>;
				template<typename T> using image_2d_array = image<T, 2, helper::array>;
				template<typename T> using image_2d_multisample = image<T, 2, helper::multisample>;
				template<typename T> using image_2d_multisample_array = image<T, 2, helper::multisample_array>;

				template<typename T> struct atomic_counter {};
			}
			using attribute = std::variant<
				float, detail::vec2<float>, detail::vec3<float>, detail::vec4<float>,
				detail::mat2<float>, detail::mat3<float>, detail::mat4<float>,
				detail::mat2x3<float>, detail::mat2x4<float>, detail::mat3x2<float>,
				detail::mat3x4<float>, detail::mat4x2<float>, detail::mat4x3<float>,

				int, detail::vec2<int>, detail::vec3<int>, detail::vec4<int>,
				unsigned, detail::vec2<unsigned>, detail::vec3<unsigned>, detail::vec4<unsigned>,

				double, detail::vec2<double>, detail::vec3<double>, detail::vec4<double>,
				detail::mat2<double>, detail::mat3<double>, detail::mat4<double>,
				detail::mat2x3<double>, detail::mat2x4<double>, detail::mat3x2<double>,
				detail::mat3x4<double>, detail::mat4x2<double>, detail::mat4x3<double>
			>;
			using uniform = std::variant<
				float, detail::vec2<float>, detail::vec3<float>, detail::vec4<float>,
				detail::mat2<float>, detail::mat3<float>, detail::mat4<float>,
				detail::mat2x3<float>, detail::mat2x4<float>, detail::mat3x2<float>,
				detail::mat3x4<float>, detail::mat4x2<float>, detail::mat4x3<float>,

				int, detail::vec2<int>, detail::vec3<int>, detail::vec4<int>,
				unsigned, detail::vec2<unsigned>, detail::vec3<unsigned>, detail::vec4<unsigned>,
				bool, detail::vec2<bool>, detail::vec3<bool>, detail::vec4<bool>,

				double, detail::vec2<double>, detail::vec3<double>, detail::vec4<double>,
				detail::mat2<double>, detail::mat3<double>, detail::mat4<double>,
				detail::mat2x3<double>, detail::mat2x4<double>, detail::mat3x2<double>,
				detail::mat3x4<double>, detail::mat4x2<double>, detail::mat4x3<double>,

				detail::sampler_1d<float>, detail::sampler_2d<float>, detail::sampler_3d<float>,
				detail::sampler_cube<float>, detail::sampler_cube_shadow<float>,
				detail::sampler_1d_shadow<float>, detail::sampler_2d_shadow<float>,
				detail::sampler_1d_array<float>, detail::sampler_2d_array<float>,
				detail::sampler_1d_array_shadow<float>, detail::sampler_2d_array_shadow<float>,
				detail::sampler_2d_multisample<float>, detail::sampler_2d_multisample_array<float>,
				detail::sampler_buffer<float>,
				detail::sampler_rect<float>, detail::sampler_rect_shadow<float>,

				detail::sampler_1d<int>, detail::sampler_2d<int>, detail::sampler_3d<int>,
				detail::sampler_cube<int>,
				detail::sampler_1d_array<int>, detail::sampler_2d_array<int>,
				detail::sampler_2d_multisample<int>, detail::sampler_2d_multisample_array<int>,
				detail::sampler_buffer<int>, detail::sampler_rect<int>,

				detail::sampler_1d<unsigned>, detail::sampler_2d<unsigned>, detail::sampler_3d<unsigned>,
				detail::sampler_cube<unsigned>,
				detail::sampler_1d_array<unsigned>, detail::sampler_2d_array<unsigned>,
				detail::sampler_2d_multisample<unsigned>, detail::sampler_2d_multisample_array<unsigned>,
				detail::sampler_buffer<unsigned>, detail::sampler_rect<unsigned>,

				detail::image_1d<float>, detail::image_2d<float>, detail::image_3d<float>,
				detail::image_rect<float>, detail::image_cube<float>, detail::image_buffer<float>,
				detail::image_1d_array<float>, detail::image_2d_array<float>,
				detail::image_2d_multisample<float>, detail::image_2d_multisample_array<float>,

				detail::image_1d<int>, detail::image_2d<int>, detail::image_3d<int>,
				detail::image_rect<int>, detail::image_cube<int>, detail::image_buffer<int>,
				detail::image_1d_array<int>, detail::image_2d_array<int>,
				detail::image_2d_multisample<int>, detail::image_2d_multisample_array<int>,

				detail::image_1d<unsigned>, detail::image_2d<unsigned>, detail::image_3d<unsigned>,
				detail::image_rect<unsigned>, detail::image_cube<unsigned>, detail::image_buffer<unsigned>,
				detail::image_1d_array<unsigned>, detail::image_2d_array<unsigned>,
				detail::image_2d_multisample<unsigned>, detail::image_2d_multisample_array<unsigned>,

				detail::atomic_counter<unsigned>
			> ;
		}

		class uniform;
		namespace detail {
			template <typename variant_t>
			class variable_interface : public variant_t {
			protected:
				template<typename variable_t>
				variable_interface(variable_t &&type, size_t size, int location,
								   std::string const &name,
								   program const &program_ref) noexcept
					: variant_t(std::move(type)), size(size), location(location),
					name(name), program_ref(program_ref) {}
			public:
				const std::string name;
				program const &program_ref;
			protected:
				const int location;
				const size_t size;
			};

			template <size_t vector_size, typename param_t>
			void set_vectors(int location, unsigned count, param_t const *value);
			template <size_t width, size_t height, typename param_t>
			void set_matrices(int location, unsigned count, param_t const *value);
			template <typename param_t>
			void set_values(int location, unsigned count, param_t const *value) {
				set_vectors<1>(location, count, value);
			}
			void variable_set_without_convertion_message(uniform const &variable);
			void variable_set_with_convertion_message(uniform const &variable);
			void cannot_convert_variable_type_error(uniform const &variable);
			void variable_size_error(uniform const &variable, size_t expected, size_t received);

		#pragma warning(push)
		#pragma warning(disable : 4100)
			template <typename param_t>
			struct set_uniform_visitor {
				uniform const &variable;
				param_t const *ptr;
				size_t count;

				set_uniform_visitor(uniform const &variable, param_t const *ptr, size_t count)
					: variable(variable), ptr(ptr), count(count) {}

				template<typename variable_t, size_t dimention_count, typename detail_t, bool is_shadow>
				void operator()(type::detail::sampler<variable_t, dimention_count, detail_t, is_shadow> const &input) {
					impl<int, 1>(&set_values<int>);
				}
				template<typename variable_t, size_t dimention_count, typename detail_t>
				void operator()(type::detail::image<variable_t, dimention_count, detail_t> const &input) {
					impl<int, 1>(&set_values<int>);
				}
				void operator()(type::detail::atomic_counter<unsigned> const &input) {
					impl<unsigned, 1>(&set_values<unsigned>);
				}
				template<typename variable_t, size_t vector_size>
				void operator()(type::detail::vec<variable_t, vector_size> const &input) {
					impl<variable_t, vector_size>(&set_vectors<vector_size, variable_t>);
				}
				template<typename variable_t, size_t width, size_t height>
				void operator()(type::detail::mat<variable_t, width, height> const &input) {
					impl<variable_t, width *height>(&set_matrices<width, height, variable_t>);
				}
				template <typename variable_t>
				void operator()(variable_t const &input) {
					impl<variable_t, 1>(&set_values<variable_t>);
				}

			private:
				template<typename destination_t, size_t datatype_size>
				void impl(void (*callable)(int, unsigned, destination_t const *));
			};

			template <class tested_t>
			struct is_initializer_list_or_vector { static constexpr bool value = false; };
			template <class tested_t>
			struct is_initializer_list_or_vector<std::vector<tested_t>> {
				static constexpr bool value = true;
			};
			template <class tested_t>
			struct is_initializer_list_or_vector<std::initializer_list<tested_t>> {
				static constexpr bool value = true;
			};
		}
	#pragma warning(pop)

		class attribute : public detail::variable_interface<type::attribute> {
			friend program;
			friend clap::gl::vertex::array;
		public:
			using detail::variable_interface<type::attribute>::variable_interface;
			friend std::ostream &::operator<<(std::ostream &stream, attribute const &variable);
		};

		class uniform : public detail::variable_interface<type::uniform> {
			friend program;
			template <typename param_t> friend struct detail::set_uniform_visitor;
		public:
			using detail::variable_interface<type::uniform>::variable_interface;
			friend std::ostream &::operator<<(std::ostream &stream, uniform const &variable);

			template<typename param_t>
			uniform const &set(std::vector<param_t> const &vector) const {
				return set(vector.data(), vector.size());
			}
			template<typename param_t>
			uniform const &set(std::initializer_list<param_t> const &list) const {
				return set(list.begin(), list.size());
			}
			template<typename param_t, typename ...param_Ts>
			std::enable_if_t<!detail::is_initializer_list_or_vector<param_t>::value, uniform> const &set(param_t const &first, param_Ts const &...others) const {
				return set(std::initializer_list{ first, others... });
			}
			template<typename param_t, size_t N> inline uniform const &set(param_t const(&values)[N]) const {
				return set(values, N);
			}

			template <typename param_t>
			uniform const &operator=(std::initializer_list<param_t> const &anything) const {
				return set(anything);
			}
			template <typename param_t>
			uniform const &operator=(param_t const &anything) const {
				return set(anything);
			}

		protected:
			template <typename param_t>
			uniform const &set(param_t const *ptr, size_t count) const;
		};
	}
	namespace detail {
		template <typename T>
		T const &unknown_variable_error(std::string_view const &name);

		template <typename T>
		class storage : protected std::unordered_map<std::string, T> {
			friend program;
		public:
			T const &operator[](std::string const &name) const {
				auto found = this->find(name);
				if (found != this->end())
					return found->second;
				else
					return unknown_variable_error<T>(name);
			}

			using std::unordered_map<std::string, T>::cbegin;
			using std::unordered_map<std::string, T>::cend;

			storage(storage const &) = delete;
			storage(storage &&) noexcept = delete;
		private:
			storage() {}
		};

		struct lock_program_callable {
			program const &program_ref;
			essential::stack<clap::gl::shader::program const *>::iterator operator()();
		};
		struct unlock_program_callable {
			program const &program_ref;
			void operator()(essential::stack<clap::gl::shader::program const *>::iterator);
		};

		class program_guard : essential::simple_guard<lock_program_callable, unlock_program_callable> {
		public:
			program_guard(program const &program_ref) :
				essential::simple_guard<lock_program_callable, unlock_program_callable>(
					detail::lock_program_callable{ program_ref },
					detail::unlock_program_callable{ program_ref }) {}
		};
	}

	class program : public gl::detail::object_interface {
		friend detail::linker;
		friend detail::lock_program_callable;
		friend detail::unlock_program_callable;
	public:
		~program();

		program(program const &another) = delete;
		program(program &&another) noexcept : id(another.id) { another.id = 0; }

		inline detail::program_guard use() const {
			return detail::program_guard(*this);
		}

		friend inline std::ostream &operator<<(std::ostream &stream, program const &object) {
			return stream << "shader program (linked, with id = " << object.id << ")";
		}
	private:
		program(unsigned id);

	public:
		detail::storage<variable::uniform> uniform;
		detail::storage<variable::attribute> attribute;
	private:
		unsigned id;
	};

	template <typename ...Ts>
	inline program link(Ts const &...ts) {
		return detail::linker{ ts... };
	}

	template <typename ...Ts>
	inline std::unique_ptr<program> link_unique(Ts const &...ts) {
		return std::make_unique<program>(std::move(detail::linker{ ts... }));
	}

	template <typename ...Ts>
	inline std::shared_ptr<program> link_shared(Ts const &...ts) {
		return std::make_shared<program>(std::move(detail::linker{ ts... }));
	}
}

namespace clap::gl::detail::convert {
	GLenum to_gl(shader::type v);
	shader::type to_shader_type(GLenum v);
	shader::type to_shader_type_from_string(std::string const &v);

	GLenum to_gl(shader::variable::type::attribute const &v);
	GLenum to_gl_type(shader::variable::type::attribute const &v);
	shader::variable::type::attribute to_attribute(GLenum v);
	GLenum to_gl(shader::variable::type::uniform const &v);
	shader::variable::type::uniform to_uniform(GLenum v);
}
std::ostream &operator<<(std::ostream &stream, clap::gl::shader::type target);

template<typename param_t>
inline clap::gl::shader::variable::uniform const &clap::gl::shader::variable::uniform::set(param_t const *ptr, size_t count) const {
	if (auto context = program_ref.access_context(); context)
		std::visit(detail::set_uniform_visitor(*this, ptr, count), *this);
	return *this;
}
template<typename param_t>
template<typename destination_t, size_t datatype_size>
inline void clap::gl::shader::variable::detail::set_uniform_visitor<param_t>::impl(void(*callable)(int, unsigned, destination_t const *)) {
	if (count == datatype_size * variable.size)
		if constexpr (std::is_same<param_t, destination_t>::value) {
			auto guard = variable.program_ref.use();
			callable(variable.location, unsigned(variable.size), ptr);
			variable_set_without_convertion_message(variable);
		} else if constexpr (std::is_convertible<param_t, destination_t>::value) {
			auto *tmp = new destination_t[count];
			std::transform(ptr, ptr + count, tmp, [](param_t const &input)
						   -> destination_t { return static_cast<destination_t>(input); });
			auto guard = variable.program_ref.use();
			callable(variable.location, unsigned(variable.size), tmp);
			variable_set_with_convertion_message(variable);
			delete[] tmp;
		} else
			cannot_convert_variable_type_error(variable);
	else
		variable_size_error(variable, datatype_size * variable.size, count);
}
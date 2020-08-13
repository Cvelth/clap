#pragma once
#include <map>
#include <string>

namespace clap::gl::shader::detail {
	class object;
}

namespace clap::resource {
	void load();
	void clear();
}

namespace clap::resource::detail {
	class shader_object_storage_t;
	class shaders_t;

	void load_shader(shaders_t &storage, std::string const &name, clap::gl::shader::detail::object *object);
	void load_texture(std::string const &filename);

	using shader_storage_t = std::map<std::string, clap::gl::shader::detail::object *>;
	class shaders_t : private shader_storage_t {
		friend shader_object_storage_t;
		friend void load_shader(shaders_t &storage, std::string const &name, clap::gl::shader::detail::object *object);
		friend void clap::resource::clear();
	public:
		clap::gl::shader::detail::object &operator[](std::string const &identificator);

		using shader_storage_t::begin;
		using shader_storage_t::end;
		using shader_storage_t::rbegin;
		using shader_storage_t::rend;

		using shader_storage_t::cbegin;
		using shader_storage_t::cend;
		using shader_storage_t::crbegin;
		using shader_storage_t::crend;

		inline ~shaders_t() {
			clear();
		}

	protected:
		void clear();

	private:
		shaders_t() = default;
		shaders_t(shaders_t const &) = delete;
		shaders_t(shaders_t &&) = delete;
	};

	class shader_object_storage_t {
	public:
		inline static detail::shaders_t fragment;
		inline static detail::shaders_t vertex;
		inline static detail::shaders_t geometry;
		inline static detail::shaders_t compute;
		inline static detail::shaders_t tesselation_control;
		inline static detail::shaders_t tesselation_evaluation;
	};
}

namespace clap::resource {
	using shader = detail::shader_object_storage_t;
	// Other resource types (e.g. texture)
}
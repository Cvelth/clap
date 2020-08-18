#pragma once
#include <map>
#include <string>

namespace clap::gl::shader::detail {
	class object;
}
namespace clap::gl::texture {
	class _1d;
	class _2d;
	class _3d;
	class _1d_array;
	class _2d_array;
	class rectangle;
	class multisample;
	class multisample_array;
}

namespace clap::resource {
	void load();
	void clear();
}

namespace clap::resource::detail {
	template<typename T>
	using underlying_container_t = std::map<std::string, T*>;

	class shaders_t;
	class shader_object_storage_t;

	template<typename texture_type_t> class textures_t;
	class texture_object_storage_t;

	void load_shader(shaders_t &storage, std::string const &name, clap::gl::shader::detail::object *object);
	void load_texture(std::string const &filename, std::string const &texture_name);

	void unloaded_resource_check();
	void non_existent_file_error(std::string const &identificator);
	
	template <typename T>
	void call_destructor(T *ptr);

	using underlying_shader_container_t = underlying_container_t<clap::gl::shader::detail::object>;
	class shaders_t : private underlying_shader_container_t {
		friend shader_object_storage_t;
		friend void load_shader(shaders_t &storage, std::string const &name, clap::gl::shader::detail::object *object);
		friend void clap::resource::clear();
	public:
		clap::gl::shader::detail::object &operator[](std::string const &identificator) {
			unloaded_resource_check();
			if (auto found = underlying_shader_container_t::find(identificator); found != end())
				return *found->second;
			else {
				non_existent_file_error(identificator);
				throw std::exception{};
			}
		}

		using underlying_shader_container_t::begin;
		using underlying_shader_container_t::end;
		using underlying_shader_container_t::rbegin;
		using underlying_shader_container_t::rend;

		using underlying_shader_container_t::cbegin;
		using underlying_shader_container_t::cend;
		using underlying_shader_container_t::crbegin;
		using underlying_shader_container_t::crend;

		inline ~shaders_t() { clear(); }

	protected:
		void clear() {
			for (auto pair : *this)
				if (pair.second)
					call_destructor(pair.second);
			this->underlying_shader_container_t::clear();
		}

	private:
		shaders_t() = default;
		shaders_t(shaders_t const &) = delete;
		shaders_t(shaders_t &&) = delete;
	};

	template<typename texture_type_t>
	class textures_t : private underlying_container_t<texture_type_t> {
		friend texture_object_storage_t;
		friend void load_texture(std::string const &filename, std::string const &texture_name);
		friend void clap::resource::clear();
	public:
		texture_type_t &operator[](std::string const &identificator) {
			unloaded_resource_check();
			if (auto found = underlying_container_t<texture_type_t>::find(identificator); found != end())
				return *found->second;
			else {
				non_existent_file_error(identificator);
				throw std::exception{};
			}
		}

		using underlying_container_t<texture_type_t>::begin;
		using underlying_container_t<texture_type_t>::end;
		using underlying_container_t<texture_type_t>::rbegin;
		using underlying_container_t<texture_type_t>::rend;

		using underlying_container_t<texture_type_t>::cbegin;
		using underlying_container_t<texture_type_t>::cend;
		using underlying_container_t<texture_type_t>::crbegin;
		using underlying_container_t<texture_type_t>::crend;

		inline ~textures_t() { clear(); }

	protected:
		void clear() {
			for (auto pair : *this)
				if (pair.second)
					call_destructor(pair.second);
			this->underlying_container_t<texture_type_t>::clear();
		}

	private:
		textures_t() = default;
		textures_t(shaders_t const &) = delete;
		textures_t(shaders_t &&) = delete;
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

	class texture_object_storage_t {
	public:
		inline static detail::textures_t<gl::texture::_1d> _1d;
		inline static detail::textures_t<gl::texture::_2d> _2d;
		inline static detail::textures_t<gl::texture::_3d> _3d;
		inline static detail::textures_t<gl::texture::_1d_array> _1d_array;
		inline static detail::textures_t<gl::texture::_2d_array> _2d_array;
		inline static detail::textures_t<gl::texture::rectangle> rectangle;
		inline static detail::textures_t<gl::texture::multisample> multisample;
		inline static detail::textures_t<gl::texture::multisample_array> multisample_array;
	};
}

namespace clap::resource {
	using shader = detail::shader_object_storage_t;
	using texture = detail::texture_object_storage_t;

	// Other resource types are to be added here
}
#pragma once
#include <map>
#include <string>

namespace nowide {
	std::string narrow(const std::wstring &s);
}

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
namespace clap::render {
	class font;
}

namespace clap::resource {
	void load();
	void clear();
}

namespace clap::resource::detail {
	class shader_object_storage_t;
	class texture_object_storage_t;
	class font_object_storage_t;

	template <typename contained_typename>
	class resource_container_t;

	template<typename storage_t, typename object_t>
	void load_resource(std::basic_string<char8_t> const &access_name, storage_t &storage, object_t *object) {
		storage.insert(std::pair(access_name, object));
	}

	void unloaded_resource_check();
	void non_existent_file_error(std::basic_string<char8_t> const &identificator);
	
	template <typename T>
	void call_destructor(T *ptr);

	template<typename T>
	using underlying_container_t = std::map<std::basic_string<char8_t>, T *>;
	template <typename contained_typename>
	class resource_container_t : private underlying_container_t<contained_typename> {
		friend void clap::resource::clear();

		template<typename storage_t, typename object_t>
		friend void load_resource(std::basic_string<char8_t> const &access_name, storage_t &storage, object_t *object);

		friend shader_object_storage_t;
		friend texture_object_storage_t;
		friend font_object_storage_t;

	public:
		contained_typename &operator[](std::basic_string<char8_t> const &identificator) {
			unloaded_resource_check();
			if (auto found = underlying_container_t<contained_typename>::find(identificator); found != end())
				return *found->second;
			else {
				non_existent_file_error(identificator);
				throw std::exception{};
			}
		}
		contained_typename &operator[](std::basic_string<char> const &identificator) {
			return operator[](std::basic_string<char8_t>((char8_t const*) identificator.c_str()));
		}
		contained_typename &operator[](std::basic_string<wchar_t> const &identificator) {
			return operator[](nowide::narrow(identificator));
		}

		using underlying_container_t<contained_typename>::begin;
		using underlying_container_t<contained_typename>::end;
		using underlying_container_t<contained_typename>::rbegin;
		using underlying_container_t<contained_typename>::rend;

		using underlying_container_t<contained_typename>::cbegin;
		using underlying_container_t<contained_typename>::cend;
		using underlying_container_t<contained_typename>::crbegin;
		using underlying_container_t<contained_typename>::crend;

		inline ~resource_container_t() { clear(); }

	protected:
		void clear() {
			for (auto pair : *this)
				if (pair.second)
					call_destructor(pair.second);
			this->underlying_container_t<contained_typename>::clear();
		}

	private:
		resource_container_t() = default;
		resource_container_t(resource_container_t const &) = delete;
		resource_container_t(resource_container_t &&) = delete;
	};

	class shader_object_storage_t {
	public:
		inline static detail::resource_container_t<clap::gl::shader::detail::object> fragment;
		inline static detail::resource_container_t<clap::gl::shader::detail::object> vertex;
		inline static detail::resource_container_t<clap::gl::shader::detail::object> geometry;
		inline static detail::resource_container_t<clap::gl::shader::detail::object> compute;
		inline static detail::resource_container_t<clap::gl::shader::detail::object> tesselation_control;
		inline static detail::resource_container_t<clap::gl::shader::detail::object> tesselation_evaluation;
	};

	class texture_object_storage_t {
	public:
		inline static detail::resource_container_t<gl::texture::_1d> _1d;
		inline static detail::resource_container_t<gl::texture::_2d> _2d;
		inline static detail::resource_container_t<gl::texture::_3d> _3d;
		inline static detail::resource_container_t<gl::texture::_1d_array> _1d_array;
		inline static detail::resource_container_t<gl::texture::_2d_array> _2d_array;
		inline static detail::resource_container_t<gl::texture::rectangle> rectangle;
		inline static detail::resource_container_t<gl::texture::multisample> multisample;
		inline static detail::resource_container_t<gl::texture::multisample_array> multisample_array;
	};
	class font_object_storage_t {
	public:
		inline static detail::resource_container_t<clap::render::font> font;
	};
}

namespace clap::resource {
	using shader = detail::shader_object_storage_t;
	using texture = detail::texture_object_storage_t;
	inline static auto &font = detail::font_object_storage_t::font;

	// Other resource types are to be added here
}
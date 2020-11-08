#include "gl/texture.hpp"
#include "essential/log.hpp"

#include "gl/detail/context.hpp"

#include "glad/glad.h"

template <clap::gl::texture::detail::target texture_type>
clap::gl::texture::detail::interface<texture_type>::~interface() {
	if (auto context = access_context(); context) {
		for (auto &texture_stack : context->texture_stack) {
			auto iterator = texture_stack.begin();
			auto pred = [this](auto &interface) { return std::get<detail::interface<texture_type> const *>(interface) == this; };
			while ((iterator = std::find_if(iterator, texture_stack.end(), pred)) != texture_stack.end()) {
				log::warning::major << "The destructor of a " << *this << " was called while it's still in use.";
				*iterator = (detail::interface<texture_type> const *) nullptr;
			}
		}
	}

	if (id)
		glDeleteTextures(1, &id);
	log::message::minor << "A " << *this << " was destroyed.";
}


template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_depth_stencil_texture_mode(
	depth_stencil_texture_mode mode
) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexParameteri(clap::gl::detail::convert::to_gl(texture_type),
						GL_DEPTH_STENCIL_TEXTURE_MODE,
						gl::detail::convert::to_gl(mode));
	}
}
template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_base_level(int level) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexParameteri(clap::gl::detail::convert::to_gl(texture_type),
						GL_TEXTURE_BASE_LEVEL, level);
	}
}
template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_border_color(
	float r, float g, float b, float a
) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();

		GLfloat temp[] = { r, g, b, a };
		glTexParameterfv(clap::gl::detail::convert::to_gl(texture_type),
						 GL_TEXTURE_BORDER_COLOR, temp);
	}
}
template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_lod_bias(float bias) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexParameterf(clap::gl::detail::convert::to_gl(texture_type),
						GL_TEXTURE_LOD_BIAS, bias);
	}
}
template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_min_filter(min_filter filter) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexParameteri(clap::gl::detail::convert::to_gl(texture_type),
						GL_TEXTURE_MIN_FILTER, gl::detail::convert::to_gl(filter));
	}
}
template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_mag_filter(mag_filter filter) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexParameteri(clap::gl::detail::convert::to_gl(texture_type),
						GL_TEXTURE_MAG_FILTER, gl::detail::convert::to_gl(filter));
	}
}
template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_min_lod(float value) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexParameterf(clap::gl::detail::convert::to_gl(texture_type),
						GL_TEXTURE_MIN_LOD, value);
	}
}
template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_max_lod(float value) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexParameterf(clap::gl::detail::convert::to_gl(texture_type),
						GL_TEXTURE_MAX_LOD, value);
	}
}
template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_max_level(int level) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexParameteri(clap::gl::detail::convert::to_gl(texture_type),
						GL_TEXTURE_MAX_LEVEL, level);
	}
}
template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_texture_wrap_s(wrap wrap) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexParameteri(clap::gl::detail::convert::to_gl(texture_type),
						GL_TEXTURE_WRAP_S, gl::detail::convert::to_gl(wrap));
	}
}
template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_texture_wrap_t(wrap wrap) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexParameteri(clap::gl::detail::convert::to_gl(texture_type),
						GL_TEXTURE_WRAP_T, gl::detail::convert::to_gl(wrap));
	}
}
template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::interface<texture_type>::set_texture_wrap_r(wrap wrap) {
	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexParameteri(clap::gl::detail::convert::to_gl(texture_type),
						GL_TEXTURE_WRAP_R, gl::detail::convert::to_gl(wrap));
	}
}

template <clap::gl::texture::detail::target texture_type>
size_t clap::gl::texture::detail::interface<texture_type>::maximum_size() {
	GLint out = 0;
	if (auto context = access_context_s(); context)
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &out);
	return size_t(out);
}
template <clap::gl::texture::detail::target texture_type>
size_t clap::gl::texture::detail::interface<texture_type>::maximum_layer_count() {
	GLint out = 0;
	if (auto context = access_context_s(); context)
		glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, &out);
	return size_t(out);
}
template <clap::gl::texture::detail::target texture_type>
size_t clap::gl::texture::detail::interface<texture_type>::maximum_size_3d() {
	GLint out = 0;
	if (auto context = access_context_s(); context)
		glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &out);
	return size_t(out);
}


template <>
template <>
clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::interface(
	void *data, size_t width, bool generate_mipmap,
	texture::internal_format internal_format,
	external_format external_format,
	external_type external_type)
	: internal_format(internal_format),
	has_width<target::_1d, needs_width<target::_1d>>{width},
	id(0)
{
	if (auto context = access_context(); context) {
		glGenTextures(1, &id);
		if (id == 0) {
			log::warning::critical << "Texture Object creation failed.";
			log::info::major << "Requested type: " << target::_1d << ".";
		} else {
			glTexImage1D(gl::detail::convert::to_gl(target::_2d), 0,
						 gl::detail::convert::to_gl(internal_format),
						 GLsizei(width), 0,
						 gl::detail::convert::to_gl(external_format),
						 gl::detail::convert::to_gl(external_type),
						 data);

			log::message::minor << "A " << *this << " was created.";
			log::info::major << "Dimentions are (" << width << ").";
			if (generate_mipmap) {
				glGenerateMipmap(gl::detail::convert::to_gl(target::_1d));
				log::info::minor << "Mipmap was generated.";
			}
		}
	}
}
template <>
template <>
clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::interface(
	void *data, size_t width, size_t height, bool generate_mipmap,
	texture::internal_format internal_format,
	external_format external_format,
	external_type external_type)
	: internal_format(internal_format),
	has_width<target::_2d, needs_width<target::_2d>>{width},
	has_height<target::_2d, needs_height<target::_2d>>{height},
	id(0)
{
	if (auto context = access_context(); context) {
		glGenTextures(1, &id);
		if (id == 0) {
			log::warning::critical << "Texture Object creation failed.";
			log::info::major << "Requested type: " << target::_2d << ".";
		} else {
			glTexImage2D(gl::detail::convert::to_gl(target::_2d), 0,
						 gl::detail::convert::to_gl(internal_format),
						 GLsizei(width), GLsizei(height), 0,
						 gl::detail::convert::to_gl(external_format),
						 gl::detail::convert::to_gl(external_type),
						 data);

			log::message::minor << "A " << *this << " was created.";
			log::info::major << "Dimentions are (" << width << ", " << height << ").";
			if (generate_mipmap) {
				glGenerateMipmap(gl::detail::convert::to_gl(target::_2d));
				log::info::minor << "Mipmap was generated.";
			}
		}
	}
}
template <>
template <>
clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::interface(
	void *data, size_t width, size_t height, size_t depth, bool generate_mipmap,
	texture::internal_format internal_format,
	external_format external_format,
	external_type external_type)
	: internal_format(internal_format),
	has_width<target::_3d, needs_width<target::_3d>>{width},
	has_height<target::_3d, needs_height<target::_3d>>{height},
	has_depth<target::_3d, needs_depth<target::_3d>>{depth},
	id(0)
{
	if (auto context = access_context(); context) {
		glGenTextures(1, &id);
		if (id == 0) {
			log::warning::critical << "Texture Object creation failed.";
			log::info::major << "Requested type: " << target::_3d << ".";
		} else {
			glTexImage3D(gl::detail::convert::to_gl(target::_3d), 0,
						 gl::detail::convert::to_gl(internal_format),
						 GLsizei(width), GLsizei(height), GLsizei(depth), 0,
						 gl::detail::convert::to_gl(external_format),
						 gl::detail::convert::to_gl(external_type),
						 data);

			log::message::minor << "A " << *this << " was created.";
			log::info::major << "Dimentions are (" << width << ", " << height << ", " << depth << ").";
			if (generate_mipmap) {
				glGenerateMipmap(gl::detail::convert::to_gl(target::_3d));
				log::info::minor << "Mipmap was generated.";
			}
		}
	}
}
template <>
template <>
clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::interface(
	void *data, size_t width, size_t count, bool generate_mipmap,
	texture::internal_format internal_format,
	external_format external_format,
	external_type external_type)
	: internal_format(internal_format),
	has_width<target::_1d_array, needs_width<target::_1d_array>>{width},
	has_count<target::_1d_array, needs_count<target::_1d_array>>{count},
	id(0)
{
	if (auto context = access_context(); context) {
		glGenTextures(1, &id);
		if (id == 0) {
			log::warning::critical << "Texture Object creation failed.";
			log::info::major << "Requested type: " << target::_1d_array << ".";
		} else {
			glTexImage2D(gl::detail::convert::to_gl(target::_1d_array), 0,
						 gl::detail::convert::to_gl(internal_format),
						 GLsizei(width), GLsizei(count), 0,
						 gl::detail::convert::to_gl(external_format),
						 gl::detail::convert::to_gl(external_type),
						 data);

			log::message::minor << "A " << *this << " was created.";
			log::info::major << "Dimentions are (" << width << ") x " << count << ".";
			if (generate_mipmap) {
				glGenerateMipmap(gl::detail::convert::to_gl(target::_1d_array));
				log::info::minor << "Mipmap was generated.";
			}
		}
	}
}
template <>
template <>
clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::interface(
	void *data, size_t width, size_t height, size_t count, bool generate_mipmap,
	texture::internal_format internal_format,
	external_format external_format,
	external_type external_type)
	: internal_format(internal_format),
	has_width<target::_2d_array, needs_width<target::_2d_array>>{width},
	has_height<target::_2d_array, needs_height<target::_2d_array>>{height},
	has_count<target::_2d_array, needs_count<target::_2d_array>>{count},
	id(0)
{
	if (auto context = access_context(); context) {
		glGenTextures(1, &id);
		if (id == 0) {
			log::warning::critical << "Texture Object creation failed.";
			log::info::major << "Requested type: " << target::_2d_array << ".";
		} else {
			glTexImage3D(gl::detail::convert::to_gl(target::_2d_array), 0,
						 gl::detail::convert::to_gl(internal_format),
						 GLsizei(width), GLsizei(height), GLsizei(count), 0,
						 gl::detail::convert::to_gl(external_format),
						 gl::detail::convert::to_gl(external_type),
						 data);

			log::message::minor << "A " << *this << " was created.";
			log::info::major << "Dimentions are (" << width << ", " << height << ") x " << count << ".";
			if (generate_mipmap) {
				glGenerateMipmap(gl::detail::convert::to_gl(target::_2d_array));
				log::info::minor << "Mipmap was generated.";
			}
		}
	}
}
/*
template <>
template <>
clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::interface(
	size_t sample_count, size_t width, size_t height,
	bool are_samples_fixed, texture::internal_format internal_format)
	: internal_format(internal_format),
	has_width<target::multisample, needs_width<target::multisample>>{width},
	has_height<target::multisample, needs_height<target::multisample>>{height},
	has_sample_count<target::multisample, needs_sample_count<target::multisample>>{sample_count},
	id(0)
{
		if (auto context = access_context(); context) {
		glGenTextures(1, &id);
		if (id == 0) {
			log::warning::critical << "Texture Object creation failed.";
			log::info::major << "Requested type: " << target::multisample << ".";
		} else {
			glTexImage2DMultisample(gl::detail::convert::to_gl(target::multisample),
									GLsizei(sample_count),
									gl::detail::convert::to_gl(internal_format),
									GLsizei(width), GLsizei(height), are_samples_fixed);

			log::message::minor << "A " << *this << " was created.";
			log::info::major << "Dimentions are (" << width << ", " << height << ").";
			log::info::major << "Sample count: " << sample_count << ".";
			log::info::major << "Samples are" << (are_samples_fixed ? " " : " not ") << "fixed.";
		}
	}
}
template <>
template <>
clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::interface(
	size_t sample_count, size_t width, size_t height, size_t count,
	bool are_samples_fixed, texture::internal_format internal_format)
	: internal_format(internal_format),
	has_width<target::multisample_array, needs_width<target::multisample_array>>{width},
	has_height<target::multisample_array, needs_height<target::multisample_array>>{height},
	has_count<target::multisample_array, needs_count<target::multisample_array>>{count},
	has_sample_count<target::multisample_array, needs_sample_count<target::multisample_array>>{sample_count},
	id(0)
{
	if (auto context = access_context(); context) {
		glGenTextures(1, &id);
		if (id == 0) {
			log::warning::critical << "Texture Object creation failed.";
			log::info::major << "Requested type: " << target::multisample_array << ".";
		} else {
			glTexImage3DMultisample(gl::detail::convert::to_gl(target::multisample_array),
									GLsizei(sample_count),
									gl::detail::convert::to_gl(internal_format),
									GLsizei(width), GLsizei(height), GLsizei(count),
									are_samples_fixed);

			log::message::minor << "A " << *this << " was created.";
			log::info::major << "Dimentions are (" << width << ", " << height << ") x " << count << ".";
			log::info::major << "Sample count: " << sample_count << ".";
			log::info::major << "Samples are" << (are_samples_fixed ? " " : " not ") << "fixed.";
		}
	}
}*/

template <>
template <>
void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::data(
	void *data, size_t offset_x, size_t width, bool generate_mipmap,
	external_format external_format, external_type external_type, int level) {

	if (width + offset_x > this->width) {
		log::warning::critical << "Attempt to change data out of texture bounds";
		return;
	}

	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexSubImage1D(gl::detail::convert::to_gl(target::_1d), level,
						GLsizei(offset_x), GLsizei(width),
						gl::detail::convert::to_gl(external_format),
						gl::detail::convert::to_gl(external_type),
						data);
		log::message::minor << "A " << *this << " was modified.";

		if (generate_mipmap) {
			glGenerateMipmap(gl::detail::convert::to_gl(target::_1d));
			log::info::minor << "Mipmap was generated.";
		}
	}
}
template <>
template <>
void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::data(
	void *data, size_t offset_x, size_t offset_y,
	size_t width, size_t height, bool generate_mipmap,
	external_format external_format, external_type external_type, int level) {

	if (width + offset_x > this->width || height + offset_y > this->height) {
		log::warning::critical << "Attempt to change data out of texture bounds";
		return;
	}

	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexSubImage2D(gl::detail::convert::to_gl(target::_2d), level,
						GLsizei(offset_x), GLsizei(offset_y),
						GLsizei(width), GLsizei(height),
						gl::detail::convert::to_gl(external_format),
						gl::detail::convert::to_gl(external_type),
						data);
		log::message::minor << "A " << *this << " was modified.";

		if (generate_mipmap) {
			glGenerateMipmap(gl::detail::convert::to_gl(target::_2d));
			log::info::minor << "Mipmap was generated.";
		}
	}
}
template <>
template <>
void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::data(
	void *data, size_t offset_x, size_t offset_y, size_t offset_z,
	size_t width, size_t height, size_t depth, bool generate_mipmap,
	external_format external_format, external_type external_type, int level) {

	if (width + offset_x > this->width || height + offset_y > this->height || depth + offset_z > this->depth) {
		log::warning::critical << "Attempt to change data out of texture bounds";
		return;
	}

	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexSubImage3D(gl::detail::convert::to_gl(target::_3d), level,
						GLsizei(offset_x), GLsizei(offset_y), GLsizei(offset_z),
						GLsizei(width), GLsizei(height), GLsizei(depth),
						gl::detail::convert::to_gl(external_format),
						gl::detail::convert::to_gl(external_type),
						data);
		log::message::minor << "A " << *this << " was modified.";

		if (generate_mipmap) {
			glGenerateMipmap(gl::detail::convert::to_gl(target::_3d));
			log::info::minor << "Mipmap was generated.";
		}
	}
}
template <>
template <>
void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::data(
	void *data, size_t offset_x, size_t offset_c, size_t width, size_t count, bool generate_mipmap,
	external_format external_format, external_type external_type, int level) {

	if (width + offset_x > this->width || count + offset_c > this->count) {
		log::warning::critical << "Attempt to change data out of texture bounds";
		return;
	}

	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexSubImage2D(gl::detail::convert::to_gl(target::_1d_array), level,
						GLsizei(offset_x), GLsizei(offset_c),
						GLsizei(width), GLsizei(count),
						gl::detail::convert::to_gl(external_format),
						gl::detail::convert::to_gl(external_type),
						data);
		log::message::minor << "A " << *this << " was modified.";

		if (generate_mipmap) {
			glGenerateMipmap(gl::detail::convert::to_gl(target::_1d_array));
			log::info::minor << "Mipmap was generated.";
		}
	}
}
template <>
template <>
void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::data(
	void *data, size_t offset_x, size_t offset_y, size_t offset_c,
	size_t width, size_t height, size_t count, bool generate_mipmap,
	external_format external_format, external_type external_type, int level) {

	if (width + offset_x > this->width || height + offset_y > this->height || count + offset_c > this->count) {
		log::warning::critical << "Attempt to change data out of texture bounds";
		return;
	}

	if (auto context = access_context(); context) {
		auto guard = this->bind();
		glTexSubImage3D(gl::detail::convert::to_gl(target::_2d_array), level,
						GLsizei(offset_x), GLsizei(offset_y), GLsizei(offset_c),
						GLsizei(width), GLsizei(height), GLsizei(count),
						gl::detail::convert::to_gl(external_format),
						gl::detail::convert::to_gl(external_type),
						data);
		log::message::minor << "A " << *this << " was modified.";

		if (generate_mipmap) {
			glGenerateMipmap(gl::detail::convert::to_gl(target::_2d_array));
			log::info::minor << "Mipmap was generated.";
		}
	}
}

template <clap::gl::texture::detail::target texture_type>
typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<texture_type>::operator()() {
	if (auto context = texture_ref.access_context(); context) {
		if (context->texture_stack.empty()) {
			int value = -1;
			glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &value);
			if (value < 0)
				log::error::critical << "Error attempting to request 'GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS'.";
			else
				context->texture_stack.resize((size_t) value);
		}
		if (unit >= context->texture_stack.size()) {
			log::error::critical << "Attempting to bind texture to a non-existent texture unit.";
			log::info::major << "Requested target: " << unit << ".";
			log::info::major << "Available targets: " << context->texture_stack.size();
		}

		detail::generic_interface ptr = &texture_ref;
		auto out = context->texture_stack[unit].push(ptr);

		log::message::negligible << "A " << texture_ref << " was bound.";
		log::info::major << "It was added to the top of the stack.";
		glActiveTexture(GLenum(GL_TEXTURE0 + unit));
		glBindTexture(gl::detail::convert::to_gl(texture_type), texture_ref.id);

		return out;
	}
}

template <clap::gl::texture::detail::target texture_type>
void clap::gl::texture::detail::unbind_texture_callable<texture_type>::operator()(typename essential::stack<generic_interface>::iterator iterator) {
	if (auto context = texture_ref.access_context(); context) {
		if (unit >= context->texture_stack.size()) {
			log::error::critical << "Attempting to unbind texture from a non-existent texture unit.";
			log::info::major << "Requested target: " << unit << ".";
			log::info::major << "Available targets: " << context->texture_stack.size();
		} else if (context->texture_stack[unit].is_front(iterator)) {
			if (auto active = context->texture_stack[unit].pop(); std::visit([](auto *a) { return bool(a); }, active))
				log::message::negligible << "A " << *std::get<detail::interface<texture_type> const *>(active) << " was unbound.";
			else
				log::warning::minor << "Stopping usage of a " << texture_type << " object after it was already destroyed.";

			glActiveTexture(GLenum(GL_TEXTURE0 + unit));
			if (context->texture_stack[unit].empty()) {
				log::info::major << "Stack is empty.";
				glBindTexture(gl::detail::convert::to_gl(texture_type), 0);
			} else {
				auto reactivated = context->texture_stack[unit].peek();
				auto reactivated_ptr = std::get<detail::interface<texture_type> const *>(reactivated);
				log::info::major << "A previously used " << *reactivated_ptr
					<< " was rebound, as it's the next element on the stack.";
				glBindTexture(gl::detail::convert::to_gl(texture_type), reactivated_ptr->id);
			}
		} else {
			log::message::negligible << "Removing a " << texture_ref << " from bound texture stack.";
			log::info::major << "This doesn't affect any currently bound textures in any way.";
			context->texture_stack[unit].erase(iterator);
		}
	}
}


template clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::~interface();
template clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::~interface();
template clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::~interface();
template clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::~interface();
template clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::~interface();
template clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::~interface();
template clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::~interface();
template clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::~interface();
template clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::~interface();
template clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::~interface();
template clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::~interface();

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode);

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_base_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_base_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_base_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_base_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_base_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_base_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_base_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_base_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_base_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_base_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_base_level(int level);

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_border_color(float r, float g, float b, float a);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_border_color(float r, float g, float b, float a);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_border_color(float r, float g, float b, float a);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_border_color(float r, float g, float b, float a);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_border_color(float r, float g, float b, float a);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_border_color(float r, float g, float b, float a);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_border_color(float r, float g, float b, float a);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_border_color(float r, float g, float b, float a);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_border_color(float r, float g, float b, float a);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_border_color(float r, float g, float b, float a);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_border_color(float r, float g, float b, float a);

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_lod_bias(float bias);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_lod_bias(float bias);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_lod_bias(float bias);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_lod_bias(float bias);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_lod_bias(float bias);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_lod_bias(float bias);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_lod_bias(float bias);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_lod_bias(float bias);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_lod_bias(float bias);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_lod_bias(float bias);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_lod_bias(float bias);

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_min_filter(min_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_min_filter(min_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_min_filter(min_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_min_filter(min_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_min_filter(min_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_min_filter(min_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_min_filter(min_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_min_filter(min_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_min_filter(min_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_min_filter(min_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_min_filter(min_filter filter);

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_mag_filter(mag_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_mag_filter(mag_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_mag_filter(mag_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_mag_filter(mag_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_mag_filter(mag_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_mag_filter(mag_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_mag_filter(mag_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_mag_filter(mag_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_mag_filter(mag_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_mag_filter(mag_filter filter);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_mag_filter(mag_filter filter);

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_min_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_min_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_min_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_min_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_min_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_min_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_min_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_min_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_min_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_min_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_min_lod(float value);

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_max_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_max_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_max_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_max_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_max_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_max_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_max_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_max_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_max_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_max_lod(float value);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_max_lod(float value);

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_max_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_max_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_max_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_max_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_max_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_max_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_max_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_max_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_max_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_max_level(int level);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_max_level(int level);

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_texture_wrap_s(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_texture_wrap_s(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_texture_wrap_s(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_texture_wrap_s(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_texture_wrap_s(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_texture_wrap_s(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_texture_wrap_s(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_texture_wrap_s(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_texture_wrap_s(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_texture_wrap_s(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_texture_wrap_s(wrap wrap);

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_texture_wrap_t(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_texture_wrap_t(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_texture_wrap_t(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_texture_wrap_t(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_texture_wrap_t(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_texture_wrap_t(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_texture_wrap_t(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_texture_wrap_t(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_texture_wrap_t(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_texture_wrap_t(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_texture_wrap_t(wrap wrap);

template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::set_texture_wrap_r(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::set_texture_wrap_r(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::set_texture_wrap_r(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::set_texture_wrap_r(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::set_texture_wrap_r(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::set_texture_wrap_r(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::set_texture_wrap_r(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::set_texture_wrap_r(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::set_texture_wrap_r(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::set_texture_wrap_r(wrap wrap);
template void clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::set_texture_wrap_r(wrap wrap);

template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::maximum_size();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::maximum_size();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::maximum_size();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::maximum_size();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::maximum_size();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::maximum_size();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::maximum_size();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::maximum_size();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::maximum_size();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::maximum_size();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::maximum_size();

template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::maximum_layer_count();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::maximum_layer_count();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::maximum_layer_count();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::maximum_layer_count();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::maximum_layer_count();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::maximum_layer_count();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::maximum_layer_count();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::maximum_layer_count();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::maximum_layer_count();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::maximum_layer_count();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::maximum_layer_count();

template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d>::maximum_size_3d();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d>::maximum_size_3d();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_3d>::maximum_size_3d();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_1d_array>::maximum_size_3d();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::_2d_array>::maximum_size_3d();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::rectangle>::maximum_size_3d();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map>::maximum_size_3d();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::cube_map_array>::maximum_size_3d();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::buffer>::maximum_size_3d();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample>::maximum_size_3d();
template size_t clap::gl::texture::detail::interface<clap::gl::texture::detail::target::multisample_array>::maximum_size_3d();

template typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<clap::gl::texture::detail::target::_1d>::operator()();
template typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<clap::gl::texture::detail::target::_2d>::operator()();
template typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<clap::gl::texture::detail::target::_3d>::operator()();
template typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<clap::gl::texture::detail::target::_1d_array>::operator()();
template typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<clap::gl::texture::detail::target::_2d_array>::operator()();
template typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<clap::gl::texture::detail::target::rectangle>::operator()();
template typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<clap::gl::texture::detail::target::cube_map>::operator()();
template typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<clap::gl::texture::detail::target::cube_map_array>::operator()();
template typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<clap::gl::texture::detail::target::buffer>::operator()();
template typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<clap::gl::texture::detail::target::multisample>::operator()();
template typename clap::essential::stack<clap::gl::texture::detail::generic_interface>::iterator clap::gl::texture::detail::bind_texture_callable<clap::gl::texture::detail::target::multisample_array>::operator()();

template void clap::gl::texture::detail::unbind_texture_callable<clap::gl::texture::detail::target::_1d>::operator()(typename essential::stack<clap::gl::texture::detail::generic_interface>::iterator iterator);
template void clap::gl::texture::detail::unbind_texture_callable<clap::gl::texture::detail::target::_2d>::operator()(typename essential::stack<clap::gl::texture::detail::generic_interface>::iterator iterator);
template void clap::gl::texture::detail::unbind_texture_callable<clap::gl::texture::detail::target::_3d>::operator()(typename essential::stack<clap::gl::texture::detail::generic_interface>::iterator iterator);
template void clap::gl::texture::detail::unbind_texture_callable<clap::gl::texture::detail::target::_1d_array>::operator()(typename essential::stack<clap::gl::texture::detail::generic_interface>::iterator iterator);
template void clap::gl::texture::detail::unbind_texture_callable<clap::gl::texture::detail::target::_2d_array>::operator()(typename essential::stack<clap::gl::texture::detail::generic_interface>::iterator iterator);
template void clap::gl::texture::detail::unbind_texture_callable<clap::gl::texture::detail::target::rectangle>::operator()(typename essential::stack<clap::gl::texture::detail::generic_interface>::iterator iterator);
template void clap::gl::texture::detail::unbind_texture_callable<clap::gl::texture::detail::target::cube_map>::operator()(typename essential::stack<clap::gl::texture::detail::generic_interface>::iterator iterator);
template void clap::gl::texture::detail::unbind_texture_callable<clap::gl::texture::detail::target::cube_map_array>::operator()(typename essential::stack<clap::gl::texture::detail::generic_interface>::iterator iterator);
template void clap::gl::texture::detail::unbind_texture_callable<clap::gl::texture::detail::target::buffer>::operator()(typename essential::stack<clap::gl::texture::detail::generic_interface>::iterator iterator);
template void clap::gl::texture::detail::unbind_texture_callable<clap::gl::texture::detail::target::multisample>::operator()(typename essential::stack<clap::gl::texture::detail::generic_interface>::iterator iterator);
template void clap::gl::texture::detail::unbind_texture_callable<clap::gl::texture::detail::target::multisample_array>::operator()(typename essential::stack<clap::gl::texture::detail::generic_interface>::iterator iterator);

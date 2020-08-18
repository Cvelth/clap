#include "gl/texture.hpp"

#include "glad/gl.h"

#include "gl/detail/state.hpp"
#include "essential/log.hpp"

clap::gl::texture::detail::interface::interface(texture::target target,
												texture::internal_format internal_format)
	: target(target), internal_format(internal_format) {
	gl::detail::state::ensure_loaded();

	glGenTextures(1, &id);
	log::message::minor << "A new texture of type \"" << target << "\" was created.";
}
clap::gl::texture::detail::interface::~interface() {
	while (auto target = gl::detail::state::is_bound(this))
		gl::detail::state::unbind(*target);

	glDeleteTextures(1, &id);
	log::message::minor << "A texture of type \"" << target << "\" was destroyed.";
}

void clap::gl::texture::detail::interface::bind() {
	clap::gl::detail::state::bind(target, this);
}

template <GLenum parameter_name, typename lambda_t, typename ...Ts>
void set_parameter_template(clap::gl::texture::target const &target, 
							clap::gl::texture::detail::interface *ptr, 
							lambda_t lambda, Ts...params) {
	auto was_bound = clap::gl::detail::state::unbind(target);
	clap::gl::detail::state::bind(target, ptr);

	lambda(clap::gl::detail::convert::to_gl(target), parameter_name, params...);

	if (was_bound)
		clap::gl::detail::state::bind(target, was_bound);
}

void clap::gl::texture::detail::interface::set_depth_stencil_texture_mode(depth_stencil_texture_mode mode) {
	set_parameter_template<GL_DEPTH_STENCIL_TEXTURE_MODE>(target, this, glTexParameteri,
														  gl::detail::convert::to_gl(mode));
}
void clap::gl::texture::detail::interface::set_base_level(int level) {
	set_parameter_template<GL_TEXTURE_BASE_LEVEL>(target, this, glTexParameteri, level);
}
void clap::gl::texture::detail::interface::set_texure_border_color(float r, float g, float b, float a) {
	GLfloat temp[] = { r, g, b, a };
	set_parameter_template<GL_TEXTURE_BORDER_COLOR>(target, this, glTexParameterfv, temp);
}
void clap::gl::texture::detail::interface::set_lod_bias(float bias) {
	set_parameter_template<GL_TEXTURE_LOD_BIAS>(target, this, glTexParameterf, bias);
}
void clap::gl::texture::detail::interface::set_min_filter(min_filter filter) {
	set_parameter_template<GL_TEXTURE_MIN_FILTER>(target, this, glTexParameteri, 
												  gl::detail::convert::to_gl(filter));
}
void clap::gl::texture::detail::interface::set_mag_filter(mag_filter filter) {
	set_parameter_template<GL_TEXTURE_MAG_FILTER>(target, this, glTexParameteri,
												  gl::detail::convert::to_gl(filter));
}
void clap::gl::texture::detail::interface::set_min_lod(float value) {
	set_parameter_template<GL_TEXTURE_MIN_LOD>(target, this, glTexParameterf, value);
}
void clap::gl::texture::detail::interface::set_max_lod(float value) {
	set_parameter_template<GL_TEXTURE_MAX_LOD>(target, this, glTexParameterf, value);
}
void clap::gl::texture::detail::interface::set_max_level(int level) {
	set_parameter_template<GL_TEXTURE_MAX_LEVEL>(target, this, glTexParameteri, level);
}
void clap::gl::texture::detail::interface::set_texture_wrap_s(wrap wrap) {
	set_parameter_template<GL_TEXTURE_WRAP_S>(target, this, glTexParameteri,
											  gl::detail::convert::to_gl(wrap));
}
void clap::gl::texture::detail::interface::set_texture_wrap_t(wrap wrap) {
	set_parameter_template<GL_TEXTURE_WRAP_T>(target, this, glTexParameteri,
											  gl::detail::convert::to_gl(wrap));
}
void clap::gl::texture::detail::interface::set_texture_wrap_r(wrap wrap) {
	set_parameter_template<GL_TEXTURE_WRAP_R>(target, this, glTexParameteri,
											  gl::detail::convert::to_gl(wrap));
}


clap::gl::texture::_1d::_1d(texture::target target, void *data, size_t width, bool generate_mipmap,
							texture::internal_format internal_format, external_format external_format,
							external_type external_type)
	: detail::interface(target, internal_format), width(width) {
	auto was_bound = gl::detail::state::unbind(target);
	gl::detail::state::bind(target, this);

	glTexImage1D(gl::detail::convert::to_gl(target), 0,
				 gl::detail::convert::to_gl(internal_format),
				 GLsizei(width), 0,
				 gl::detail::convert::to_gl(external_format),
				 gl::detail::convert::to_gl(external_type),
				 data);
	log::message::minor << "A new texture of type \"" << target << "\" was initialized.";
	log::info::major << "Dimentions are (" << width << ").";

	if (generate_mipmap) {
		glGenerateMipmap(gl::detail::convert::to_gl(target));
		log::info::minor << "Mipmap was generated.";
	}

	if (was_bound)
		gl::detail::state::bind(target, was_bound);
}

void clap::gl::texture::_1d::data(void *data, size_t offset, size_t width, bool generate_mipmap,
								  int level, external_format external_format, external_type external_type) {
	if (width + offset > this->width) {
		log::warning::critical << "Attempt to change data out of texture bounds";
		return;
	}

	auto was_bound = gl::detail::state::unbind(target);
	gl::detail::state::bind(target, this);

	glTexSubImage1D(gl::detail::convert::to_gl(target), level,
					GLsizei(offset), GLsizei(width),
					gl::detail::convert::to_gl(external_format),
					gl::detail::convert::to_gl(external_type),
					data);
	log::message::minor << "Texture (" << target << ") was modified.";

	if (generate_mipmap) {
		glGenerateMipmap(gl::detail::convert::to_gl(target));
		log::info::minor << "Mipmap was generated.";
	}

	if (was_bound)
		gl::detail::state::bind(target, was_bound);
}


clap::gl::texture::_2d::_2d(texture::target target, void *data, size_t width, size_t height, 
							bool generate_mipmap,
							texture::internal_format internal_format, external_format external_format,
							external_type external_type)
	: detail::interface(target, internal_format), width(width), height(height) {
	auto was_bound = gl::detail::state::unbind(target);
	gl::detail::state::bind(target, this);

	glTexImage2D(gl::detail::convert::to_gl(target), 0,
				 gl::detail::convert::to_gl(internal_format),
				 GLsizei(width), GLsizei(height), 0,
				 gl::detail::convert::to_gl(external_format),
				 gl::detail::convert::to_gl(external_type),
				 data);
	log::message::minor << "A new texture of type \"" << target << "\" was initialized.";
	log::info::major << "Dimentions are (" << width << ", " << height << ").";

	if (generate_mipmap) {
		glGenerateMipmap(gl::detail::convert::to_gl(target));
		log::info::minor << "Mipmap was generated.";
	}

	if (was_bound)
		gl::detail::state::bind(target, was_bound);
}

void clap::gl::texture::_2d::data(void *data, size_t offset_x, size_t offset_y, size_t width, size_t height, 
								  bool generate_mipmap, int level, 
								  external_format external_format, external_type external_type) {
	if (width + offset_x > this->width || height + offset_y > this->height) {
		log::warning::critical << "Attempt to change data out of texture bounds";
		return;
	}

	auto was_bound = gl::detail::state::unbind(target);
	gl::detail::state::bind(target, this);

	glTexSubImage2D(gl::detail::convert::to_gl(target), level,
					GLsizei(offset_x), GLsizei(offset_y),
					GLsizei(width), GLsizei(height),
					gl::detail::convert::to_gl(external_format),
					gl::detail::convert::to_gl(external_type),
					data);
	log::message::minor << "Texture (" << target << ") was modified.";

	if (generate_mipmap) {
		glGenerateMipmap(gl::detail::convert::to_gl(target));
		log::info::minor << "Mipmap was generated.";
	}

	if (was_bound)
		gl::detail::state::bind(target, was_bound);
}


clap::gl::texture::_3d::_3d(texture::target target, void *data, size_t width, size_t height, size_t depth,
							bool generate_mipmap, texture::internal_format internal_format, 
							external_format external_format, external_type external_type)
	: detail::interface(target, internal_format), width(width), height(height), depth(depth) {
	auto was_bound = gl::detail::state::unbind(target);
	gl::detail::state::bind(target, this);

	glTexImage3D(gl::detail::convert::to_gl(target), 0,
				 gl::detail::convert::to_gl(internal_format),
				 GLsizei(width), GLsizei(height), GLsizei(depth), 0,
				 gl::detail::convert::to_gl(external_format),
				 gl::detail::convert::to_gl(external_type),
				 data);
	log::message::minor << "A new texture of type \"" << target << "\" was initialized.";
	log::info::major << "Dimentions are (" << width << ", " << height << ", " << depth << ").";

	if (generate_mipmap) {
		glGenerateMipmap(gl::detail::convert::to_gl(target));
		log::info::minor << "Mipmap was generated.";
	}

	if (was_bound)
		gl::detail::state::bind(target, was_bound);
}

void clap::gl::texture::_3d::data(void *data, size_t offset_x, size_t offset_y, size_t offset_z,
								  size_t width, size_t height, size_t depth, bool generate_mipmap,
								  int level, external_format external_format, external_type external_type) {
	if (width + offset_x > this->width || height + offset_y > this->height || depth + offset_z > this->depth) {
		log::warning::critical << "Attempt to change data out of texture bounds";
		return;
	}

	auto was_bound = gl::detail::state::unbind(target);
	gl::detail::state::bind(target, this);

	glTexSubImage3D(gl::detail::convert::to_gl(target), level,
					GLsizei(offset_x), GLsizei(offset_y), GLsizei(offset_z),
					GLsizei(width), GLsizei(height), GLsizei(depth),
					gl::detail::convert::to_gl(external_format),
					gl::detail::convert::to_gl(external_type),
					data);
	log::message::minor << "Texture (" << target << ") was modified.";

	if (generate_mipmap) {
		glGenerateMipmap(gl::detail::convert::to_gl(target));
		log::info::minor << "Mipmap was generated.";
	}

	if (was_bound)
		gl::detail::state::bind(target, was_bound);
}

clap::gl::texture::multisample::multisample(void *data, size_t sample_count, size_t width, size_t height,
											bool are_samples_fixed, texture::internal_format internal_format)
	: detail::interface(target::multisample, internal_format), width(width), height(height) {
	auto was_bound = gl::detail::state::unbind(target);
	gl::detail::state::bind(target, this);

	glTexImage2DMultisample(gl::detail::convert::to_gl(target), GLsizei(sample_count),
							gl::detail::convert::to_gl(internal_format),
							GLsizei(width), GLsizei(height), are_samples_fixed);
	log::message::minor << "A new texture of type \"" << target << "\" was initialized.";
	log::info::major << "Dimentions are (" << width << ", " << height << ").";
	log::info::minor << "'sample_count' is " << sample_count << ".";

	if (was_bound)
		gl::detail::state::bind(target, was_bound);
}

clap::gl::texture::multisample_array::multisample_array(void *data, size_t sample_count,
														size_t width, size_t height, size_t depth,
														bool are_samples_fixed,
														texture::internal_format internal_format)
	: detail::interface(target::multisample_array, internal_format), width(width), height(height), depth(depth) {
	auto was_bound = gl::detail::state::unbind(target);
	gl::detail::state::bind(target, this);

	glTexImage3DMultisample(gl::detail::convert::to_gl(target), GLsizei(sample_count),
							gl::detail::convert::to_gl(internal_format),
							GLsizei(width), GLsizei(height), GLsizei(depth), are_samples_fixed);
	log::message::minor << "A new texture of type \"" << target << "\" was initialized.";
	log::info::major << "Dimentions are (" << width << ", " << height << ", " << depth << ").";
	log::info::minor << "'sample_count' is " << sample_count << ".";

	if (was_bound)
		gl::detail::state::bind(target, was_bound);
}

#include "essential/log.hpp"

#include "gl/buffer.hpp"
#include "gl/misc.hpp"
#include "gl/shader.hpp"
#include "gl/texture.hpp"
#include "gl/vertex_array.hpp"

#include "glad/glad.h"

#pragma warning(push)
#pragma warning(disable : 4715)

GLenum clap::gl::detail::convert::to_gl(clap::gl::buffer::target v) {
	switch (v) {
		case clap::gl::buffer::target::array: return GL_ARRAY_BUFFER;
		case clap::gl::buffer::target::atomic_counter: return GL_ATOMIC_COUNTER_BUFFER;
		case clap::gl::buffer::target::copy_read: return GL_COPY_READ_BUFFER;
		case clap::gl::buffer::target::copy_write: return GL_COPY_WRITE_BUFFER;
		case clap::gl::buffer::target::indirect_dispatch: return GL_DISPATCH_INDIRECT_BUFFER;
		case clap::gl::buffer::target::indirect_draw: return GL_DRAW_INDIRECT_BUFFER;
		case clap::gl::buffer::target::element_array: return GL_ELEMENT_ARRAY_BUFFER;
		case clap::gl::buffer::target::pixel_pack: return GL_PIXEL_PACK_BUFFER;
		case clap::gl::buffer::target::pixel_unpack: return GL_PIXEL_UNPACK_BUFFER;
		case clap::gl::buffer::target::query: return GL_QUERY_BUFFER;
		case clap::gl::buffer::target::shader_storage: return GL_SHADER_STORAGE_BUFFER;
		case clap::gl::buffer::target::texture: return GL_TEXTURE_BUFFER;
		case clap::gl::buffer::target::transform_feedback: return GL_TRANSFORM_FEEDBACK_BUFFER;
		case clap::gl::buffer::target::uniform: return GL_UNIFORM_BUFFER;
	}
	log::error::critical << "Unsupported enum value.";
}
clap::gl::buffer::target clap::gl::detail::convert::to_buffer_target(GLenum v) {
	switch (v) {
		case GL_ARRAY_BUFFER: return clap::gl::buffer::target::array;
		case GL_ATOMIC_COUNTER_BUFFER: return clap::gl::buffer::target::atomic_counter;
		case GL_COPY_READ_BUFFER: return clap::gl::buffer::target::copy_read;
		case GL_COPY_WRITE_BUFFER: return clap::gl::buffer::target::copy_write;
		case GL_DISPATCH_INDIRECT_BUFFER: return clap::gl::buffer::target::indirect_dispatch;
		case GL_DRAW_INDIRECT_BUFFER: return clap::gl::buffer::target::indirect_draw;
		case GL_ELEMENT_ARRAY_BUFFER: return clap::gl::buffer::target::element_array;
		case GL_PIXEL_PACK_BUFFER: return clap::gl::buffer::target::pixel_pack;
		case GL_PIXEL_UNPACK_BUFFER: return clap::gl::buffer::target::pixel_unpack;
		case GL_QUERY_BUFFER: return clap::gl::buffer::target::query;
		case GL_SHADER_STORAGE_BUFFER: return clap::gl::buffer::target::shader_storage;
		case GL_TEXTURE_BUFFER: return clap::gl::buffer::target::texture;
		case GL_TRANSFORM_FEEDBACK_BUFFER: return clap::gl::buffer::target::transform_feedback;
		case GL_UNIFORM_BUFFER: return clap::gl::buffer::target::uniform;
	}
	log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::buffer::access v) {
	switch (v) {
		case clap::gl::buffer::access::read_only: return GL_READ_ONLY;
		case clap::gl::buffer::access::read_write: return GL_READ_WRITE;
		case clap::gl::buffer::access::write_only: return GL_WRITE_ONLY;
	}
	log::error::critical << "Unsupported enum value.";
}
clap::gl::buffer::access clap::gl::detail::convert::to_access(GLenum v) {
	switch (v) {
		case GL_READ_ONLY: return clap::gl::buffer::access::read_only;
		case GL_READ_WRITE: return clap::gl::buffer::access::read_write;
		case GL_WRITE_ONLY: return clap::gl::buffer::access::write_only;
	}
	log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::buffer::usage v) {
	switch (v) {
		case clap::gl::buffer::usage::stream_draw: return GL_STREAM_DRAW;
		case clap::gl::buffer::usage::stream_read: return GL_STREAM_READ;
		case clap::gl::buffer::usage::stream_copy: return GL_STREAM_COPY;
		case clap::gl::buffer::usage::static_draw: return GL_STATIC_DRAW;
		case clap::gl::buffer::usage::static_read: return GL_STATIC_READ;
		case clap::gl::buffer::usage::static_copy: return GL_STATIC_COPY;
		case clap::gl::buffer::usage::dynamic_draw: return GL_DYNAMIC_DRAW;
		case clap::gl::buffer::usage::dynamic_read: return GL_DYNAMIC_READ;
		case clap::gl::buffer::usage::dynamic_copy: return GL_DYNAMIC_COPY;
	}
	log::error::critical << "Unsupported enum value.";
}
clap::gl::buffer::usage clap::gl::detail::convert::to_usage(GLenum v) {
	switch (v) {
		case GL_STREAM_DRAW: return clap::gl::buffer::usage::stream_draw;
		case GL_STREAM_READ: return clap::gl::buffer::usage::stream_read;
		case GL_STREAM_COPY: return clap::gl::buffer::usage::stream_copy;
		case GL_STATIC_DRAW: return clap::gl::buffer::usage::static_draw;
		case GL_STATIC_READ: return clap::gl::buffer::usage::static_read;
		case GL_STATIC_COPY: return clap::gl::buffer::usage::static_copy;
		case GL_DYNAMIC_DRAW: return clap::gl::buffer::usage::dynamic_draw;
		case GL_DYNAMIC_READ: return clap::gl::buffer::usage::dynamic_read;
		case GL_DYNAMIC_COPY: return clap::gl::buffer::usage::dynamic_copy;
	}
	log::error::critical << "Unsupported enum value.";
}

std::ostream &operator<<(std::ostream &stream, clap::gl::buffer::target target) {
	switch (target) {
		case clap::gl::buffer::target::array:
			stream << "buffer::target::array"; break;
		case clap::gl::buffer::target::atomic_counter:
			stream << "buffer::target::atomic_counter"; break;
		case clap::gl::buffer::target::copy_read:
			stream << "buffer::target::copy_read"; break;
		case clap::gl::buffer::target::copy_write:
			stream << "buffer::target::copy_write"; break;
		case clap::gl::buffer::target::indirect_dispatch:
			stream << "buffer::target::indirect_dispatch"; break;
		case clap::gl::buffer::target::indirect_draw:
			stream << "buffer::target::indirect_draw"; break;
		case clap::gl::buffer::target::element_array:
			stream << "buffer::target::element_array"; break;
		case clap::gl::buffer::target::pixel_pack:
			stream << "buffer::target::pixel_pack"; break;
		case clap::gl::buffer::target::pixel_unpack:
			stream << "buffer::target::pixel_unpack"; break;
		case clap::gl::buffer::target::query:
			stream << "buffer::target::query"; break;
		case clap::gl::buffer::target::shader_storage:
			stream << "buffer::target::shader_storage"; break;
		case clap::gl::buffer::target::texture:
			stream << "buffer::target::texture"; break;
		case clap::gl::buffer::target::transform_feedback:
			stream << "buffer::target::transform_feedback"; break;
		case clap::gl::buffer::target::uniform:
			stream << "buffer::target::uniform"; break;
		default:
			clap::log::warning::major << "Unsupported enum value.";
	}
	return stream;
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::shader::type v) {
	switch (v) {
		case clap::gl::shader::type::fragment:					return GL_FRAGMENT_SHADER;
		case clap::gl::shader::type::vertex:					return GL_VERTEX_SHADER;
		case clap::gl::shader::type::geometry:					return GL_GEOMETRY_SHADER;
		case clap::gl::shader::type::compute:					return GL_COMPUTE_SHADER;
		case clap::gl::shader::type::tesselation_control:		return GL_TESS_CONTROL_SHADER;
		case clap::gl::shader::type::tesselation_evaluation:	return GL_TESS_EVALUATION_SHADER;
	}
	log::error::critical << "Unsupported enum value.";
}
clap::gl::shader::type clap::gl::detail::convert::to_shader_type(GLenum v) {
	switch (v) {
		case GL_FRAGMENT_SHADER:		return clap::gl::shader::type::fragment;
		case GL_VERTEX_SHADER:			return clap::gl::shader::type::vertex;
		case GL_GEOMETRY_SHADER:		return clap::gl::shader::type::geometry;
		case GL_COMPUTE_SHADER:			return clap::gl::shader::type::compute;
		case GL_TESS_CONTROL_SHADER:	return clap::gl::shader::type::tesselation_control;
		case GL_TESS_EVALUATION_SHADER: return clap::gl::shader::type::tesselation_evaluation;
	}
	log::error::critical << "Unsupported enum value.";
}

clap::gl::shader::type clap::gl::detail::convert::to_shader_type_from_string(std::string const &v) {
	if (v == "fragment")
		return clap::gl::shader::type::fragment;
	else if (v == "vertex")
		return clap::gl::shader::type::vertex;
	else if (v == "geometry")
		return clap::gl::shader::type::geometry;
	else if (v == "compute")
		return clap::gl::shader::type::compute;
	else if (v == "tesselation_control")
		return clap::gl::shader::type::tesselation_control;
	else if (v == "tesselation_evaluation")
		return clap::gl::shader::type::tesselation_evaluation;

	log::error::critical << "Unsupported enum value.";
}

struct attribute_type_visitor {
	GLenum operator()(float v) { return GL_FLOAT; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec2<float> v) { return GL_FLOAT_VEC2; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec3<float> v) { return GL_FLOAT_VEC3; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec4<float> v) { return GL_FLOAT_VEC4; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat2<float> v) { return GL_FLOAT_MAT2; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat3<float> v) { return GL_FLOAT_MAT3; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat4<float> v) { return GL_FLOAT_MAT4; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat2x3<float> v) { return GL_FLOAT_MAT2x3; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat2x4<float> v) { return GL_FLOAT_MAT2x4; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat3x2<float> v) { return GL_FLOAT_MAT3x2; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat3x4<float> v) { return GL_FLOAT_MAT3x4; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat4x2<float> v) { return GL_FLOAT_MAT4x2; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat4x3<float> v) { return GL_FLOAT_MAT4x3; }
	GLenum operator()(int v) { return GL_INT; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec2<int> v) { return GL_INT_VEC2; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec3<int> v) { return GL_INT_VEC3; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec4<int> v) { return GL_INT_VEC4; }
	GLenum operator()(unsigned v) { return GL_UNSIGNED_INT; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec2<unsigned> v) { return GL_UNSIGNED_INT_VEC2; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec3<unsigned> v) { return GL_UNSIGNED_INT_VEC3; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec4<unsigned> v) { return GL_UNSIGNED_INT_VEC4; }
	GLenum operator()(double v) { return GL_DOUBLE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec2<double> v) { return GL_DOUBLE_VEC2; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec3<double> v) { return GL_DOUBLE_VEC3; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec4<double> v) { return GL_DOUBLE_VEC4; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat2<double> v) { return GL_DOUBLE_MAT2; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat3<double> v) { return GL_DOUBLE_MAT3; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat4<double> v) { return GL_DOUBLE_MAT4; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat2x3<double> v) { return GL_DOUBLE_MAT2x3; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat2x4<double> v) { return GL_DOUBLE_MAT2x4; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat3x2<double> v) { return GL_DOUBLE_MAT3x2; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat3x4<double> v) { return GL_DOUBLE_MAT3x4; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat4x2<double> v) { return GL_DOUBLE_MAT4x2; }
	GLenum operator()(clap::gl::shader::variable::type::detail::mat4x3<double> v) { return GL_DOUBLE_MAT4x3; }
};
struct attribute_simple_type_visitor {
	template<typename variable_t, size_t vector_size> 
	GLenum operator()(clap::gl::shader::variable::type::detail::vec<variable_t, vector_size> const &input) { 
		return impl<variable_t>(); 
	}
	template<typename variable_t, size_t width, size_t height> 
	GLenum operator()(clap::gl::shader::variable::type::detail::mat<variable_t, width, height> const &input) { 
		return impl<variable_t>(); 
	}
	template <typename variable_t> GLenum operator()(variable_t const &input) {
		return impl<variable_t>(); 
	}

private:
	template <typename variable_t>
	GLenum impl();
};
template<> GLenum attribute_simple_type_visitor::impl<float>() { return GL_FLOAT; }
template<> GLenum attribute_simple_type_visitor::impl<int>() { return GL_INT; }
template<> GLenum attribute_simple_type_visitor::impl<unsigned>() { return GL_UNSIGNED_INT; }
template<> GLenum attribute_simple_type_visitor::impl<double>() { return GL_DOUBLE; }

GLenum clap::gl::detail::convert::to_gl(shader::variable::type::attribute const &variant) {
	return std::visit(attribute_type_visitor{}, variant);
}
GLenum clap::gl::detail::convert::to_gl_type(shader::variable::type::attribute const &variant) {
	return std::visit(attribute_simple_type_visitor{}, variant);
}
clap::gl::shader::variable::type::attribute clap::gl::detail::convert::to_attribute(GLenum v) {
	switch (v) {
		case GL_FLOAT: return float{};
		case GL_FLOAT_VEC2: return clap::gl::shader::variable::type::detail::vec2<float>{};
		case GL_FLOAT_VEC3: return clap::gl::shader::variable::type::detail::vec3<float>{};
		case GL_FLOAT_VEC4: return clap::gl::shader::variable::type::detail::vec4<float>{};
		case GL_FLOAT_MAT2: return clap::gl::shader::variable::type::detail::mat2<float>{};
		case GL_FLOAT_MAT3: return clap::gl::shader::variable::type::detail::mat3<float>{};
		case GL_FLOAT_MAT4: return clap::gl::shader::variable::type::detail::mat4<float>{};
		case GL_FLOAT_MAT2x3: return clap::gl::shader::variable::type::detail::mat2x3<float>{};
		case GL_FLOAT_MAT2x4: return clap::gl::shader::variable::type::detail::mat2x4<float>{};
		case GL_FLOAT_MAT3x2: return clap::gl::shader::variable::type::detail::mat3x2<float>{};
		case GL_FLOAT_MAT3x4: return clap::gl::shader::variable::type::detail::mat3x4<float>{};
		case GL_FLOAT_MAT4x2: return clap::gl::shader::variable::type::detail::mat4x2<float>{};
		case GL_FLOAT_MAT4x3: return clap::gl::shader::variable::type::detail::mat4x3<float>{};
		case GL_INT: return int{};
		case GL_INT_VEC2: return clap::gl::shader::variable::type::detail::vec2<int>{};
		case GL_INT_VEC3: return clap::gl::shader::variable::type::detail::vec3<int>{};
		case GL_INT_VEC4: return clap::gl::shader::variable::type::detail::vec4<int>{};
		case GL_UNSIGNED_INT: return unsigned{};
		case GL_UNSIGNED_INT_VEC2: return clap::gl::shader::variable::type::detail::vec2<unsigned>{};
		case GL_UNSIGNED_INT_VEC3: return clap::gl::shader::variable::type::detail::vec3<unsigned>{};
		case GL_UNSIGNED_INT_VEC4: return clap::gl::shader::variable::type::detail::vec4<unsigned>{};
		case GL_DOUBLE: return double{};
		case GL_DOUBLE_VEC2: return clap::gl::shader::variable::type::detail::vec2<double>{};
		case GL_DOUBLE_VEC3: return clap::gl::shader::variable::type::detail::vec3<double>{};
		case GL_DOUBLE_VEC4: return clap::gl::shader::variable::type::detail::vec4<double>{};
		case GL_DOUBLE_MAT2: return clap::gl::shader::variable::type::detail::mat2<double>{};
		case GL_DOUBLE_MAT3: return clap::gl::shader::variable::type::detail::mat3<double>{};
		case GL_DOUBLE_MAT4: return clap::gl::shader::variable::type::detail::mat4<double>{};
		case GL_DOUBLE_MAT2x3: return clap::gl::shader::variable::type::detail::mat2x3<double>{};
		case GL_DOUBLE_MAT2x4: return clap::gl::shader::variable::type::detail::mat2x4<double>{};
		case GL_DOUBLE_MAT3x2: return clap::gl::shader::variable::type::detail::mat3x2<double>{};
		case GL_DOUBLE_MAT3x4: return clap::gl::shader::variable::type::detail::mat3x4<double>{};
		case GL_DOUBLE_MAT4x2: return clap::gl::shader::variable::type::detail::mat4x2<double>{};
		case GL_DOUBLE_MAT4x3: return clap::gl::shader::variable::type::detail::mat4x3<double>{};
	}
	log::error::critical << "Unsupported enum value.";
}

struct uniform_type_visitor : public attribute_type_visitor {
	using attribute_type_visitor::operator();

	GLenum operator()(bool v) { return GL_BOOL; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec2<bool> v) { return GL_BOOL_VEC2; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec3<bool> v) { return GL_BOOL_VEC3; }
	GLenum operator()(clap::gl::shader::variable::type::detail::vec4<bool> v) { return GL_BOOL_VEC4; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_1d<float> v) { return GL_SAMPLER_1D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d<float> v) { return GL_SAMPLER_2D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_3d<float> v) { return GL_SAMPLER_3D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_cube<float> v) { return GL_SAMPLER_CUBE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_cube_shadow<float> v) { return GL_SAMPLER_CUBE_SHADOW; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_1d_shadow<float> v) { return GL_SAMPLER_1D_SHADOW; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d_shadow<float> v) { return GL_SAMPLER_2D_SHADOW; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_1d_array<float> v) { return GL_SAMPLER_1D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d_array<float> v) { return GL_SAMPLER_2D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_1d_array_shadow<float> v) { return GL_SAMPLER_1D_ARRAY_SHADOW; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d_array_shadow<float> v) { return GL_SAMPLER_2D_ARRAY_SHADOW; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d_multisample<float> v) { return GL_SAMPLER_2D_MULTISAMPLE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d_multisample_array<float> v) { return GL_SAMPLER_2D_MULTISAMPLE_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_buffer<float> v) { return GL_SAMPLER_BUFFER; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_rect<float> v) { return GL_SAMPLER_2D_RECT; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_rect_shadow<float> v) { return GL_SAMPLER_2D_RECT_SHADOW; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_1d<int> v) { return GL_INT_SAMPLER_1D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d<int> v) { return GL_INT_SAMPLER_2D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_3d<int> v) { return GL_INT_SAMPLER_3D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_cube<int> v) { return GL_INT_SAMPLER_CUBE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_1d_array<int> v) { return GL_INT_SAMPLER_1D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d_array<int> v) { return GL_INT_SAMPLER_2D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d_multisample<int> v) { return GL_INT_SAMPLER_2D_MULTISAMPLE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d_multisample_array<int> v) { return GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_buffer<int> v) { return GL_INT_SAMPLER_BUFFER; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_rect<int> v) { return GL_INT_SAMPLER_2D_RECT; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_1d<unsigned> v) { return GL_UNSIGNED_INT_SAMPLER_1D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d<unsigned> v) { return GL_UNSIGNED_INT_SAMPLER_2D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_3d<unsigned> v) { return GL_UNSIGNED_INT_SAMPLER_3D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_cube<unsigned> v) { return GL_UNSIGNED_INT_SAMPLER_CUBE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_1d_array<unsigned> v) { return GL_UNSIGNED_INT_SAMPLER_1D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d_array<unsigned> v) { return GL_UNSIGNED_INT_SAMPLER_2D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d_multisample<unsigned> v) { return GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_2d_multisample_array<unsigned> v) { return GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_buffer<unsigned> v) { return GL_UNSIGNED_INT_SAMPLER_BUFFER; }
	GLenum operator()(clap::gl::shader::variable::type::detail::sampler_rect<unsigned> v) { return GL_UNSIGNED_INT_SAMPLER_2D_RECT; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_1d<float> v) { return GL_IMAGE_1D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d<float> v) { return GL_IMAGE_2D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_3d<float> v) { return GL_IMAGE_3D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_rect<float> v) { return GL_IMAGE_2D_RECT; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_cube<float> v) { return GL_IMAGE_CUBE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_buffer<float> v) { return GL_IMAGE_BUFFER; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_1d_array<float> v) { return GL_IMAGE_1D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d_array<float> v) { return GL_IMAGE_2D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d_multisample<float> v) { return GL_IMAGE_2D_MULTISAMPLE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d_multisample_array<float> v) { return GL_IMAGE_2D_MULTISAMPLE_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_1d<int> v) { return GL_INT_IMAGE_1D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d<int> v) { return GL_INT_IMAGE_2D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_3d<int> v) { return GL_INT_IMAGE_3D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_rect<int> v) { return GL_INT_IMAGE_2D_RECT; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_cube<int> v) { return GL_INT_IMAGE_CUBE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_buffer<int> v) { return GL_INT_IMAGE_BUFFER; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_1d_array<int> v) { return GL_INT_IMAGE_1D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d_array<int> v) { return GL_INT_IMAGE_2D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d_multisample<int> v) { return GL_INT_IMAGE_2D_MULTISAMPLE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d_multisample_array<int> v) { return GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_1d<unsigned> v) { return GL_UNSIGNED_INT_IMAGE_1D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d<unsigned> v) { return GL_UNSIGNED_INT_IMAGE_2D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_3d<unsigned> v) { return GL_UNSIGNED_INT_IMAGE_3D; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_rect<unsigned> v) { return GL_UNSIGNED_INT_IMAGE_2D_RECT; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_cube<unsigned> v) { return GL_UNSIGNED_INT_IMAGE_CUBE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_buffer<unsigned> v) { return GL_UNSIGNED_INT_IMAGE_BUFFER; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_1d_array<unsigned> v) { return GL_UNSIGNED_INT_IMAGE_1D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d_array<unsigned> v) { return GL_UNSIGNED_INT_IMAGE_2D_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d_multisample<unsigned> v) { return GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE; }
	GLenum operator()(clap::gl::shader::variable::type::detail::image_2d_multisample_array<unsigned> v) { return GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY; }
	GLenum operator()(clap::gl::shader::variable::type::detail::atomic_counter<unsigned> v) { return GL_UNSIGNED_INT_ATOMIC_COUNTER; }
};
GLenum clap::gl::detail::convert::to_gl(shader::variable::type::uniform const &variant) {
	return std::visit(uniform_type_visitor{}, variant);
}
clap::gl::shader::variable::type::uniform clap::gl::detail::convert::to_uniform(GLenum v) {
	switch (v) {
		case GL_FLOAT: return float{};
		case GL_FLOAT_VEC2: return clap::gl::shader::variable::type::detail::vec2<float>{};
		case GL_FLOAT_VEC3: return clap::gl::shader::variable::type::detail::vec3<float>{};
		case GL_FLOAT_VEC4: return clap::gl::shader::variable::type::detail::vec4<float>{};
		case GL_FLOAT_MAT2: return clap::gl::shader::variable::type::detail::mat2<float>{};
		case GL_FLOAT_MAT3: return clap::gl::shader::variable::type::detail::mat3<float>{};
		case GL_FLOAT_MAT4: return clap::gl::shader::variable::type::detail::mat4<float>{};
		case GL_FLOAT_MAT2x3: return clap::gl::shader::variable::type::detail::mat2x3<float>{};
		case GL_FLOAT_MAT2x4: return clap::gl::shader::variable::type::detail::mat2x4<float>{};
		case GL_FLOAT_MAT3x2: return clap::gl::shader::variable::type::detail::mat3x2<float>{};
		case GL_FLOAT_MAT3x4: return clap::gl::shader::variable::type::detail::mat3x4<float>{};
		case GL_FLOAT_MAT4x2: return clap::gl::shader::variable::type::detail::mat4x2<float>{};
		case GL_FLOAT_MAT4x3: return clap::gl::shader::variable::type::detail::mat4x3<float>{};
		case GL_INT: return int{};
		case GL_INT_VEC2: return clap::gl::shader::variable::type::detail::vec2<int>{};
		case GL_INT_VEC3: return clap::gl::shader::variable::type::detail::vec3<int>{};
		case GL_INT_VEC4: return clap::gl::shader::variable::type::detail::vec4<int>{};
		case GL_UNSIGNED_INT: return unsigned{};
		case GL_UNSIGNED_INT_VEC2: return clap::gl::shader::variable::type::detail::vec2<unsigned>{};
		case GL_UNSIGNED_INT_VEC3: return clap::gl::shader::variable::type::detail::vec3<unsigned>{};
		case GL_UNSIGNED_INT_VEC4: return clap::gl::shader::variable::type::detail::vec4<unsigned>{};
		case GL_DOUBLE: return double{};
		case GL_DOUBLE_VEC2: return clap::gl::shader::variable::type::detail::vec2<double>{};
		case GL_DOUBLE_VEC3: return clap::gl::shader::variable::type::detail::vec3<double>{};
		case GL_DOUBLE_VEC4: return clap::gl::shader::variable::type::detail::vec4<double>{};
		case GL_DOUBLE_MAT2: return clap::gl::shader::variable::type::detail::mat2<double>{};
		case GL_DOUBLE_MAT3: return clap::gl::shader::variable::type::detail::mat3<double>{};
		case GL_DOUBLE_MAT4: return clap::gl::shader::variable::type::detail::mat4<double>{};
		case GL_DOUBLE_MAT2x3: return clap::gl::shader::variable::type::detail::mat2x3<double>{};
		case GL_DOUBLE_MAT2x4: return clap::gl::shader::variable::type::detail::mat2x4<double>{};
		case GL_DOUBLE_MAT3x2: return clap::gl::shader::variable::type::detail::mat3x2<double>{};
		case GL_DOUBLE_MAT3x4: return clap::gl::shader::variable::type::detail::mat3x4<double>{};
		case GL_DOUBLE_MAT4x2: return clap::gl::shader::variable::type::detail::mat4x2<double>{};
		case GL_DOUBLE_MAT4x3: return clap::gl::shader::variable::type::detail::mat4x3<double>{};

		case GL_BOOL: return bool{};
		case GL_BOOL_VEC2: return clap::gl::shader::variable::type::detail::vec2<bool>{};
		case GL_BOOL_VEC3: return clap::gl::shader::variable::type::detail::vec3<bool>{};
		case GL_BOOL_VEC4: return clap::gl::shader::variable::type::detail::vec4<bool>{};
		case GL_SAMPLER_1D: return clap::gl::shader::variable::type::detail::sampler_1d<float>{};
		case GL_SAMPLER_2D: return clap::gl::shader::variable::type::detail::sampler_2d<float>{};
		case GL_SAMPLER_3D: return clap::gl::shader::variable::type::detail::sampler_3d<float>{};
		case GL_SAMPLER_CUBE: return clap::gl::shader::variable::type::detail::sampler_cube<float>{};
		case GL_SAMPLER_CUBE_SHADOW: return clap::gl::shader::variable::type::detail::sampler_cube_shadow<float>{};
		case GL_SAMPLER_1D_SHADOW: return clap::gl::shader::variable::type::detail::sampler_1d_shadow<float>{};
		case GL_SAMPLER_2D_SHADOW: return clap::gl::shader::variable::type::detail::sampler_2d_shadow<float>{};
		case GL_SAMPLER_1D_ARRAY: return clap::gl::shader::variable::type::detail::sampler_1d_array<float>{};
		case GL_SAMPLER_2D_ARRAY: return clap::gl::shader::variable::type::detail::sampler_2d_array<float>{};
		case GL_SAMPLER_1D_ARRAY_SHADOW: return clap::gl::shader::variable::type::detail::sampler_1d_array_shadow<float>{};
		case GL_SAMPLER_2D_ARRAY_SHADOW: return clap::gl::shader::variable::type::detail::sampler_2d_array_shadow<float>{};
		case GL_SAMPLER_2D_MULTISAMPLE: return clap::gl::shader::variable::type::detail::sampler_2d_multisample<float>{};
		case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return clap::gl::shader::variable::type::detail::sampler_2d_multisample_array<float>{};
		case GL_SAMPLER_BUFFER: return clap::gl::shader::variable::type::detail::sampler_buffer<float>{};
		case GL_SAMPLER_2D_RECT: return clap::gl::shader::variable::type::detail::sampler_rect<float>{};
		case GL_SAMPLER_2D_RECT_SHADOW: return clap::gl::shader::variable::type::detail::sampler_rect_shadow<float>{};
		case GL_INT_SAMPLER_1D: return clap::gl::shader::variable::type::detail::sampler_1d<int>{};
		case GL_INT_SAMPLER_2D: return clap::gl::shader::variable::type::detail::sampler_2d<int>{};
		case GL_INT_SAMPLER_3D: return clap::gl::shader::variable::type::detail::sampler_3d<int>{};
		case GL_INT_SAMPLER_CUBE: return clap::gl::shader::variable::type::detail::sampler_cube<int>{};
		case GL_INT_SAMPLER_1D_ARRAY: return clap::gl::shader::variable::type::detail::sampler_1d_array<int>{};
		case GL_INT_SAMPLER_2D_ARRAY: return clap::gl::shader::variable::type::detail::sampler_2d_array<int>{};
		case GL_INT_SAMPLER_2D_MULTISAMPLE: return clap::gl::shader::variable::type::detail::sampler_2d_multisample<int>{};
		case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return clap::gl::shader::variable::type::detail::sampler_2d_multisample_array<int>{};
		case GL_INT_SAMPLER_BUFFER: return clap::gl::shader::variable::type::detail::sampler_buffer<int>{};
		case GL_INT_SAMPLER_2D_RECT: return clap::gl::shader::variable::type::detail::sampler_rect<int>{};
		case GL_UNSIGNED_INT_SAMPLER_1D: return clap::gl::shader::variable::type::detail::sampler_1d<unsigned>{};
		case GL_UNSIGNED_INT_SAMPLER_2D: return clap::gl::shader::variable::type::detail::sampler_2d<unsigned>{};
		case GL_UNSIGNED_INT_SAMPLER_3D: return clap::gl::shader::variable::type::detail::sampler_3d<unsigned>{};
		case GL_UNSIGNED_INT_SAMPLER_CUBE: return clap::gl::shader::variable::type::detail::sampler_cube<unsigned>{};
		case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: return clap::gl::shader::variable::type::detail::sampler_1d_array<unsigned>{};
		case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return clap::gl::shader::variable::type::detail::sampler_2d_array<unsigned>{};
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return clap::gl::shader::variable::type::detail::sampler_2d_multisample<unsigned>{};
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return clap::gl::shader::variable::type::detail::sampler_2d_multisample_array<unsigned>{};
		case GL_UNSIGNED_INT_SAMPLER_BUFFER: return clap::gl::shader::variable::type::detail::sampler_buffer<unsigned>{};
		case GL_UNSIGNED_INT_SAMPLER_2D_RECT: return clap::gl::shader::variable::type::detail::sampler_rect<unsigned>{};
		case GL_IMAGE_1D: return clap::gl::shader::variable::type::detail::image_1d<float>{};
		case GL_IMAGE_2D: return clap::gl::shader::variable::type::detail::image_2d<float>{};
		case GL_IMAGE_3D: return clap::gl::shader::variable::type::detail::image_3d<float>{};
		case GL_IMAGE_2D_RECT: return clap::gl::shader::variable::type::detail::image_rect<float>{};
		case GL_IMAGE_CUBE: return clap::gl::shader::variable::type::detail::image_cube<float>{};
		case GL_IMAGE_BUFFER: return clap::gl::shader::variable::type::detail::image_buffer<float>{};
		case GL_IMAGE_1D_ARRAY: return clap::gl::shader::variable::type::detail::image_1d_array<float>{};
		case GL_IMAGE_2D_ARRAY: return clap::gl::shader::variable::type::detail::image_2d_array<float>{};
		case GL_IMAGE_2D_MULTISAMPLE: return clap::gl::shader::variable::type::detail::image_2d_multisample<float>{};
		case GL_IMAGE_2D_MULTISAMPLE_ARRAY: return clap::gl::shader::variable::type::detail::image_2d_multisample_array<float>{};
		case GL_INT_IMAGE_1D: return clap::gl::shader::variable::type::detail::image_1d<int>{};
		case GL_INT_IMAGE_2D: return clap::gl::shader::variable::type::detail::image_2d<int>{};
		case GL_INT_IMAGE_3D: return clap::gl::shader::variable::type::detail::image_3d<int>{};
		case GL_INT_IMAGE_2D_RECT: return clap::gl::shader::variable::type::detail::image_rect<int>{};
		case GL_INT_IMAGE_CUBE: return clap::gl::shader::variable::type::detail::image_cube<int>{};
		case GL_INT_IMAGE_BUFFER: return clap::gl::shader::variable::type::detail::image_buffer<int>{};
		case GL_INT_IMAGE_1D_ARRAY: return clap::gl::shader::variable::type::detail::image_1d_array<int>{};
		case GL_INT_IMAGE_2D_ARRAY: return clap::gl::shader::variable::type::detail::image_2d_array<int>{};
		case GL_INT_IMAGE_2D_MULTISAMPLE: return clap::gl::shader::variable::type::detail::image_2d_multisample<int>{};
		case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return clap::gl::shader::variable::type::detail::image_2d_multisample_array<int>{};
		case GL_UNSIGNED_INT_IMAGE_1D: return clap::gl::shader::variable::type::detail::image_1d<unsigned>{};
		case GL_UNSIGNED_INT_IMAGE_2D: return clap::gl::shader::variable::type::detail::image_2d<unsigned>{};
		case GL_UNSIGNED_INT_IMAGE_3D: return clap::gl::shader::variable::type::detail::image_3d<unsigned>{};
		case GL_UNSIGNED_INT_IMAGE_2D_RECT: return clap::gl::shader::variable::type::detail::image_rect<unsigned>{};
		case GL_UNSIGNED_INT_IMAGE_CUBE: return clap::gl::shader::variable::type::detail::image_cube<unsigned>{};
		case GL_UNSIGNED_INT_IMAGE_BUFFER: return clap::gl::shader::variable::type::detail::image_buffer<unsigned>{};
		case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: return clap::gl::shader::variable::type::detail::image_1d_array<unsigned>{};
		case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: return clap::gl::shader::variable::type::detail::image_2d_array<unsigned>{};
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: return clap::gl::shader::variable::type::detail::image_2d_multisample<unsigned>{};
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return clap::gl::shader::variable::type::detail::image_2d_multisample_array<unsigned>{};
		case GL_UNSIGNED_INT_ATOMIC_COUNTER: return clap::gl::shader::variable::type::detail::atomic_counter<unsigned>{};
	}
	log::error::critical << "Unsupported enum value.";
}

std::ostream &operator<<(std::ostream &stream, clap::gl::shader::type type) {
	switch (type) {
		case clap::gl::shader::type::fragment:					stream << "fragment"; break;
		case clap::gl::shader::type::vertex:					stream << "vertex"; break;
		case clap::gl::shader::type::geometry:					stream << "geometry"; break;
		case clap::gl::shader::type::compute:					stream << "compute"; break;
		case clap::gl::shader::type::tesselation_control:		stream << "tesselation control"; break;
		case clap::gl::shader::type::tesselation_evaluation:	stream << "tesselation evaluation"; break;
		default:
			clap::log::warning::major << "Unsupported enum value.";
	}
	return stream;
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::texture::target v) {
	switch (v) {
		case clap::gl::texture::target::_1d: return GL_TEXTURE_1D;
		case clap::gl::texture::target::_2d: return GL_TEXTURE_2D;
		case clap::gl::texture::target::_3d: return GL_TEXTURE_3D;
		case clap::gl::texture::target::_1d_array: return GL_TEXTURE_1D_ARRAY;
		case clap::gl::texture::target::_2d_array: return GL_TEXTURE_2D_ARRAY;
		case clap::gl::texture::target::rectangle: return GL_TEXTURE_RECTANGLE;
		case clap::gl::texture::target::cube_map: return GL_TEXTURE_CUBE_MAP;
		case clap::gl::texture::target::cube_map_array: return GL_TEXTURE_CUBE_MAP_ARRAY;
		case clap::gl::texture::target::buffer: return GL_TEXTURE_BUFFER;
		case clap::gl::texture::target::multisample: return GL_TEXTURE_2D_MULTISAMPLE;
		case clap::gl::texture::target::multisample_array: return GL_TEXTURE_2D_MULTISAMPLE_ARRAY;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::texture::target clap::gl::detail::convert::to_texture_target(GLenum v) {
	switch (v) {
		case GL_TEXTURE_1D: return clap::gl::texture::target::_1d;
		case GL_TEXTURE_2D: return clap::gl::texture::target::_2d;
		case GL_TEXTURE_3D: return clap::gl::texture::target::_3d;
		case GL_TEXTURE_1D_ARRAY: return clap::gl::texture::target::_1d_array;
		case GL_TEXTURE_2D_ARRAY: return clap::gl::texture::target::_2d_array;
		case GL_TEXTURE_RECTANGLE: return clap::gl::texture::target::rectangle;
		case GL_TEXTURE_CUBE_MAP: return clap::gl::texture::target::cube_map;
		case GL_TEXTURE_CUBE_MAP_ARRAY: return clap::gl::texture::target::cube_map_array;
		case GL_TEXTURE_BUFFER: return clap::gl::texture::target::buffer;
		case GL_TEXTURE_2D_MULTISAMPLE: return clap::gl::texture::target::multisample;
		case GL_TEXTURE_2D_MULTISAMPLE_ARRAY: return clap::gl::texture::target::multisample_array;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::texture::internal_format v) {
	switch (v) {
		case texture::internal_format::depth_component: return GL_DEPTH_COMPONENT;
		case texture::internal_format::depth_stencil: return GL_DEPTH_STENCIL;
		case texture::internal_format::red: return GL_RED;
		case texture::internal_format::rg: return GL_RG;
		case texture::internal_format::rgb: return GL_RGB;
		case texture::internal_format::rgba: return GL_RGBA;
		case texture::internal_format::r8: return GL_R8;
		case texture::internal_format::r8_snorm: return GL_R8_SNORM;
		case texture::internal_format::r16: return GL_R16;
		case texture::internal_format::r16_snorm: return GL_R16_SNORM;
		case texture::internal_format::rg8: return GL_RG8;
		case texture::internal_format::rg8_snorm: return GL_RG8_SNORM;
		case texture::internal_format::rg16: return GL_RG16;
		case texture::internal_format::rg16_snorm: return GL_RG16_SNORM;
		case texture::internal_format::r3g3b2: return GL_R3_G3_B2;
		case texture::internal_format::rgb4: return GL_RGB4;
		case texture::internal_format::rgb5: return GL_RGB5;
		case texture::internal_format::rgb8: return GL_RGB8;
		case texture::internal_format::rgb8_snorm: return GL_RGB8_SNORM;
		case texture::internal_format::rgb10: return GL_RGB10;
		case texture::internal_format::rgb12: return GL_RGB12;
		case texture::internal_format::rgb16_snorm: return GL_RGB16_SNORM;
		case texture::internal_format::rgba2: return GL_RGBA2;
		case texture::internal_format::rgba4: return GL_RGBA4;
		case texture::internal_format::rgb5a1: return GL_RGB5_A1;
		case texture::internal_format::rgba8: return GL_RGBA8;
		case texture::internal_format::rgba8_snorm: return GL_RGBA8_SNORM;
		case texture::internal_format::rgb10a2: return GL_RGB10_A2;
		case texture::internal_format::rgb10a2ui: return GL_RGB10_A2UI;
		case texture::internal_format::rgba12: return GL_RGBA12;
		case texture::internal_format::rgba16: return GL_RGBA16;
		case texture::internal_format::srgb8: return GL_SRGB8;
		case texture::internal_format::srgba8: return GL_SRGB8_ALPHA8;
		case texture::internal_format::r16f: return GL_R16F;
		case texture::internal_format::rg16f: return GL_RG16F;
		case texture::internal_format::rgb16f: return GL_RGB16F;
		case texture::internal_format::rgba16f: return GL_RGBA16F;
		case texture::internal_format::r32f: return GL_R32F;
		case texture::internal_format::rg32f: return GL_RG32F;
		case texture::internal_format::rgb32f: return GL_RGB32F;
		case texture::internal_format::rgba32f: return GL_RGBA32F;
		case texture::internal_format::r11fg1fb10f: return GL_R11F_G11F_B10F;
		case texture::internal_format::rgb9e5: return GL_RGB9_E5;
		case texture::internal_format::r8i: return GL_R8I;
		case texture::internal_format::r8ui: return GL_R8UI;
		case texture::internal_format::r16i: return GL_R16I;
		case texture::internal_format::r16ui: return GL_R16UI;
		case texture::internal_format::r32i: return GL_R32I;
		case texture::internal_format::r32ui: return GL_R32UI;
		case texture::internal_format::rg8i: return GL_RG8I;
		case texture::internal_format::rg8ui: return GL_RG8UI;
		case texture::internal_format::rg16i: return GL_RG16I;
		case texture::internal_format::rg16ui: return GL_RG16UI;
		case texture::internal_format::rg32i: return GL_RG32I;
		case texture::internal_format::rg32ui: return GL_RG32UI;
		case texture::internal_format::rgb8i: return GL_RGB8I;
		case texture::internal_format::rgb8ui: return GL_RGB8UI;
		case texture::internal_format::rgb16i: return GL_RGB16I;
		case texture::internal_format::rgb16ui: return GL_RGB16UI;
		case texture::internal_format::rgb32i: return GL_RGB32I;
		case texture::internal_format::rgb32ui: return GL_RGB32UI;
		case texture::internal_format::rgba8i: return GL_RGBA8I;
		case texture::internal_format::rgba8ui: return GL_RGBA8UI;
		case texture::internal_format::rgba16i: return GL_RGBA16I;
		case texture::internal_format::rgba16ui: return GL_RGBA16UI;
		case texture::internal_format::rgba32i: return GL_RGBA32I;
		case texture::internal_format::rgba32ui: return GL_RGBA32UI;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::texture::internal_format clap::gl::detail::convert::to_internal_format(GLenum v) {
	switch (v) {
		case GL_DEPTH_COMPONENT: return texture::internal_format::depth_component;
		case GL_DEPTH_STENCIL: return texture::internal_format::depth_stencil;
		case GL_RED: return texture::internal_format::red;
		case GL_RG: return texture::internal_format::rg;
		case GL_RGB: return texture::internal_format::rgb;
		case GL_RGBA: return texture::internal_format::rgba;
		case GL_R8: return texture::internal_format::r8;
		case GL_R8_SNORM: return texture::internal_format::r8_snorm;
		case GL_R16: return texture::internal_format::r16;
		case GL_R16_SNORM: return texture::internal_format::r16_snorm;
		case GL_RG8: return texture::internal_format::rg8;
		case GL_RG8_SNORM: return texture::internal_format::rg8_snorm;
		case GL_RG16: return texture::internal_format::rg16;
		case GL_RG16_SNORM: return texture::internal_format::rg16_snorm;
		case GL_R3_G3_B2: return texture::internal_format::r3g3b2;
		case GL_RGB4: return texture::internal_format::rgb4;
		case GL_RGB5: return texture::internal_format::rgb5;
		case GL_RGB8: return texture::internal_format::rgb8;
		case GL_RGB8_SNORM: return texture::internal_format::rgb8_snorm;
		case GL_RGB10: return texture::internal_format::rgb10;
		case GL_RGB12: return texture::internal_format::rgb12;
		case GL_RGB16_SNORM: return texture::internal_format::rgb16_snorm;
		case GL_RGBA2: return texture::internal_format::rgba2;
		case GL_RGBA4: return texture::internal_format::rgba4;
		case GL_RGB5_A1: return texture::internal_format::rgb5a1;
		case GL_RGBA8: return texture::internal_format::rgba8;
		case GL_RGBA8_SNORM: return texture::internal_format::rgba8_snorm;
		case GL_RGB10_A2: return texture::internal_format::rgb10a2;
		case GL_RGB10_A2UI: return texture::internal_format::rgb10a2ui;
		case GL_RGBA12: return texture::internal_format::rgba12;
		case GL_RGBA16: return texture::internal_format::rgba16;
		case GL_SRGB8: return texture::internal_format::srgb8;
		case GL_SRGB8_ALPHA8: return texture::internal_format::srgba8;
		case GL_R16F: return texture::internal_format::r16f;
		case GL_RG16F: return texture::internal_format::rg16f;
		case GL_RGB16F: return texture::internal_format::rgb16f;
		case GL_RGBA16F: return texture::internal_format::rgba16f;
		case GL_R32F: return texture::internal_format::r32f;
		case GL_RG32F: return texture::internal_format::rg32f;
		case GL_RGB32F: return texture::internal_format::rgb32f;
		case GL_RGBA32F: return texture::internal_format::rgba32f;
		case GL_R11F_G11F_B10F: return texture::internal_format::r11fg1fb10f;
		case GL_RGB9_E5: return texture::internal_format::rgb9e5;
		case GL_R8I: return texture::internal_format::r8i;
		case GL_R8UI: return texture::internal_format::r8ui;
		case GL_R16I: return texture::internal_format::r16i;
		case GL_R16UI: return texture::internal_format::r16ui;
		case GL_R32I: return texture::internal_format::r32i;
		case GL_R32UI: return texture::internal_format::r32ui;
		case GL_RG8I: return texture::internal_format::rg8i;
		case GL_RG8UI: return texture::internal_format::rg8ui;
		case GL_RG16I: return texture::internal_format::rg16i;
		case GL_RG16UI: return texture::internal_format::rg16ui;
		case GL_RG32I: return texture::internal_format::rg32i;
		case GL_RG32UI: return texture::internal_format::rg32ui;
		case GL_RGB8I: return texture::internal_format::rgb8i;
		case GL_RGB8UI: return texture::internal_format::rgb8ui;
		case GL_RGB16I: return texture::internal_format::rgb16i;
		case GL_RGB16UI: return texture::internal_format::rgb16ui;
		case GL_RGB32I: return texture::internal_format::rgb32i;
		case GL_RGB32UI: return texture::internal_format::rgb32ui;
		case GL_RGBA8I: return texture::internal_format::rgba8i;
		case GL_RGBA8UI: return texture::internal_format::rgba8ui;
		case GL_RGBA16I: return texture::internal_format::rgba16i;
		case GL_RGBA16UI: return texture::internal_format::rgba16ui;
		case GL_RGBA32I: return texture::internal_format::rgba32i;
		case GL_RGBA32UI: return texture::internal_format::rgba32ui;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::texture::external_format v) {
	switch (v) {
		case texture::external_format::r: return GL_RED;
		case texture::external_format::rg: return GL_RG;
		case texture::external_format::rgb: return GL_RGB;
		case texture::external_format::bgr: return GL_BGR;
		case texture::external_format::rgba: return GL_RGBA;
		case texture::external_format::bgra: return GL_BGRA;
		case texture::external_format::ri: return GL_RED_INTEGER;
		case texture::external_format::rgi: return GL_RG_INTEGER;
		case texture::external_format::rgbi: return GL_RGB_INTEGER;
		case texture::external_format::bgri: return GL_BGR_INTEGER;
		case texture::external_format::rgbai: return GL_RGBA_INTEGER;
		case texture::external_format::bgrai: return GL_BGRA_INTEGER;
		case texture::external_format::stencil_index: return GL_STENCIL_INDEX;
		case texture::external_format::depth_component: return GL_DEPTH_COMPONENT;
		case texture::external_format::depth_stencil: return GL_DEPTH_STENCIL;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::texture::external_format clap::gl::detail::convert::to_external_format(GLenum v) {
	switch (v) {
		case GL_RED: return texture::external_format::r;
		case GL_RG: return texture::external_format::rg;
		case GL_RGB: return texture::external_format::rgb;
		case GL_BGR: return texture::external_format::bgr;
		case GL_RGBA: return texture::external_format::rgba;
		case GL_BGRA: return texture::external_format::bgra;
		case GL_RED_INTEGER: return texture::external_format::ri;
		case GL_RG_INTEGER: return texture::external_format::rgi;
		case GL_RGB_INTEGER: return texture::external_format::rgbi;
		case GL_BGR_INTEGER: return texture::external_format::bgri;
		case GL_RGBA_INTEGER: return texture::external_format::rgbai;
		case GL_BGRA_INTEGER: return texture::external_format::bgrai;
		case GL_STENCIL_INDEX: return texture::external_format::stencil_index;
		case GL_DEPTH_COMPONENT: return texture::external_format::depth_component;
		case GL_DEPTH_STENCIL: return texture::external_format::depth_stencil;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::texture::external_type v) {
	switch (v) {
		case texture::external_type::unsigned_byte: return GL_UNSIGNED_BYTE;
		case texture::external_type::byte: return GL_BYTE;
		case texture::external_type::unsigned_short: return GL_UNSIGNED_SHORT;
		case texture::external_type::_short: return GL_SHORT;
		case texture::external_type::unsinged_int: return GL_UNSIGNED_INT;
		case texture::external_type::_int: return GL_INT;
		case texture::external_type::half_float: return GL_HALF_FLOAT;
		case texture::external_type::_float: return GL_FLOAT;
		case texture::external_type::unsigned_byte_3_3_2: return GL_UNSIGNED_BYTE_3_3_2;
		case texture::external_type::unsigned_byte_2_3_3_r: return GL_UNSIGNED_BYTE_2_3_3_REV;
		case texture::external_type::unsigned_short_5_6_5: return GL_UNSIGNED_SHORT_5_6_5;
		case texture::external_type::unsigned_short_5_6_5_r: return GL_UNSIGNED_SHORT_5_6_5_REV;
		case texture::external_type::unsigned_short_4_4_4_4: return GL_UNSIGNED_SHORT_4_4_4_4;
		case texture::external_type::unsigned_short_4_4_4_4_r: return GL_UNSIGNED_SHORT_4_4_4_4_REV;
		case texture::external_type::unsigned_short_5_5_5_1: return GL_UNSIGNED_SHORT_5_5_5_1;
		case texture::external_type::unsigned_short_1_5_5_5_r: return GL_UNSIGNED_SHORT_1_5_5_5_REV;
		case texture::external_type::unsigned_int_8_8_8_8: return GL_UNSIGNED_INT_8_8_8_8;
		case texture::external_type::unsigned_int_8_8_8_8_r: return GL_UNSIGNED_INT_8_8_8_8_REV;
		case texture::external_type::unsigned_int_10_10_10_2: return GL_UNSIGNED_INT_10_10_10_2;
		case texture::external_type::unsigned_int_2_10_10_10_r: return GL_UNSIGNED_INT_2_10_10_10_REV;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::texture::external_type clap::gl::detail::convert::to_texture_type(GLenum v) {
	switch (v) {
		case GL_UNSIGNED_BYTE: return texture::external_type::unsigned_byte;
		case GL_BYTE: return texture::external_type::byte;
		case GL_UNSIGNED_SHORT: return texture::external_type::unsigned_short;
		case GL_SHORT: return texture::external_type::_short;
		case GL_UNSIGNED_INT: return texture::external_type::unsinged_int;
		case GL_INT: return texture::external_type::_int;
		case GL_HALF_FLOAT: return texture::external_type::half_float;
		case GL_FLOAT: return texture::external_type::_float;
		case GL_UNSIGNED_BYTE_3_3_2: return texture::external_type::unsigned_byte_3_3_2;
		case GL_UNSIGNED_BYTE_2_3_3_REV: return texture::external_type::unsigned_byte_2_3_3_r;
		case GL_UNSIGNED_SHORT_5_6_5: return texture::external_type::unsigned_short_5_6_5;
		case GL_UNSIGNED_SHORT_5_6_5_REV: return texture::external_type::unsigned_short_5_6_5_r;
		case GL_UNSIGNED_SHORT_4_4_4_4: return texture::external_type::unsigned_short_4_4_4_4;
		case GL_UNSIGNED_SHORT_4_4_4_4_REV: return texture::external_type::unsigned_short_4_4_4_4_r;
		case GL_UNSIGNED_SHORT_5_5_5_1: return texture::external_type::unsigned_short_5_5_5_1;
		case GL_UNSIGNED_SHORT_1_5_5_5_REV: return texture::external_type::unsigned_short_1_5_5_5_r;
		case GL_UNSIGNED_INT_8_8_8_8: return texture::external_type::unsigned_int_8_8_8_8;
		case GL_UNSIGNED_INT_8_8_8_8_REV: return texture::external_type::unsigned_int_8_8_8_8_r;
		case GL_UNSIGNED_INT_10_10_10_2: return texture::external_type::unsigned_int_10_10_10_2;
		case GL_UNSIGNED_INT_2_10_10_10_REV: return texture::external_type::unsigned_int_2_10_10_10_r;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::texture::depth_stencil_texture_mode v) {
	switch (v) {
		case texture::depth_stencil_texture_mode::depth_component: return GL_DEPTH_COMPONENT;
		case texture::depth_stencil_texture_mode::stencil_index: return GL_STENCIL_INDEX;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::texture::depth_stencil_texture_mode clap::gl::detail::convert::to_depth_stencil_texture_mode(GLenum v) {
	switch (v) {
		case GL_DEPTH_COMPONENT: return texture::depth_stencil_texture_mode::depth_component;
		case GL_STENCIL_INDEX: return texture::depth_stencil_texture_mode::stencil_index;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::texture::min_filter v) {
	switch (v) {
		case texture::min_filter::nearest: return GL_NEAREST;
		case texture::min_filter::linear: return GL_LINEAR;
		case texture::min_filter::nearest_mipmap_nearest: return GL_NEAREST_MIPMAP_NEAREST;
		case texture::min_filter::linear_mipmap_nearest: return GL_LINEAR_MIPMAP_NEAREST;
		case texture::min_filter::nearest_mipmap_linear: return GL_NEAREST_MIPMAP_LINEAR;
		case texture::min_filter::linear_mipmap_linear: return GL_LINEAR_MIPMAP_LINEAR;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::texture::min_filter clap::gl::detail::convert::to_min_filter(GLenum v) {
	switch (v) {
		case GL_NEAREST: return texture::min_filter::nearest;
		case GL_LINEAR: return texture::min_filter::linear;
		case GL_NEAREST_MIPMAP_NEAREST: return texture::min_filter::nearest_mipmap_nearest;
		case GL_LINEAR_MIPMAP_NEAREST: return texture::min_filter::linear_mipmap_nearest;
		case GL_NEAREST_MIPMAP_LINEAR: return texture::min_filter::nearest_mipmap_linear;
		case GL_LINEAR_MIPMAP_LINEAR: return texture::min_filter::linear_mipmap_linear;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::texture::mag_filter v) {
	switch (v) {
		case texture::mag_filter::nearest: return GL_NEAREST;
		case texture::mag_filter::linear: return GL_LINEAR;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::texture::mag_filter clap::gl::detail::convert::to_mag_filter(GLenum v) {
	switch (v) {
		case GL_NEAREST: return texture::mag_filter::nearest;
		case GL_LINEAR: return texture::mag_filter::linear;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::texture::wrap v) {
	switch (v) {
		case texture::wrap::clamp_to_edge: return GL_CLAMP_TO_EDGE;
		case texture::wrap::clamp_to_border: return GL_CLAMP_TO_BORDER;
		case texture::wrap::mirrored_repeat: return GL_MIRRORED_REPEAT;
		case texture::wrap::repeat: return GL_REPEAT;
		case texture::wrap::mirror_clamp_to_edge: return GL_MIRROR_CLAMP_TO_EDGE;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::texture::wrap clap::gl::detail::convert::to_wrap(GLenum v) {
	switch (v) {
		case GL_CLAMP_TO_EDGE: return texture::wrap::clamp_to_edge;
		case GL_CLAMP_TO_BORDER: return texture::wrap::clamp_to_border;
		case GL_MIRRORED_REPEAT: return texture::wrap::mirrored_repeat;
		case GL_REPEAT: return texture::wrap::repeat;
		case GL_MIRROR_CLAMP_TO_EDGE: return texture::wrap::mirror_clamp_to_edge;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

std::ostream &operator<<(std::ostream &stream, clap::gl::texture::target target) {
	switch (target) {
		case clap::gl::texture::target::_1d:
			stream << "texture::target::_1d"; break;
		case clap::gl::texture::target::_2d:
			stream << "texture::target::_2d"; break;
		case clap::gl::texture::target::_3d:
			stream << "texture::target::_3d"; break;
		case clap::gl::texture::target::_1d_array:
			stream << "texture::target::_1d_array"; break;
		case clap::gl::texture::target::_2d_array:
			stream << "texture::target::_2d_array"; break;
		case clap::gl::texture::target::rectangle:
			stream << "texture::target::rectangle"; break;
		case clap::gl::texture::target::cube_map:
			stream << "texture::target::cube_map"; break;
		case clap::gl::texture::target::cube_map_array:
			stream << "texture::target::cube_map_array"; break;
		case clap::gl::texture::target::buffer:
			stream << "texture::target::buffer"; break;
		case clap::gl::texture::target::multisample:
			stream << "texture::target::multisample"; break;
		case clap::gl::texture::target::multisample_array:
			stream << "texture::target::multisample_array"; break;
		default:
			clap::log::warning::major << "Unsupported enum value.";
	}
	return stream;
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::vertex_array::connection v) {
	switch (v) {
		case clap::gl::vertex_array::connection::points: return GL_POINTS;
		case clap::gl::vertex_array::connection::lines: return GL_LINES;
		case clap::gl::vertex_array::connection::lines_w_adjacency: return GL_LINES_ADJACENCY;
		case clap::gl::vertex_array::connection::line_strip: return GL_LINE_STRIP;
		case clap::gl::vertex_array::connection::line_strip_w_adjacency: return GL_LINE_STRIP_ADJACENCY;
		case clap::gl::vertex_array::connection::line_loop: return GL_LINE_LOOP;
		case clap::gl::vertex_array::connection::triangles: return GL_TRIANGLES;
		case clap::gl::vertex_array::connection::triangles_w_adjacency: return GL_TRIANGLES_ADJACENCY;
		case clap::gl::vertex_array::connection::triangle_strip: return GL_TRIANGLE_STRIP;
		case clap::gl::vertex_array::connection::triangle_strip_w_adjacency: return GL_TRIANGLE_STRIP_ADJACENCY;;
		case clap::gl::vertex_array::connection::triangle_fan: return GL_TRIANGLE_FAN;
		case clap::gl::vertex_array::connection::patches: return GL_PATCHES;
	}
	log::error::critical << "Unsupported enum value.";
}

clap::gl::vertex_array::connection clap::gl::detail::convert::to_connection_type(GLenum v) {
	switch (v) {
		case GL_POINTS:						  return clap::gl::vertex_array::connection::points;
		case GL_LINES:						  return clap::gl::vertex_array::connection::lines;
		case GL_LINES_ADJACENCY:			  return clap::gl::vertex_array::connection::lines_w_adjacency;
		case GL_LINE_STRIP:					  return clap::gl::vertex_array::connection::line_strip;
		case GL_LINE_STRIP_ADJACENCY:		  return clap::gl::vertex_array::connection::line_strip_w_adjacency;
		case GL_LINE_LOOP:					  return clap::gl::vertex_array::connection::line_loop;
		case GL_TRIANGLES:					  return clap::gl::vertex_array::connection::triangles;
		case GL_TRIANGLES_ADJACENCY:		  return clap::gl::vertex_array::connection::triangles_w_adjacency;
		case GL_TRIANGLE_STRIP:				  return clap::gl::vertex_array::connection::triangle_strip;
		case GL_TRIANGLE_STRIP_ADJACENCY:	  return clap::gl::vertex_array::connection::triangle_strip_w_adjacency;
		case GL_TRIANGLE_FAN:				  return clap::gl::vertex_array::connection::triangle_fan;
		case GL_PATCHES:					  return clap::gl::vertex_array::connection::patches;
	}
	log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::vertex_array::index_type v) {
	switch (v) {
		case clap::gl::vertex_array::index_type::unsigned_byte: return GL_UNSIGNED_BYTE;
		case clap::gl::vertex_array::index_type::unsigned_short: return GL_UNSIGNED_SHORT;
		case clap::gl::vertex_array::index_type::unsigned_int: return GL_UNSIGNED_INT;
	}
	log::error::critical << "Unsupported enum value.";
}

clap::gl::vertex_array::index_type clap::gl::detail::convert::to_index_type(GLenum v) {
	switch (v) {
		case GL_UNSIGNED_BYTE: return clap::gl::vertex_array::index_type::unsigned_byte;
		case GL_UNSIGNED_SHORT: return clap::gl::vertex_array::index_type::unsigned_short;
		case GL_UNSIGNED_INT: return clap::gl::vertex_array::index_type::unsigned_int;
	}
	log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::detail::blend_function v) {
	switch (v) {
		case clap::gl::detail::blend_function::zero: return GL_ZERO;
		case clap::gl::detail::blend_function::one: return GL_ONE;
		case clap::gl::detail::blend_function::source_color: return GL_SRC_COLOR;
		case clap::gl::detail::blend_function::one_minus_source_color: return GL_ONE_MINUS_SRC_COLOR;
		case clap::gl::detail::blend_function::destination_color: return GL_DST_COLOR;
		case clap::gl::detail::blend_function::one_minus_destination_color: return GL_ONE_MINUS_DST_COLOR;
		case clap::gl::detail::blend_function::source_alpha: return GL_SRC_ALPHA;
		case clap::gl::detail::blend_function::one_minus_source_alpha: return GL_ONE_MINUS_SRC_ALPHA;
		case clap::gl::detail::blend_function::destination_alpha: return GL_DST_ALPHA;
		case clap::gl::detail::blend_function::one_minus_destination_alpha: return GL_ONE_MINUS_DST_ALPHA;
		case clap::gl::detail::blend_function::constant_color: return GL_CONSTANT_COLOR;
		case clap::gl::detail::blend_function::one_minus_constant_color: return GL_ONE_MINUS_CONSTANT_COLOR;
		case clap::gl::detail::blend_function::constant_alpha: return GL_CONSTANT_ALPHA;
		case clap::gl::detail::blend_function::one_minus_constant_alpha: return GL_ONE_MINUS_CONSTANT_ALPHA;
		case clap::gl::detail::blend_function::source_alpha_saturate: return GL_SRC_ALPHA_SATURATE;
		case clap::gl::detail::blend_function::source_1_color: return GL_SRC1_COLOR;
		case clap::gl::detail::blend_function::one_minus_source_1_color: return GL_ONE_MINUS_SRC1_COLOR;
		case clap::gl::detail::blend_function::source_1_alpha: return GL_SRC1_ALPHA;
		case clap::gl::detail::blend_function::one_minus_source_1_alpha: return GL_ONE_MINUS_SRC1_ALPHA;
	}
	log::error::critical << "Unsupported enum value.";
}
clap::gl::detail::blend_function clap::gl::detail::convert::to_blend_function(GLenum v) {
	switch (v) {
		case GL_ZERO: return clap::gl::detail::blend_function::zero;
		case GL_ONE: return clap::gl::detail::blend_function::one;
		case GL_SRC_COLOR: return clap::gl::detail::blend_function::source_color;
		case GL_ONE_MINUS_SRC_COLOR: return clap::gl::detail::blend_function::one_minus_source_color;
		case GL_DST_COLOR: return clap::gl::detail::blend_function::destination_color;
		case GL_ONE_MINUS_DST_COLOR: return clap::gl::detail::blend_function::one_minus_destination_color;
		case GL_SRC_ALPHA: return clap::gl::detail::blend_function::source_alpha;
		case GL_ONE_MINUS_SRC_ALPHA: return clap::gl::detail::blend_function::one_minus_source_alpha;
		case GL_DST_ALPHA: return clap::gl::detail::blend_function::destination_alpha;
		case GL_ONE_MINUS_DST_ALPHA: return clap::gl::detail::blend_function::one_minus_destination_alpha;
		case GL_CONSTANT_COLOR: return clap::gl::detail::blend_function::constant_color;
		case GL_ONE_MINUS_CONSTANT_COLOR: return clap::gl::detail::blend_function::one_minus_constant_color;
		case GL_CONSTANT_ALPHA: return clap::gl::detail::blend_function::constant_alpha;
		case GL_ONE_MINUS_CONSTANT_ALPHA: return clap::gl::detail::blend_function::one_minus_constant_alpha;
		case GL_SRC_ALPHA_SATURATE: return clap::gl::detail::blend_function::source_alpha_saturate;
		case GL_SRC1_COLOR: return clap::gl::detail::blend_function::source_1_color;
		case GL_ONE_MINUS_SRC1_COLOR: return clap::gl::detail::blend_function::one_minus_source_1_color;
		case GL_SRC1_ALPHA: return clap::gl::detail::blend_function::source_1_alpha;
		case GL_ONE_MINUS_SRC1_ALPHA: return clap::gl::detail::blend_function::one_minus_source_1_alpha;
	}
	log::error::critical << "Unsupported enum value.";
}
std::ostream &operator<<(std::ostream &stream, clap::gl::detail::blend_function function) {
	switch (function) {
		case clap::gl::detail::blend_function::zero:
			stream << "0"; break;
		case clap::gl::detail::blend_function::one:
			stream << "1"; break;
		case clap::gl::detail::blend_function::source_color:
			stream << "source_color"; break;
		case clap::gl::detail::blend_function::one_minus_source_color:
			stream << "1 - source_color"; break;
		case clap::gl::detail::blend_function::destination_color:
			stream << "destination_color"; break;
		case clap::gl::detail::blend_function::one_minus_destination_color:
			stream << "1 - destination_color"; break;
		case clap::gl::detail::blend_function::source_alpha:
			stream << "source_alpha"; break;
		case clap::gl::detail::blend_function::one_minus_source_alpha:
			stream << "1 - source_alpha"; break;
		case clap::gl::detail::blend_function::destination_alpha:
			stream << "destination_alpha"; break;
		case clap::gl::detail::blend_function::one_minus_destination_alpha:
			stream << "1 - destination_alpha"; break;
		case clap::gl::detail::blend_function::constant_color:
			stream << "constant_color"; break;
		case clap::gl::detail::blend_function::one_minus_constant_color:
			stream << "1 - constant_color"; break;
		case clap::gl::detail::blend_function::constant_alpha:
			stream << "constant_alpha"; break;
		case clap::gl::detail::blend_function::one_minus_constant_alpha:
			stream << "1 - constant_alpha"; break;
		case clap::gl::detail::blend_function::source_alpha_saturate:
			stream << "source_alpha_saturate"; break;
		case clap::gl::detail::blend_function::source_1_color:
			stream << "source_1_color"; break;
		case clap::gl::detail::blend_function::one_minus_source_1_color:
			stream << "1 - source_1_color"; break;
		case clap::gl::detail::blend_function::source_1_alpha:
			stream << "source_1_alpha"; break;
		case clap::gl::detail::blend_function::one_minus_source_1_alpha:
			stream << "1 - source_1_alpha"; break;
		default:
			clap::log::warning::major << "Unsupported enum value.";
	}
	return stream;
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::face v) {
	switch (v) {
		case clap::gl::face::back: return GL_BACK;
		case clap::gl::face::both: return GL_FRONT_AND_BACK;
		case clap::gl::face::front: return GL_FRONT;
	}
	log::error::critical << "Unsupported enum value.";
}
clap::gl::face clap::gl::detail::convert::to_face(GLenum v) {
	switch (v) {
		case GL_BACK: return clap::gl::face::back;
		case GL_FRONT_AND_BACK: return clap::gl::face::both;
		case GL_FRONT: return clap::gl::face::front;
	}
	log::error::critical << "Unsupported enum value.";
}
std::ostream &operator<<(std::ostream &stream, clap::gl::face function) {
	switch (function) {
		case clap::gl::face::back:
			stream << "back faces"; break;
		case clap::gl::face::both:
			stream << "both front and back faces"; break;
		case clap::gl::face::front:
			stream << "front faces"; break;
		default:
			clap::log::warning::major << "Unsupported enum value.";
	}
	return stream;
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::depth_function v) {
	switch (v) {
		case clap::gl::depth_function::never: return GL_NEVER;
		case clap::gl::depth_function::less: return GL_LESS;
		case clap::gl::depth_function::equal: return GL_EQUAL;
		case clap::gl::depth_function::less_or_equal: return GL_LEQUAL;
		case clap::gl::depth_function::greater: return GL_GREATER;
		case clap::gl::depth_function::not_equal: return GL_NOTEQUAL;
		case clap::gl::depth_function::greater_or_equal: return GL_GEQUAL;
		case clap::gl::depth_function::always: return GL_ALWAYS;
	}
	log::error::critical << "Unsupported enum value.";
}
clap::gl::depth_function clap::gl::detail::convert::to_depth_function(GLenum v) {
	switch (v) {
		case GL_NEVER: return clap::gl::depth_function::never;
		case GL_LESS: return clap::gl::depth_function::less;
		case GL_EQUAL: return clap::gl::depth_function::equal;
		case GL_LEQUAL: return clap::gl::depth_function::less_or_equal;
		case GL_GREATER: return clap::gl::depth_function::greater;
		case GL_NOTEQUAL: return clap::gl::depth_function::not_equal;
		case GL_GEQUAL: return clap::gl::depth_function::greater_or_equal;
		case GL_ALWAYS: return clap::gl::depth_function::always;
	}
	log::error::critical << "Unsupported enum value.";
}
std::ostream &operator<<(std::ostream &stream, clap::gl::depth_function function) {
	switch (function) {
		case clap::gl::depth_function::never:
			stream << "never be passed"; break;
		case clap::gl::depth_function::less:
			stream << "be passed if new value is less than the current one"; break;
		case clap::gl::depth_function::equal:
			stream << "be passed if new value is equal to the current one"; break;
		case clap::gl::depth_function::less_or_equal:
			stream << "be passed if new value is less or equal than the current one"; break;
		case clap::gl::depth_function::greater:
			stream << "be passed if new value is greater than the current one"; break;
		case clap::gl::depth_function::not_equal:
			stream << "be passed if new value is not equal to the current one"; break;
		case clap::gl::depth_function::greater_or_equal:
			stream << "be passed if new value is greater or equal than the current one"; break;
		case clap::gl::depth_function::always:
			stream << "always be passed"; break;
		default:
			clap::log::warning::major << "Unsupported enum value.";
	}
	return stream;
}

#pragma warning(pop)
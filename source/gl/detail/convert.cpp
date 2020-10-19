#include "essential/log.hpp"

#include "gl/misc.hpp"
#include "gl/shader.hpp"
#include "gl/texture.hpp"
#include "gl/vertex.hpp"
#include "gl/detail/window.hpp"

#include "glad/glad.h"
#include "glfw/glfw3.h"

#pragma warning(push)
#pragma warning(disable : 4715)

GLenum clap::gl::detail::convert::to_gl(clap::gl::vertex::buffer_target v) {
	switch (v) {
		case clap::gl::vertex::buffer_target::array: return GL_ARRAY_BUFFER;
		case clap::gl::vertex::buffer_target::atomic_counter: return GL_ATOMIC_COUNTER_BUFFER;
		case clap::gl::vertex::buffer_target::copy_read: return GL_COPY_READ_BUFFER;
		case clap::gl::vertex::buffer_target::copy_write: return GL_COPY_WRITE_BUFFER;
		case clap::gl::vertex::buffer_target::indirect_dispatch: return GL_DISPATCH_INDIRECT_BUFFER;
		case clap::gl::vertex::buffer_target::indirect_draw: return GL_DRAW_INDIRECT_BUFFER;
		case clap::gl::vertex::buffer_target::element_array: return GL_ELEMENT_ARRAY_BUFFER;
		case clap::gl::vertex::buffer_target::pixel_pack: return GL_PIXEL_PACK_BUFFER;
		case clap::gl::vertex::buffer_target::pixel_unpack: return GL_PIXEL_UNPACK_BUFFER;
		case clap::gl::vertex::buffer_target::query: return GL_QUERY_BUFFER;
		case clap::gl::vertex::buffer_target::shader_storage: return GL_SHADER_STORAGE_BUFFER;
		case clap::gl::vertex::buffer_target::texture: return GL_TEXTURE_BUFFER;
		case clap::gl::vertex::buffer_target::transform_feedback: return GL_TRANSFORM_FEEDBACK_BUFFER;
		case clap::gl::vertex::buffer_target::uniform: return GL_UNIFORM_BUFFER;
	}
	log::error::critical << "Unsupported enum value.";
}
clap::gl::vertex::buffer_target clap::gl::detail::convert::to_buffer_target(GLenum v) {
	switch (v) {
		case GL_ARRAY_BUFFER: return clap::gl::vertex::buffer_target::array;
		case GL_ATOMIC_COUNTER_BUFFER: return clap::gl::vertex::buffer_target::atomic_counter;
		case GL_COPY_READ_BUFFER: return clap::gl::vertex::buffer_target::copy_read;
		case GL_COPY_WRITE_BUFFER: return clap::gl::vertex::buffer_target::copy_write;
		case GL_DISPATCH_INDIRECT_BUFFER: return clap::gl::vertex::buffer_target::indirect_dispatch;
		case GL_DRAW_INDIRECT_BUFFER: return clap::gl::vertex::buffer_target::indirect_draw;
		case GL_ELEMENT_ARRAY_BUFFER: return clap::gl::vertex::buffer_target::element_array;
		case GL_PIXEL_PACK_BUFFER: return clap::gl::vertex::buffer_target::pixel_pack;
		case GL_PIXEL_UNPACK_BUFFER: return clap::gl::vertex::buffer_target::pixel_unpack;
		case GL_QUERY_BUFFER: return clap::gl::vertex::buffer_target::query;
		case GL_SHADER_STORAGE_BUFFER: return clap::gl::vertex::buffer_target::shader_storage;
		case GL_TEXTURE_BUFFER: return clap::gl::vertex::buffer_target::texture;
		case GL_TRANSFORM_FEEDBACK_BUFFER: return clap::gl::vertex::buffer_target::transform_feedback;
		case GL_UNIFORM_BUFFER: return clap::gl::vertex::buffer_target::uniform;
	}
	log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::vertex::buffer::access v) {
	switch (v) {
		case clap::gl::vertex::buffer::access::read_only: return GL_READ_ONLY;
		case clap::gl::vertex::buffer::access::read_write: return GL_READ_WRITE;
		case clap::gl::vertex::buffer::access::write_only: return GL_WRITE_ONLY;
	}
	log::error::critical << "Unsupported enum value.";
}
clap::gl::vertex::buffer::access clap::gl::detail::convert::to_access(GLenum v) {
	switch (v) {
		case GL_READ_ONLY: return clap::gl::vertex::buffer::access::read_only;
		case GL_READ_WRITE: return clap::gl::vertex::buffer::access::read_write;
		case GL_WRITE_ONLY: return clap::gl::vertex::buffer::access::write_only;
	}
	log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::vertex::buffer::usage v) {
	switch (v) {
		case clap::gl::vertex::buffer::usage::stream_draw: return GL_STREAM_DRAW;
		case clap::gl::vertex::buffer::usage::stream_read: return GL_STREAM_READ;
		case clap::gl::vertex::buffer::usage::stream_copy: return GL_STREAM_COPY;
		case clap::gl::vertex::buffer::usage::static_draw: return GL_STATIC_DRAW;
		case clap::gl::vertex::buffer::usage::static_read: return GL_STATIC_READ;
		case clap::gl::vertex::buffer::usage::static_copy: return GL_STATIC_COPY;
		case clap::gl::vertex::buffer::usage::dynamic_draw: return GL_DYNAMIC_DRAW;
		case clap::gl::vertex::buffer::usage::dynamic_read: return GL_DYNAMIC_READ;
		case clap::gl::vertex::buffer::usage::dynamic_copy: return GL_DYNAMIC_COPY;
	}
	log::error::critical << "Unsupported enum value.";
}
clap::gl::vertex::buffer::usage clap::gl::detail::convert::to_usage(GLenum v) {
	switch (v) {
		case GL_STREAM_DRAW: return clap::gl::vertex::buffer::usage::stream_draw;
		case GL_STREAM_READ: return clap::gl::vertex::buffer::usage::stream_read;
		case GL_STREAM_COPY: return clap::gl::vertex::buffer::usage::stream_copy;
		case GL_STATIC_DRAW: return clap::gl::vertex::buffer::usage::static_draw;
		case GL_STATIC_READ: return clap::gl::vertex::buffer::usage::static_read;
		case GL_STATIC_COPY: return clap::gl::vertex::buffer::usage::static_copy;
		case GL_DYNAMIC_DRAW: return clap::gl::vertex::buffer::usage::dynamic_draw;
		case GL_DYNAMIC_READ: return clap::gl::vertex::buffer::usage::dynamic_read;
		case GL_DYNAMIC_COPY: return clap::gl::vertex::buffer::usage::dynamic_copy;
	}
	log::error::critical << "Unsupported enum value.";
}

std::ostream &operator<<(std::ostream &stream, clap::gl::vertex::buffer_target target) {
	switch (target) {
		case clap::gl::vertex::buffer_target::array:
			stream << "vertex::buffer_target::array"; break;
		case clap::gl::vertex::buffer_target::atomic_counter:
			stream << "vertex::buffer_target::atomic_counter"; break;
		case clap::gl::vertex::buffer_target::copy_read:
			stream << "vertex::buffer_target::copy_read"; break;
		case clap::gl::vertex::buffer_target::copy_write:
			stream << "vertex::buffer_target::copy_write"; break;
		case clap::gl::vertex::buffer_target::indirect_dispatch:
			stream << "vertex::buffer_target::indirect_dispatch"; break;
		case clap::gl::vertex::buffer_target::indirect_draw:
			stream << "vertex::buffer_target::indirect_draw"; break;
		case clap::gl::vertex::buffer_target::element_array:
			stream << "vertex::buffer_target::element_array"; break;
		case clap::gl::vertex::buffer_target::pixel_pack:
			stream << "vertex::buffer_target::pixel_pack"; break;
		case clap::gl::vertex::buffer_target::pixel_unpack:
			stream << "vertex::buffer_target::pixel_unpack"; break;
		case clap::gl::vertex::buffer_target::query:
			stream << "vertex::buffer_target::query"; break;
		case clap::gl::vertex::buffer_target::shader_storage:
			stream << "vertex::buffer_target::shader_storage"; break;
		case clap::gl::vertex::buffer_target::texture:
			stream << "vertex::buffer_target::texture"; break;
		case clap::gl::vertex::buffer_target::transform_feedback:
			stream << "vertex::buffer_target::transform_feedback"; break;
		case clap::gl::vertex::buffer_target::uniform:
			stream << "vertex::buffer_target::uniform"; break;
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

using underlying = std::underlying_type_t<clap::gl::detail::window::event::modificator_mask>;
clap::gl::detail::window::event::modificator_mask operator|(clap::gl::detail::window::event::modificator_mask lhs, clap::gl::detail::window::event::modificator_mask rhs) {
	return static_cast<clap::gl::detail::window::event::modificator_mask>(static_cast<underlying>(lhs) | static_cast<underlying>(rhs));
}
clap::gl::detail::window::event::modificator_mask operator|=(clap::gl::detail::window::event::modificator_mask &lhs, clap::gl::detail::window::event::modificator_mask rhs) {
	return lhs = lhs | rhs;
}
clap::gl::detail::window::event::modificator_mask operator&(clap::gl::detail::window::event::modificator_mask lhs, clap::gl::detail::window::event::modificator_mask rhs) {
	return static_cast<clap::gl::detail::window::event::modificator_mask>(static_cast<underlying>(lhs) & static_cast<underlying>(rhs));
}
clap::gl::detail::window::event::modificator_mask operator&=(clap::gl::detail::window::event::modificator_mask &lhs, clap::gl::detail::window::event::modificator_mask rhs) {
	return lhs = lhs & rhs;
}
clap::gl::detail::window::event::modificator_mask operator^(clap::gl::detail::window::event::modificator_mask lhs, clap::gl::detail::window::event::modificator_mask rhs) {
	return static_cast<clap::gl::detail::window::event::modificator_mask>(static_cast<underlying>(lhs) ^ static_cast<underlying>(rhs));
}
clap::gl::detail::window::event::modificator_mask operator^=(clap::gl::detail::window::event::modificator_mask &lhs, clap::gl::detail::window::event::modificator_mask rhs) {
	return lhs = lhs ^ rhs;
}


int clap::gl::detail::convert::to_glfw(clap::gl::detail::window::event::key v) {
	switch (v) {
		case clap::gl::detail::window::event::key::unknown: return GLFW_KEY_UNKNOWN;
		case clap::gl::detail::window::event::key::space: return GLFW_KEY_SPACE;
		case clap::gl::detail::window::event::key::apostrophe: return GLFW_KEY_APOSTROPHE;
		case clap::gl::detail::window::event::key::comma: return GLFW_KEY_COMMA;
		case clap::gl::detail::window::event::key::minus: return GLFW_KEY_MINUS;
		case clap::gl::detail::window::event::key::period: return GLFW_KEY_PERIOD;
		case clap::gl::detail::window::event::key::slash: return GLFW_KEY_SLASH;
		case clap::gl::detail::window::event::key::n_0: return GLFW_KEY_0;
		case clap::gl::detail::window::event::key::n_1: return GLFW_KEY_1;
		case clap::gl::detail::window::event::key::n_2: return GLFW_KEY_2;
		case clap::gl::detail::window::event::key::n_3: return GLFW_KEY_3;
		case clap::gl::detail::window::event::key::n_4: return GLFW_KEY_4;
		case clap::gl::detail::window::event::key::n_5: return GLFW_KEY_5;
		case clap::gl::detail::window::event::key::n_6: return GLFW_KEY_6;
		case clap::gl::detail::window::event::key::n_7: return GLFW_KEY_7;
		case clap::gl::detail::window::event::key::n_8: return GLFW_KEY_8;
		case clap::gl::detail::window::event::key::n_9: return GLFW_KEY_9;
		case clap::gl::detail::window::event::key::semicolon: return GLFW_KEY_SEMICOLON;
		case clap::gl::detail::window::event::key::equal: return GLFW_KEY_EQUAL;
		case clap::gl::detail::window::event::key::a: return GLFW_KEY_A;
		case clap::gl::detail::window::event::key::b: return GLFW_KEY_B;
		case clap::gl::detail::window::event::key::c: return GLFW_KEY_C;
		case clap::gl::detail::window::event::key::d: return GLFW_KEY_D;
		case clap::gl::detail::window::event::key::e: return GLFW_KEY_E;
		case clap::gl::detail::window::event::key::f: return GLFW_KEY_F;
		case clap::gl::detail::window::event::key::g: return GLFW_KEY_G;
		case clap::gl::detail::window::event::key::h: return GLFW_KEY_H;
		case clap::gl::detail::window::event::key::i: return GLFW_KEY_I;
		case clap::gl::detail::window::event::key::j: return GLFW_KEY_J;
		case clap::gl::detail::window::event::key::k: return GLFW_KEY_K;
		case clap::gl::detail::window::event::key::l: return GLFW_KEY_L;
		case clap::gl::detail::window::event::key::m: return GLFW_KEY_M;
		case clap::gl::detail::window::event::key::n: return GLFW_KEY_N;
		case clap::gl::detail::window::event::key::o: return GLFW_KEY_O;
		case clap::gl::detail::window::event::key::p: return GLFW_KEY_P;
		case clap::gl::detail::window::event::key::q: return GLFW_KEY_Q;
		case clap::gl::detail::window::event::key::r: return GLFW_KEY_R;
		case clap::gl::detail::window::event::key::s: return GLFW_KEY_S;
		case clap::gl::detail::window::event::key::t: return GLFW_KEY_T;
		case clap::gl::detail::window::event::key::u: return GLFW_KEY_U;
		case clap::gl::detail::window::event::key::v: return GLFW_KEY_V;
		case clap::gl::detail::window::event::key::w: return GLFW_KEY_W;
		case clap::gl::detail::window::event::key::x: return GLFW_KEY_X;
		case clap::gl::detail::window::event::key::y: return GLFW_KEY_Y;
		case clap::gl::detail::window::event::key::z: return GLFW_KEY_Z;
		case clap::gl::detail::window::event::key::bracket_left: return GLFW_KEY_LEFT_BRACKET;
		case clap::gl::detail::window::event::key::backslash: return GLFW_KEY_BACKSLASH;
		case clap::gl::detail::window::event::key::bracket_right: return GLFW_KEY_RIGHT_BRACKET;
		case clap::gl::detail::window::event::key::grave_accent: return GLFW_KEY_GRAVE_ACCENT;
		case clap::gl::detail::window::event::key::world_1: return GLFW_KEY_WORLD_1;
		case clap::gl::detail::window::event::key::world_2: return GLFW_KEY_WORLD_2;
		case clap::gl::detail::window::event::key::escape: return GLFW_KEY_ESCAPE;
		case clap::gl::detail::window::event::key::enter: return GLFW_KEY_ENTER;
		case clap::gl::detail::window::event::key::tab: return GLFW_KEY_TAB;
		case clap::gl::detail::window::event::key::backspace: return GLFW_KEY_BACKSPACE;
		case clap::gl::detail::window::event::key::insert: return GLFW_KEY_INSERT;
		case clap::gl::detail::window::event::key::delete_: return GLFW_KEY_DELETE;
		case clap::gl::detail::window::event::key::right: return GLFW_KEY_RIGHT;
		case clap::gl::detail::window::event::key::left: return GLFW_KEY_LEFT;
		case clap::gl::detail::window::event::key::down: return GLFW_KEY_DOWN;
		case clap::gl::detail::window::event::key::up: return GLFW_KEY_UP;
		case clap::gl::detail::window::event::key::page_up: return GLFW_KEY_PAGE_UP;
		case clap::gl::detail::window::event::key::page_down: return GLFW_KEY_PAGE_DOWN;
		case clap::gl::detail::window::event::key::home: return GLFW_KEY_HOME;
		case clap::gl::detail::window::event::key::end: return GLFW_KEY_END;
		case clap::gl::detail::window::event::key::caps_lock: return GLFW_KEY_CAPS_LOCK;
		case clap::gl::detail::window::event::key::scroll_lock: return GLFW_KEY_SCROLL_LOCK;
		case clap::gl::detail::window::event::key::num_lock: return GLFW_KEY_NUM_LOCK;
		case clap::gl::detail::window::event::key::print_screen: return GLFW_KEY_PRINT_SCREEN;
		case clap::gl::detail::window::event::key::pause: return GLFW_KEY_PAUSE;
		case clap::gl::detail::window::event::key::f1: return GLFW_KEY_F1;
		case clap::gl::detail::window::event::key::f2: return GLFW_KEY_F2;
		case clap::gl::detail::window::event::key::f3: return GLFW_KEY_F3;
		case clap::gl::detail::window::event::key::f4: return GLFW_KEY_F4;
		case clap::gl::detail::window::event::key::f5: return GLFW_KEY_F5;
		case clap::gl::detail::window::event::key::f6: return GLFW_KEY_F6;
		case clap::gl::detail::window::event::key::f7: return GLFW_KEY_F7;
		case clap::gl::detail::window::event::key::f8: return GLFW_KEY_F8;
		case clap::gl::detail::window::event::key::f9: return GLFW_KEY_F9;
		case clap::gl::detail::window::event::key::f10: return GLFW_KEY_F10;
		case clap::gl::detail::window::event::key::f11: return GLFW_KEY_F11;
		case clap::gl::detail::window::event::key::f12: return GLFW_KEY_F12;
		case clap::gl::detail::window::event::key::f13: return GLFW_KEY_F13;
		case clap::gl::detail::window::event::key::f14: return GLFW_KEY_F14;
		case clap::gl::detail::window::event::key::f15: return GLFW_KEY_F15;
		case clap::gl::detail::window::event::key::f16: return GLFW_KEY_F16;
		case clap::gl::detail::window::event::key::f17: return GLFW_KEY_F17;
		case clap::gl::detail::window::event::key::f18: return GLFW_KEY_F18;
		case clap::gl::detail::window::event::key::f19: return GLFW_KEY_F19;
		case clap::gl::detail::window::event::key::f20: return GLFW_KEY_F20;
		case clap::gl::detail::window::event::key::f21: return GLFW_KEY_F21;
		case clap::gl::detail::window::event::key::f22: return GLFW_KEY_F22;
		case clap::gl::detail::window::event::key::f23: return GLFW_KEY_F23;
		case clap::gl::detail::window::event::key::f24: return GLFW_KEY_F24;
		case clap::gl::detail::window::event::key::f25: return GLFW_KEY_F25;
		case clap::gl::detail::window::event::key::kp_0: return GLFW_KEY_KP_0;
		case clap::gl::detail::window::event::key::kp_1: return GLFW_KEY_KP_1;
		case clap::gl::detail::window::event::key::kp_2: return GLFW_KEY_KP_2;
		case clap::gl::detail::window::event::key::kp_3: return GLFW_KEY_KP_3;
		case clap::gl::detail::window::event::key::kp_4: return GLFW_KEY_KP_4;
		case clap::gl::detail::window::event::key::kp_5: return GLFW_KEY_KP_5;
		case clap::gl::detail::window::event::key::kp_6: return GLFW_KEY_KP_6;
		case clap::gl::detail::window::event::key::kp_7: return GLFW_KEY_KP_7;
		case clap::gl::detail::window::event::key::kp_8: return GLFW_KEY_KP_8;
		case clap::gl::detail::window::event::key::kp_9: return GLFW_KEY_KP_9;
		case clap::gl::detail::window::event::key::kp_point: return GLFW_KEY_KP_DECIMAL;
		case clap::gl::detail::window::event::key::kp_divide: return GLFW_KEY_KP_DIVIDE;
		case clap::gl::detail::window::event::key::kp_multiply: return GLFW_KEY_KP_MULTIPLY;
		case clap::gl::detail::window::event::key::kp_subtract: return GLFW_KEY_KP_SUBTRACT;
		case clap::gl::detail::window::event::key::kp_add: return GLFW_KEY_KP_ADD;
		case clap::gl::detail::window::event::key::kp_enter: return GLFW_KEY_KP_ENTER;
		case clap::gl::detail::window::event::key::kp_equal: return GLFW_KEY_KP_EQUAL;
		case clap::gl::detail::window::event::key::shift_left: return GLFW_KEY_LEFT_SHIFT;
		case clap::gl::detail::window::event::key::control_left: return GLFW_KEY_LEFT_CONTROL;
		case clap::gl::detail::window::event::key::alt_left: return GLFW_KEY_LEFT_ALT;
		case clap::gl::detail::window::event::key::super_left: return GLFW_KEY_LEFT_SUPER;
		case clap::gl::detail::window::event::key::shift_right: return GLFW_KEY_RIGHT_SHIFT;
		case clap::gl::detail::window::event::key::control_right: return GLFW_KEY_RIGHT_CONTROL;
		case clap::gl::detail::window::event::key::alt_right: return GLFW_KEY_RIGHT_ALT;
		case clap::gl::detail::window::event::key::super_right: return GLFW_KEY_RIGHT_SUPER;
		case clap::gl::detail::window::event::key::menu: return GLFW_KEY_MENU;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::detail::window::event::key clap::gl::detail::convert::to_key(int v) {
	switch (v) {
		case GLFW_KEY_UNKNOWN: return clap::gl::detail::window::event::key::unknown;
		case GLFW_KEY_SPACE: return clap::gl::detail::window::event::key::space;
		case GLFW_KEY_APOSTROPHE: return clap::gl::detail::window::event::key::apostrophe;
		case GLFW_KEY_COMMA: return clap::gl::detail::window::event::key::comma;
		case GLFW_KEY_MINUS: return clap::gl::detail::window::event::key::minus;
		case GLFW_KEY_PERIOD: return clap::gl::detail::window::event::key::period;
		case GLFW_KEY_SLASH: return clap::gl::detail::window::event::key::slash;
		case GLFW_KEY_0: return clap::gl::detail::window::event::key::n_0;
		case GLFW_KEY_1: return clap::gl::detail::window::event::key::n_1;
		case GLFW_KEY_2: return clap::gl::detail::window::event::key::n_2;
		case GLFW_KEY_3: return clap::gl::detail::window::event::key::n_3;
		case GLFW_KEY_4: return clap::gl::detail::window::event::key::n_4;
		case GLFW_KEY_5: return clap::gl::detail::window::event::key::n_5;
		case GLFW_KEY_6: return clap::gl::detail::window::event::key::n_6;
		case GLFW_KEY_7: return clap::gl::detail::window::event::key::n_7;
		case GLFW_KEY_8: return clap::gl::detail::window::event::key::n_8;
		case GLFW_KEY_9: return clap::gl::detail::window::event::key::n_9;
		case GLFW_KEY_SEMICOLON: return clap::gl::detail::window::event::key::semicolon;
		case GLFW_KEY_EQUAL: return clap::gl::detail::window::event::key::equal;
		case GLFW_KEY_A: return clap::gl::detail::window::event::key::a;
		case GLFW_KEY_B: return clap::gl::detail::window::event::key::b;
		case GLFW_KEY_C: return clap::gl::detail::window::event::key::c;
		case GLFW_KEY_D: return clap::gl::detail::window::event::key::d;
		case GLFW_KEY_E: return clap::gl::detail::window::event::key::e;
		case GLFW_KEY_F: return clap::gl::detail::window::event::key::f;
		case GLFW_KEY_G: return clap::gl::detail::window::event::key::g;
		case GLFW_KEY_H: return clap::gl::detail::window::event::key::h;
		case GLFW_KEY_I: return clap::gl::detail::window::event::key::i;
		case GLFW_KEY_J: return clap::gl::detail::window::event::key::j;
		case GLFW_KEY_K: return clap::gl::detail::window::event::key::k;
		case GLFW_KEY_L: return clap::gl::detail::window::event::key::l;
		case GLFW_KEY_M: return clap::gl::detail::window::event::key::m;
		case GLFW_KEY_N: return clap::gl::detail::window::event::key::n;
		case GLFW_KEY_O: return clap::gl::detail::window::event::key::o;
		case GLFW_KEY_P: return clap::gl::detail::window::event::key::p;
		case GLFW_KEY_Q: return clap::gl::detail::window::event::key::q;
		case GLFW_KEY_R: return clap::gl::detail::window::event::key::r;
		case GLFW_KEY_S: return clap::gl::detail::window::event::key::s;
		case GLFW_KEY_T: return clap::gl::detail::window::event::key::t;
		case GLFW_KEY_U: return clap::gl::detail::window::event::key::u;
		case GLFW_KEY_V: return clap::gl::detail::window::event::key::v;
		case GLFW_KEY_W: return clap::gl::detail::window::event::key::w;
		case GLFW_KEY_X: return clap::gl::detail::window::event::key::x;
		case GLFW_KEY_Y: return clap::gl::detail::window::event::key::y;
		case GLFW_KEY_Z: return clap::gl::detail::window::event::key::z;
		case GLFW_KEY_LEFT_BRACKET: return clap::gl::detail::window::event::key::bracket_left;
		case GLFW_KEY_BACKSLASH: return clap::gl::detail::window::event::key::backslash;
		case GLFW_KEY_RIGHT_BRACKET: return clap::gl::detail::window::event::key::bracket_right;
		case GLFW_KEY_GRAVE_ACCENT: return clap::gl::detail::window::event::key::grave_accent;
		case GLFW_KEY_WORLD_1: return clap::gl::detail::window::event::key::world_1;
		case GLFW_KEY_WORLD_2: return clap::gl::detail::window::event::key::world_2;
		case GLFW_KEY_ESCAPE: return clap::gl::detail::window::event::key::escape;
		case GLFW_KEY_ENTER: return clap::gl::detail::window::event::key::enter;
		case GLFW_KEY_TAB: return clap::gl::detail::window::event::key::tab;
		case GLFW_KEY_BACKSPACE: return clap::gl::detail::window::event::key::backspace;
		case GLFW_KEY_INSERT: return clap::gl::detail::window::event::key::insert;
		case GLFW_KEY_DELETE: return clap::gl::detail::window::event::key::delete_;
		case GLFW_KEY_RIGHT: return clap::gl::detail::window::event::key::right;
		case GLFW_KEY_LEFT: return clap::gl::detail::window::event::key::left;
		case GLFW_KEY_DOWN: return clap::gl::detail::window::event::key::down;
		case GLFW_KEY_UP: return clap::gl::detail::window::event::key::up;
		case GLFW_KEY_PAGE_UP: return clap::gl::detail::window::event::key::page_up;
		case GLFW_KEY_PAGE_DOWN: return clap::gl::detail::window::event::key::page_down;
		case GLFW_KEY_HOME: return clap::gl::detail::window::event::key::home;
		case GLFW_KEY_END: return clap::gl::detail::window::event::key::end;
		case GLFW_KEY_CAPS_LOCK: return clap::gl::detail::window::event::key::caps_lock;
		case GLFW_KEY_SCROLL_LOCK: return clap::gl::detail::window::event::key::scroll_lock;
		case GLFW_KEY_NUM_LOCK: return clap::gl::detail::window::event::key::num_lock;
		case GLFW_KEY_PRINT_SCREEN: return clap::gl::detail::window::event::key::print_screen;
		case GLFW_KEY_PAUSE: return clap::gl::detail::window::event::key::pause;
		case GLFW_KEY_F1: return clap::gl::detail::window::event::key::f1;
		case GLFW_KEY_F2: return clap::gl::detail::window::event::key::f2;
		case GLFW_KEY_F3: return clap::gl::detail::window::event::key::f3;
		case GLFW_KEY_F4: return clap::gl::detail::window::event::key::f4;
		case GLFW_KEY_F5: return clap::gl::detail::window::event::key::f5;
		case GLFW_KEY_F6: return clap::gl::detail::window::event::key::f6;
		case GLFW_KEY_F7: return clap::gl::detail::window::event::key::f7;
		case GLFW_KEY_F8: return clap::gl::detail::window::event::key::f8;
		case GLFW_KEY_F9: return clap::gl::detail::window::event::key::f9;
		case GLFW_KEY_F10: return clap::gl::detail::window::event::key::f10;
		case GLFW_KEY_F11: return clap::gl::detail::window::event::key::f11;
		case GLFW_KEY_F12: return clap::gl::detail::window::event::key::f12;
		case GLFW_KEY_F13: return clap::gl::detail::window::event::key::f13;
		case GLFW_KEY_F14: return clap::gl::detail::window::event::key::f14;
		case GLFW_KEY_F15: return clap::gl::detail::window::event::key::f15;
		case GLFW_KEY_F16: return clap::gl::detail::window::event::key::f16;
		case GLFW_KEY_F17: return clap::gl::detail::window::event::key::f17;
		case GLFW_KEY_F18: return clap::gl::detail::window::event::key::f18;
		case GLFW_KEY_F19: return clap::gl::detail::window::event::key::f19;
		case GLFW_KEY_F20: return clap::gl::detail::window::event::key::f20;
		case GLFW_KEY_F21: return clap::gl::detail::window::event::key::f21;
		case GLFW_KEY_F22: return clap::gl::detail::window::event::key::f22;
		case GLFW_KEY_F23: return clap::gl::detail::window::event::key::f23;
		case GLFW_KEY_F24: return clap::gl::detail::window::event::key::f24;
		case GLFW_KEY_F25: return clap::gl::detail::window::event::key::f25;
		case GLFW_KEY_KP_0: return clap::gl::detail::window::event::key::kp_0;
		case GLFW_KEY_KP_1: return clap::gl::detail::window::event::key::kp_1;
		case GLFW_KEY_KP_2: return clap::gl::detail::window::event::key::kp_2;
		case GLFW_KEY_KP_3: return clap::gl::detail::window::event::key::kp_3;
		case GLFW_KEY_KP_4: return clap::gl::detail::window::event::key::kp_4;
		case GLFW_KEY_KP_5: return clap::gl::detail::window::event::key::kp_5;
		case GLFW_KEY_KP_6: return clap::gl::detail::window::event::key::kp_6;
		case GLFW_KEY_KP_7: return clap::gl::detail::window::event::key::kp_7;
		case GLFW_KEY_KP_8: return clap::gl::detail::window::event::key::kp_8;
		case GLFW_KEY_KP_9: return clap::gl::detail::window::event::key::kp_9;
		case GLFW_KEY_KP_DECIMAL: return clap::gl::detail::window::event::key::kp_point;
		case GLFW_KEY_KP_DIVIDE: return clap::gl::detail::window::event::key::kp_divide;
		case GLFW_KEY_KP_MULTIPLY: return clap::gl::detail::window::event::key::kp_multiply;
		case GLFW_KEY_KP_SUBTRACT: return clap::gl::detail::window::event::key::kp_subtract;
		case GLFW_KEY_KP_ADD: return clap::gl::detail::window::event::key::kp_add;
		case GLFW_KEY_KP_ENTER: return clap::gl::detail::window::event::key::kp_enter;
		case GLFW_KEY_KP_EQUAL: return clap::gl::detail::window::event::key::kp_equal;
		case GLFW_KEY_LEFT_SHIFT: return clap::gl::detail::window::event::key::shift_left;
		case GLFW_KEY_LEFT_CONTROL: return clap::gl::detail::window::event::key::control_left;
		case GLFW_KEY_LEFT_ALT: return clap::gl::detail::window::event::key::alt_left;
		case GLFW_KEY_LEFT_SUPER: return clap::gl::detail::window::event::key::super_left;
		case GLFW_KEY_RIGHT_SHIFT: return clap::gl::detail::window::event::key::shift_right;
		case GLFW_KEY_RIGHT_CONTROL: return clap::gl::detail::window::event::key::control_right;
		case GLFW_KEY_RIGHT_ALT: return clap::gl::detail::window::event::key::alt_right;
		case GLFW_KEY_RIGHT_SUPER: return clap::gl::detail::window::event::key::super_right;
		case GLFW_KEY_MENU: return clap::gl::detail::window::event::key::menu;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

int clap::gl::detail::convert::to_glfw(clap::gl::detail::window::event::key_action v) {
	switch (v) {
		case clap::gl::detail::window::event::key_action::press: return GLFW_PRESS;
		case clap::gl::detail::window::event::key_action::release: return GLFW_RELEASE;
		case clap::gl::detail::window::event::key_action::repeat: return GLFW_REPEAT;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::detail::window::event::key_action clap::gl::detail::convert::to_key_action(int v) {
	switch (v) {
		case GLFW_PRESS: return clap::gl::detail::window::event::key_action::press;
		case GLFW_RELEASE: return clap::gl::detail::window::event::key_action::release;
		case GLFW_REPEAT: return clap::gl::detail::window::event::key_action::repeat;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

int clap::gl::detail::convert::to_glfw(clap::gl::detail::window::event::mouse_button v) {
	switch (v) {
		case clap::gl::detail::window::event::mouse_button::b_1: return GLFW_MOUSE_BUTTON_1;
		case clap::gl::detail::window::event::mouse_button::b_2: return GLFW_MOUSE_BUTTON_2;
		case clap::gl::detail::window::event::mouse_button::b_3: return GLFW_MOUSE_BUTTON_3;
		case clap::gl::detail::window::event::mouse_button::b_4: return GLFW_MOUSE_BUTTON_4;
		case clap::gl::detail::window::event::mouse_button::b_5: return GLFW_MOUSE_BUTTON_5;
		case clap::gl::detail::window::event::mouse_button::b_6: return GLFW_MOUSE_BUTTON_6;
		case clap::gl::detail::window::event::mouse_button::b_7: return GLFW_MOUSE_BUTTON_7;
		case clap::gl::detail::window::event::mouse_button::b_8: return GLFW_MOUSE_BUTTON_8;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::detail::window::event::mouse_button clap::gl::detail::convert::to_mouse_button(int v) {
	switch (v) {
		case GLFW_MOUSE_BUTTON_1: return clap::gl::detail::window::event::mouse_button::b_1;
		case GLFW_MOUSE_BUTTON_2: return clap::gl::detail::window::event::mouse_button::b_2;
		case GLFW_MOUSE_BUTTON_3: return clap::gl::detail::window::event::mouse_button::b_3;
		case GLFW_MOUSE_BUTTON_4: return clap::gl::detail::window::event::mouse_button::b_4;
		case GLFW_MOUSE_BUTTON_5: return clap::gl::detail::window::event::mouse_button::b_5;
		case GLFW_MOUSE_BUTTON_6: return clap::gl::detail::window::event::mouse_button::b_6;
		case GLFW_MOUSE_BUTTON_7: return clap::gl::detail::window::event::mouse_button::b_7;
		case GLFW_MOUSE_BUTTON_8: return clap::gl::detail::window::event::mouse_button::b_8;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

int clap::gl::detail::convert::to_glfw(clap::gl::detail::window::event::mouse_button_action v) {
	switch (v) {
		case clap::gl::detail::window::event::mouse_button_action::press: return GLFW_PRESS;
		case clap::gl::detail::window::event::mouse_button_action::release: return GLFW_RELEASE;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::detail::window::event::mouse_button_action clap::gl::detail::convert::to_mouse_button_action(int v) {
	switch (v) {
		case GLFW_PRESS: return clap::gl::detail::window::event::mouse_button_action::press;
		case GLFW_RELEASE: return clap::gl::detail::window::event::mouse_button_action::release;
	}
	clap::log::error::critical << "Unsupported enum value.";
}

int clap::gl::detail::convert::to_glfw(clap::gl::detail::window::event::modificator_mask v) {
	return static_cast<int>(static_cast<underlying>(v));
}
clap::gl::detail::window::event::modificator_mask clap::gl::detail::convert::to_modificator_mask(int v) {
	return static_cast<clap::gl::detail::window::event::modificator_mask>(static_cast<underlying>(v));
}


std::ostream &operator<<(std::ostream &s, clap::gl::detail::window::event::key v) {
	switch (v) {
		case clap::gl::detail::window::event::key::unknown: return s << "unknown";
		case clap::gl::detail::window::event::key::space: return s << "space";
		case clap::gl::detail::window::event::key::apostrophe: return s << "apostrophe";
		case clap::gl::detail::window::event::key::comma: return s << "comma";
		case clap::gl::detail::window::event::key::minus: return s << "minus";
		case clap::gl::detail::window::event::key::period: return s << "period";
		case clap::gl::detail::window::event::key::slash: return s << "slash";
		case clap::gl::detail::window::event::key::n_0: return s << "0";
		case clap::gl::detail::window::event::key::n_1: return s << "1";
		case clap::gl::detail::window::event::key::n_2: return s << "2";
		case clap::gl::detail::window::event::key::n_3: return s << "3";
		case clap::gl::detail::window::event::key::n_4: return s << "4";
		case clap::gl::detail::window::event::key::n_5: return s << "5";
		case clap::gl::detail::window::event::key::n_6: return s << "6";
		case clap::gl::detail::window::event::key::n_7: return s << "7";
		case clap::gl::detail::window::event::key::n_8: return s << "8";
		case clap::gl::detail::window::event::key::n_9: return s << "9";
		case clap::gl::detail::window::event::key::semicolon: return s << "semicolon";
		case clap::gl::detail::window::event::key::equal: return s << "equal";
		case clap::gl::detail::window::event::key::a: return s << "a";
		case clap::gl::detail::window::event::key::b: return s << "b";
		case clap::gl::detail::window::event::key::c: return s << "c";
		case clap::gl::detail::window::event::key::d: return s << "d";
		case clap::gl::detail::window::event::key::e: return s << "e";
		case clap::gl::detail::window::event::key::f: return s << "f";
		case clap::gl::detail::window::event::key::g: return s << "g";
		case clap::gl::detail::window::event::key::h: return s << "h";
		case clap::gl::detail::window::event::key::i: return s << "i";
		case clap::gl::detail::window::event::key::j: return s << "j";
		case clap::gl::detail::window::event::key::k: return s << "k";
		case clap::gl::detail::window::event::key::l: return s << "l";
		case clap::gl::detail::window::event::key::m: return s << "m";
		case clap::gl::detail::window::event::key::n: return s << "n";
		case clap::gl::detail::window::event::key::o: return s << "o";
		case clap::gl::detail::window::event::key::p: return s << "p";
		case clap::gl::detail::window::event::key::q: return s << "q";
		case clap::gl::detail::window::event::key::r: return s << "r";
		case clap::gl::detail::window::event::key::s: return s << "s";
		case clap::gl::detail::window::event::key::t: return s << "t";
		case clap::gl::detail::window::event::key::u: return s << "u";
		case clap::gl::detail::window::event::key::v: return s << "v";
		case clap::gl::detail::window::event::key::w: return s << "w";
		case clap::gl::detail::window::event::key::x: return s << "x";
		case clap::gl::detail::window::event::key::y: return s << "y";
		case clap::gl::detail::window::event::key::z: return s << "z";
		case clap::gl::detail::window::event::key::bracket_left: return s << "left bracket";
		case clap::gl::detail::window::event::key::backslash: return s << "backslash";
		case clap::gl::detail::window::event::key::bracket_right: return s << "rigth bracket";
		case clap::gl::detail::window::event::key::grave_accent: return s << "grave accent";
		case clap::gl::detail::window::event::key::world_1: return s << "world 1";
		case clap::gl::detail::window::event::key::world_2: return s << "world 2";
		case clap::gl::detail::window::event::key::escape: return s << "escape";
		case clap::gl::detail::window::event::key::enter: return s << "enter";
		case clap::gl::detail::window::event::key::tab: return s << "tab";
		case clap::gl::detail::window::event::key::backspace: return s << "backspace";
		case clap::gl::detail::window::event::key::insert: return s << "insert";
		case clap::gl::detail::window::event::key::delete_: return s << "delete";
		case clap::gl::detail::window::event::key::right: return s << "right arrow";
		case clap::gl::detail::window::event::key::left: return s << "left arrow";
		case clap::gl::detail::window::event::key::down: return s << "down arrow";
		case clap::gl::detail::window::event::key::up: return s << "up arrow";
		case clap::gl::detail::window::event::key::page_up: return s << "page up";
		case clap::gl::detail::window::event::key::page_down: return s << "page down";
		case clap::gl::detail::window::event::key::home: return s << "home";
		case clap::gl::detail::window::event::key::end: return s << "end";
		case clap::gl::detail::window::event::key::caps_lock: return s << "caps lock";
		case clap::gl::detail::window::event::key::scroll_lock: return s << "scroll lock";
		case clap::gl::detail::window::event::key::num_lock: return s << "num lock";
		case clap::gl::detail::window::event::key::print_screen: return s << "print screen";
		case clap::gl::detail::window::event::key::pause: return s << "pause";
		case clap::gl::detail::window::event::key::f1: return s << "f1";
		case clap::gl::detail::window::event::key::f2: return s << "f2";
		case clap::gl::detail::window::event::key::f3: return s << "f3";
		case clap::gl::detail::window::event::key::f4: return s << "f4";
		case clap::gl::detail::window::event::key::f5: return s << "f5";
		case clap::gl::detail::window::event::key::f6: return s << "f6";
		case clap::gl::detail::window::event::key::f7: return s << "f7";
		case clap::gl::detail::window::event::key::f8: return s << "f8";
		case clap::gl::detail::window::event::key::f9: return s << "f9";
		case clap::gl::detail::window::event::key::f10: return s << "f10";
		case clap::gl::detail::window::event::key::f11: return s << "f11";
		case clap::gl::detail::window::event::key::f12: return s << "f12";
		case clap::gl::detail::window::event::key::f13: return s << "f13";
		case clap::gl::detail::window::event::key::f14: return s << "f14";
		case clap::gl::detail::window::event::key::f15: return s << "f15";
		case clap::gl::detail::window::event::key::f16: return s << "f16";
		case clap::gl::detail::window::event::key::f17: return s << "f17";
		case clap::gl::detail::window::event::key::f18: return s << "f18";
		case clap::gl::detail::window::event::key::f19: return s << "f19";
		case clap::gl::detail::window::event::key::f20: return s << "f20";
		case clap::gl::detail::window::event::key::f21: return s << "f21";
		case clap::gl::detail::window::event::key::f22: return s << "f22";
		case clap::gl::detail::window::event::key::f23: return s << "f23";
		case clap::gl::detail::window::event::key::f24: return s << "f24";
		case clap::gl::detail::window::event::key::f25: return s << "f25";
		case clap::gl::detail::window::event::key::kp_0: return s << "keypad 0";
		case clap::gl::detail::window::event::key::kp_1: return s << "keypad 1";
		case clap::gl::detail::window::event::key::kp_2: return s << "keypad 2";
		case clap::gl::detail::window::event::key::kp_3: return s << "keypad 3";
		case clap::gl::detail::window::event::key::kp_4: return s << "keypad 4";
		case clap::gl::detail::window::event::key::kp_5: return s << "keypad 5";
		case clap::gl::detail::window::event::key::kp_6: return s << "keypad 6";
		case clap::gl::detail::window::event::key::kp_7: return s << "keypad 7";
		case clap::gl::detail::window::event::key::kp_8: return s << "keypad 8";
		case clap::gl::detail::window::event::key::kp_9: return s << "keypad 9";
		case clap::gl::detail::window::event::key::kp_point: return s << "keypad point";
		case clap::gl::detail::window::event::key::kp_divide: return s << "keypad divide";
		case clap::gl::detail::window::event::key::kp_multiply: return s << "keypad multiply";
		case clap::gl::detail::window::event::key::kp_subtract: return s << "keypad subtract";
		case clap::gl::detail::window::event::key::kp_add: return s << "keypad add";
		case clap::gl::detail::window::event::key::kp_enter: return s << "keypad enter";
		case clap::gl::detail::window::event::key::kp_equal: return s << "keypad equal";
		case clap::gl::detail::window::event::key::shift_left: return s << "left shift";
		case clap::gl::detail::window::event::key::control_left: return s << "left control";
		case clap::gl::detail::window::event::key::alt_left: return s << "left alt";
		case clap::gl::detail::window::event::key::super_left: return s << "left super";
		case clap::gl::detail::window::event::key::shift_right: return s << "right shift";
		case clap::gl::detail::window::event::key::control_right: return s << "right control";
		case clap::gl::detail::window::event::key::alt_right: return s << "right alt";
		case clap::gl::detail::window::event::key::super_right: return s << "right super";
		case clap::gl::detail::window::event::key::menu: return s << "menu";
	}
	clap::log::warning::critical << "Unsupported enum value.";
	return s;
}
std::ostream &operator<<(std::ostream &s, clap::gl::detail::window::event::key_action v) {
	switch (v) {
		case clap::gl::detail::window::event::key_action::press: return s << "pressed";
		case clap::gl::detail::window::event::key_action::release: return s << "released";
		case clap::gl::detail::window::event::key_action::repeat: return s << "repeated";
	}
	clap::log::warning::critical << "Unsupported enum value.";
	return s;
}
std::ostream &operator<<(std::ostream &s, clap::gl::detail::window::event::mouse_button v) {
	switch (v) {
		case clap::gl::detail::window::event::mouse_button::b_1: return s << "first (left) mouse button";
		case clap::gl::detail::window::event::mouse_button::b_2: return s << "second (right) mouse button";
		case clap::gl::detail::window::event::mouse_button::b_3: return s << "third (middle) mouse button";
		case clap::gl::detail::window::event::mouse_button::b_4: return s << "fourth (xButton1) mouse button";
		case clap::gl::detail::window::event::mouse_button::b_5: return s << "fifth (xButton2) mouse button";
		case clap::gl::detail::window::event::mouse_button::b_6: return s << "sixth mouse button";
		case clap::gl::detail::window::event::mouse_button::b_7: return s << "seventh mouse button";
		case clap::gl::detail::window::event::mouse_button::b_8: return s << "eighth mouse button";
	}
	clap::log::warning::critical << "Unsupported enum value.";
	return s;
}
std::ostream &operator<<(std::ostream &s, clap::gl::detail::window::event::mouse_button_action v) {
	switch (v) {
		case clap::gl::detail::window::event::mouse_button_action::press: return s << "pressed";
		case clap::gl::detail::window::event::mouse_button_action::release: return s << "released";
	}
	clap::log::warning::critical << "Unsupported enum value.";
	return s;
}
std::ostream &operator<<(std::ostream &s, clap::gl::detail::window::event::modificator_mask v) {
	if (static_cast<underlying>(v & clap::gl::detail::window::event::modificator_mask::super))
		s << "Super + ";
	if (static_cast<underlying>(v & clap::gl::detail::window::event::modificator_mask::alt))
		s << "Alt + ";
	if (static_cast<underlying>(v & clap::gl::detail::window::event::modificator_mask::ctrl))
		s << "Ctrl + ";
	if (static_cast<underlying>(v & clap::gl::detail::window::event::modificator_mask::shift))
		s << "Shift + ";
	return s;
}

#pragma warning(pop)
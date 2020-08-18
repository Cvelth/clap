#include "gl/texture.hpp"

#include "glad/gl.h"

#include "gl/detail/state.hpp"
#include "essential/log.hpp"

clap::gl::texture::multiple::multiple(size_t count) : count(count) {
	gl::detail::state::ensure_loaded();

	ids = new uint32_t[count];
	glGenTextures(GLsizei(count), ids);
	log::message::minor << "Texture objects (" << count << ") were created.";
}
clap::gl::texture::multiple::~multiple() {
	for (size_t i = 0; i < count; i++)
		while (auto target = gl::detail::state::is_bound(id(i)))
			gl::detail::state::unbind(*target);

	glDeleteTextures(GLsizei(count), ids);
	if (count) delete[] ids;
	log::message::minor << "Texture objects (" << count << ") were destroyed.";
}

clap::gl::texture::detail::indexed clap::gl::texture::multiple::id(size_t index) {
	return detail::indexed(this, index);
}

clap::gl::texture::multiple::multiple(size_t count, uint32_t *ids) : count(count), ids(ids) {
	gl::detail::state::ensure_loaded();

	for (size_t i = 0; i < count; i++)
		if (!glIsTexture(ids[i])) {
			log::warning::critical << "Unable to perform a texture object move operation. "
				"Passed texture seems to be corrupted.";
			return;
		}

	log::message::negligible << "Texture objects (" << count << ") were moved.";
}

void clap::gl::texture::detail::indexed::bind(target target) {
	gl::detail::state::bind(target, std::move(*this));
}

void clap::gl::texture::detail::indexed::data(void *data, unsigned width, unsigned height, target target,
											  bool should_compress, internal_format internal_format,
											  external_format external_format, type external_type,
											  int level) {
	/// TODO
}

clap::gl::texture::detail::indexed::operator bool() const {
	return pointer && index < pointer->count;
}
uint32_t clap::gl::texture::detail::indexed::operator*() const { return pointer->ids[index]; }

clap::gl::texture::detail::indexed::indexed(multiple *pointer, size_t index)
	: pointer(pointer), index(index) {
	if (!*this) {
		log::error::major << "Attempting to access an out of bounds texture object.";
		log::info::major << "Index is " << index << '.';
		log::info::major << "Count is " << pointer->count << '.';
	}
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

GLenum clap::gl::detail::convert::to_gl(clap::gl::texture::type v) {
	switch (v) {
		case texture::type::unsigned_byte: return GL_UNSIGNED_BYTE;
		case texture::type::byte: return GL_BYTE;
		case texture::type::unsigned_short: return GL_UNSIGNED_SHORT;
		case texture::type::_short: return GL_SHORT;
		case texture::type::unsinged_int: return GL_UNSIGNED_INT;
		case texture::type::_int: return GL_INT;
		case texture::type::half_float: return GL_HALF_FLOAT;
		case texture::type::_float: return GL_FLOAT;
		case texture::type::unsigned_byte_3_3_2: return GL_UNSIGNED_BYTE_3_3_2;
		case texture::type::unsigned_byte_2_3_3_r: return GL_UNSIGNED_BYTE_2_3_3_REV;
		case texture::type::unsigned_short_5_6_5: return GL_UNSIGNED_SHORT_5_6_5;
		case texture::type::unsigned_short_5_6_5_r: return GL_UNSIGNED_SHORT_5_6_5_REV;
		case texture::type::unsigned_short_4_4_4_4: return GL_UNSIGNED_SHORT_4_4_4_4;
		case texture::type::unsigned_short_4_4_4_4_r: return GL_UNSIGNED_SHORT_4_4_4_4_REV;
		case texture::type::unsigned_short_5_5_5_1: return GL_UNSIGNED_SHORT_5_5_5_1;
		case texture::type::unsigned_short_1_5_5_5_r: return GL_UNSIGNED_SHORT_1_5_5_5_REV;
		case texture::type::unsigned_int_8_8_8_8: return GL_UNSIGNED_INT_8_8_8_8;
		case texture::type::unsigned_int_8_8_8_8_r: return GL_UNSIGNED_INT_8_8_8_8_REV;
		case texture::type::unsigned_int_10_10_10_2: return GL_UNSIGNED_INT_10_10_10_2;
		case texture::type::unsigned_int_2_10_10_10_r: return GL_UNSIGNED_INT_2_10_10_10_REV;
	}
	clap::log::error::critical << "Unsupported enum value.";
}
clap::gl::texture::type clap::gl::detail::convert::to_texture_type(GLenum v) {
	switch (v) {
		case GL_UNSIGNED_BYTE: return texture::type::unsigned_byte;
		case GL_BYTE: return texture::type::byte;
		case GL_UNSIGNED_SHORT: return texture::type::unsigned_short;
		case GL_SHORT: return texture::type::_short;
		case GL_UNSIGNED_INT: return texture::type::unsinged_int;
		case GL_INT: return texture::type::_int;
		case GL_HALF_FLOAT: return texture::type::half_float;
		case GL_FLOAT: return texture::type::_float;
		case GL_UNSIGNED_BYTE_3_3_2: return texture::type::unsigned_byte_3_3_2;
		case GL_UNSIGNED_BYTE_2_3_3_REV: return texture::type::unsigned_byte_2_3_3_r;
		case GL_UNSIGNED_SHORT_5_6_5: return texture::type::unsigned_short_5_6_5;
		case GL_UNSIGNED_SHORT_5_6_5_REV: return texture::type::unsigned_short_5_6_5_r;
		case GL_UNSIGNED_SHORT_4_4_4_4: return texture::type::unsigned_short_4_4_4_4;
		case GL_UNSIGNED_SHORT_4_4_4_4_REV: return texture::type::unsigned_short_4_4_4_4_r;
		case GL_UNSIGNED_SHORT_5_5_5_1: return texture::type::unsigned_short_5_5_5_1;
		case GL_UNSIGNED_SHORT_1_5_5_5_REV: return texture::type::unsigned_short_1_5_5_5_r;
		case GL_UNSIGNED_INT_8_8_8_8: return texture::type::unsigned_int_8_8_8_8;
		case GL_UNSIGNED_INT_8_8_8_8_REV: return texture::type::unsigned_int_8_8_8_8_r;
		case GL_UNSIGNED_INT_10_10_10_2: return texture::type::unsigned_int_10_10_10_2;
		case GL_UNSIGNED_INT_2_10_10_10_REV: return texture::type::unsigned_int_2_10_10_10_r;
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
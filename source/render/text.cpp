#include "render/text.hpp"
#include "render/font.hpp"

#include "essential/log.hpp"

#include "nowide/convert.hpp"

#include <fstream>

#include "ft2build.h"
#include FT_FREETYPE_H

#include "glad/glad.h"

namespace clap::render::detail {
	class library {
	public:
		static void initialize();
		static void clean_up();
		static void ensure_initialized();

		static auto is_initialized() {
			return was_initialized;
		}
		static auto const &handle() {
			return freetype_handle;
		}

	protected:
		static bool was_initialized;
		static FT_Library freetype_handle;
	};
}

bool clap::render::detail::library::was_initialized = false;
FT_Library clap::render::detail::library::freetype_handle{};

void clap::render::detail::library::initialize() {
	if (!was_initialized) {
		if (FT_Init_FreeType(&clap::render::detail::library::freetype_handle))
			clap::log::error::critical << "Unable to initialize freetype.";
		clap::render::detail::library::was_initialized = true;
	}

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}
void clap::render::detail::library::clean_up() {
	if (was_initialized) {
		if (FT_Done_FreeType(clap::render::detail::library::freetype_handle))
			clap::log::error::critical << "Unable to finalize freetype.";
		clap::render::detail::library::was_initialized = false;
	}
}
void clap::render::detail::library::ensure_initialized() { initialize(); }

//Temporary. To be updated after full c++20 <bit> support arrives.
namespace nonstd {
	template<typename T>
	T bit_ceil(T input) {
		unsigned output = 1;
		if (input && !(input & (input - 1)))
			return input;

		while (output < input)
			output <<= 1;

		return output;
	}
}

#include "essential/log.hpp"

namespace clap::render::detail {
	class font_file_handle {
	public:
		FT_Face face;
		std::vector<char> buffer;

		~font_file_handle() {
			FT_Done_Face(face);
		}
	};
}

clap::render::font::~font() {
	delete file_handle;
}

clap::render::font clap::render::font::load(std::filesystem::path const &filename) {
	detail::library::ensure_initialized();

	auto *handle = new detail::font_file_handle{};

	std::ifstream font_file(filename, std::ios::in | std::ios::binary | std::ios::ate);
	auto file_size = font_file.tellg();
	font_file.seekg(0, std::ios::beg);

	handle->buffer.resize(file_size);
	if (!font_file.read(handle->buffer.data(), file_size)) {
		log::error::major << "Font file cannot be loaded. 'std::ifstream::read' returns error.";
		log::info::critical << "Path: " << filename << ".";
	}

	auto error = FT_New_Memory_Face(clap::render::detail::library::handle(),
									(FT_Byte const *) handle->buffer.data(),
									(FT_Long) file_size, 0, &handle->face);
	if (error == FT_Err_Unknown_File_Format) {
		log::error::major << "Unsupported font file cannot be loaded.";
		log::info::critical << "Path: " << filename << ".";
	} else if (error) {
		log::error::major << "Cannot open font file.";
		log::info::critical << "Path: " << filename << ".";
	}
	if (handle->face->charmap->encoding != FT_ENCODING_UNICODE) {
		log::error::minor << "Only unicode fonts are supported.";
		log::info::critical << "Path: " << filename << ".";
	}

	return handle;
}

clap::render::detail::size_data::size_data(size_t bitmap_width, size_t bitmap_height)
	: bitmap(
		gl::texture::_2d{
			nullptr,
			bitmap_width,
			bitmap_height,
			false,
			clap::gl::texture::internal_format::r8,
			clap::gl::texture::external_format::red,
			clap::gl::texture::external_type::unsigned_byte
		}
	), offset_x(0), offset_y(0), current_layer_y(0) {
	bitmap.set_min_filter(clap::gl::texture::min_filter::nearest);
	bitmap.set_mag_filter(clap::gl::texture::mag_filter::nearest);
}

void clap::render::text::update(std::basic_string<char8_t> const &string) {
	update(nowide::utf::convert_string<char32_t>(string.c_str(), string.c_str() + string.size()));
}
void clap::render::text::update(std::basic_string<char32_t> const &string) {
	auto font_face = font_handle.file_handle->face;
	FT_Set_Pixel_Sizes(font_face, 0, FT_UInt(height));

	if (font_handle.data.find(height) == font_handle.data.end()) {
		auto expected_height = nonstd::bit_ceil(height * settings::font_bitmap_size_multiplier),
			expected_width = expected_height;
		if (expected_height > gl::texture::_2d::maximum_height() ||
			expected_width > gl::texture::_2d::maximum_width()) {

			clap::log::warning::major << "Requested font bitmap size is not supported on given system. "
				"Setting it to the maximum allowed size.";
			clap::log::info::critical << "Requested size: " << height * settings::font_bitmap_size_multiplier
				<< "x" << height * settings::font_bitmap_size_multiplier << ".";
			clap::log::info::critical << "Maximum allowed size: " << gl::texture::_2d::maximum_width()
				<< "x" << gl::texture::_2d::maximum_height() << ".";
			clap::log::info::minor << "'settings::font_bitmap_size_multiplier':"
				<< settings::font_bitmap_size_multiplier << ".";
			clap::log::info::minor << "Font size: " << height << ".";
		}
		font_handle.data.emplace(
			height,
			detail::size_data(
				std::min(expected_width, gl::texture::_2d::maximum_width()),
				std::min(expected_height, gl::texture::_2d::maximum_height())
			)
		);
	}
	auto &target = font_handle.data.at(height);

	std::vector<float> buffer_data;
	buffer_data.reserve(string.size() * 4 * 2);
	size_t advance_x = 0, advance_y = 0;

	bool has_kerning = FT_HAS_KERNING(font_face);
	if (has_kerning) {
		clap::log::message::minor << "Kerning is available.";
		clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
	} else {
		clap::log::message::minor << "Kerning is not available.";
		clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
	}

	unsigned previous_index = 0;
	for (auto code_point : string) {
		if (code_point == U'\n') {
			advance_y += height;
			advance_x = 0;
			continue;
		}

		auto index = FT_Get_Char_Index(font_face, code_point);
		if (!index) {
			clap::log::warning::major << "Requested character isn't present in the choosen font. It cannot be displayed.";
			clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
			clap::log::info::major << "Character: '" << code_point << "' (0x" << std::hex << size_t(code_point) << std::dec << ").";
		}
		auto error = FT_Load_Glyph(font_face, index, FT_LOAD_RENDER);
		if (error) {
			clap::log::warning::major << "Unable to load a character glyph.";
			clap::log::info::major << "Error: " << error << " -> " << FT_Error_String(error) << ".";
			clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
			clap::log::info::major << "Character: '" << code_point << "' (0x" << std::hex << size_t(code_point) << std::dec << ").";
			clap::log::info::major << "Index: " << index << ".";
		} else {
			auto iterator = target.coordinates.find(code_point);
			if (iterator == target.coordinates.end()) {
				if (target.offset_x + font_face->glyph->bitmap.width > target.bitmap.get_width()) {
					target.offset_x = 0;
					target.offset_y += target.current_layer_y;
					target.current_layer_y = 0;
				}
				if (target.offset_y + height > target.bitmap.get_height()) {
					clap::log::warning::critical << "Font bitmap buffer is not big enough to store all the requested characters.";
					clap::log::info::critical << "No further character of the font with given size can be loaded.";
					clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
					clap::log::info::major << "Size: " << height << ".";
				} else {
					if (font_face->glyph->bitmap.pixel_mode != FT_PIXEL_MODE_GRAY) {
						clap::log::warning::critical << "Used font features unsupported pixel mode. It will not be rendered correctly.";
						clap::log::info::critical << "No further character of the font with given size can be loaded.";
						clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
					}
					if (font_face->glyph->bitmap.width != font_face->glyph->bitmap.pitch) {
						clap::log::warning::critical << "Bitmap alignment of the character contains padding. "
							"Such characters are currently not supported. It will not be rendered correctly.";
						clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
						clap::log::info::major << "Character: '" << code_point << "' (0x" << std::hex << size_t(code_point) << std::dec << ").";
						clap::log::info::major << "Index: " << index << ".";
						clap::log::info::minor << "Buffer width: " << font_face->glyph->bitmap.width << ".";
						clap::log::info::minor << "Buffer pitch: " << font_face->glyph->bitmap.pitch << ".";
					}
					if (font_face->glyph->bitmap.rows > height) {
						clap::log::warning::minor << "Bitmap of the character is too big. Part of it could be lost.";
						clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
						clap::log::info::major << "Character: '" << code_point << "' (0x" << std::hex << size_t(code_point) << std::dec << ").";
						clap::log::info::major << "Index: " << index << ".";
					}
					target.bitmap.data(font_face->glyph->bitmap.buffer, target.offset_x, target.offset_y,
									   font_face->glyph->bitmap.width, font_face->glyph->bitmap.rows,
									   false, 0, clap::gl::texture::external_format::red);
					iterator = target.coordinates.emplace(code_point,
														  detail::bitmap_coordinates{
															  target.offset_x, target.offset_y,
															  target.offset_x + font_face->glyph->bitmap.width,
															  target.offset_y + font_face->glyph->bitmap.rows
														  }).first;
					target.offset_x += font_face->glyph->bitmap.width;
					target.current_layer_y = std::max(target.current_layer_y, (size_t) font_face->glyph->bitmap.rows);
					clap::log::message::minor << "A new character was added to font bitmap atlas.";
					clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
					clap::log::info::major << "Size: " << height << ".";
					clap::log::info::major << "Character: '" << code_point << "' (0x" << std::hex << size_t(code_point) << std::dec << ").";
					clap::log::info::major << "Index: " << index << ".";
				}
			}
			if (has_kerning && previous_index && index) {
				FT_Vector kerning_data;
				error = FT_Get_Kerning(font_face, previous_index, index, FT_KERNING_DEFAULT, &kerning_data);
				advance_x += kerning_data.x >> 6;
				advance_y += kerning_data.y >> 6;
				previous_index = index;
			}

			buffer_data.push_back(float(advance_x + font_face->glyph->bitmap_left));
			buffer_data.push_back(float(advance_y + height - font_face->glyph->bitmap_top));
			buffer_data.push_back(float(iterator->second.from_x) / target.bitmap.get_width());
			buffer_data.push_back(float(iterator->second.from_y) / target.bitmap.get_height());

			buffer_data.push_back(float(advance_x + font_face->glyph->bitmap_left
										+ (iterator->second.to_x - iterator->second.from_x)));
			buffer_data.push_back(float(advance_y + height - font_face->glyph->bitmap_top
										+ (iterator->second.to_y - iterator->second.from_y)));
			buffer_data.push_back(float(iterator->second.to_x) / target.bitmap.get_width());
			buffer_data.push_back(float(iterator->second.to_y) / target.bitmap.get_height());

			advance_x += font_face->glyph->advance.x >> 6;
			advance_y += font_face->glyph->advance.y >> 6;
		}
	}

	clap::gl::vertex::buffer buffer;
	buffer.data(buffer_data.data(), sizeof(decltype(buffer_data)::value_type) * buffer_data.size(), 
				clap::gl::vertex::buffer::usage::static_draw);

	vertex_array.attribute_pointer(buffer, program->attribute["position"], 4, 0);
	vertex_array.attribute_pointer(buffer, program->attribute["texture_coordinates"], 4, 2);
	count = buffer_data.size() / 4;

	clap::log::message::minor << "Text object was updated.";
	clap::log::info::major << "Message: \"" << string << "\".";
	clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
	clap::log::info::major << "Size: " << height << ".";

	return;
}

void clap::render::text::render(int x, int y) const {
	program->uniform["offset"] = { float(x), float(y) };

	auto guard = program->use();
	font_handle.data.at(height).bitmap.bind();
	vertex_array.draw(clap::gl::vertex::array::connection::lines, count);
}

size_t clap::render::settings::font_bitmap_size_multiplier = 32;
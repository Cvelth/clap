#include "render/text.hpp"
#include "render/font.hpp"
#include "render/detail/state.hpp"
#include FT_BITMAP_H

#include "nowide/convert.hpp"

#include <fstream>

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
	detail::state::ensure_initialized();

	auto *handle = new detail::font_file_handle{};

	std::ifstream font_file(filename, std::ios::in | std::ios::binary | std::ios::ate);
	auto file_size = font_file.tellg();
	font_file.seekg(0, std::ios::beg);

	handle->buffer.resize(file_size);
	if (!font_file.read(handle->buffer.data(), file_size)) {
		log::error::major << "Font file cannot be loaded. 'std::ifstream::read' returns error.";
		log::info::critical << "Path: " << filename << ".";
	}

	auto error = FT_New_Memory_Face(clap::render::detail::state::library(),
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
			true,
			clap::gl::texture::internal_format::red,
			clap::gl::texture::external_format::red,
			clap::gl::texture::external_type::unsigned_byte
		}
	), offset_x(0), offset_y(0) {}

clap::render::text::text(std::basic_string<char8_t> const &string, font &font,
						 size_t height) : font_handle(font), height(height) {
	update(string);
}
clap::render::text::text(std::string const &string, font &font,
						 size_t height) : font_handle(font), height(height) {
	update(string);
}

void clap::render::text::update(std::basic_string<char8_t> const &string) {
	update(nowide::utf::convert_string<char32_t>(string.c_str(), string.c_str() + string.size()));
}
void clap::render::text::update(std::basic_string<char32_t> const &string) {
	auto font_face = font_handle.file_handle->face;
	FT_Set_Pixel_Sizes(font_face, 0, FT_UInt(height));

	for (auto code_point : string) {
		auto index = FT_Get_Char_Index(font_face, code_point);
		if (!index) {
			clap::log::warning::major << "Requested character isn't present in the choosen font. It cannot be displayed.";
			clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
			clap::log::info::major << "Character: '" << code_point << "' (0x" << std::hex << size_t(code_point) << std::dec << ").";
		} else {
			auto error = FT_Load_Glyph(font_face, index, FT_LOAD_RENDER);
			if (error) {
				clap::log::warning::major << "Unable to load a character glyph.";
				clap::log::info::major << "Error: " << error << " -> " << FT_Error_String(error) << ".";
				clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
				clap::log::info::major << "Character: '" << code_point << "' (0x" << std::hex << size_t(code_point) << std::dec << ").";
				clap::log::info::major << "Index: " << index << ".";
			} else {
				if (font_handle.data.find(height) == font_handle.data.end()) {
					font_handle.data.emplace(
						height,
						detail::size_data(
							font_handle.bitmap_width,
							font_handle.bitmap_height
						)
					);
				}

				if (height < font_face->glyph->bitmap.rows) {
					clap::log::warning::major << "Bitmap height of the glyph exceeds maximum allowed. Glyph is skipped.";
					clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
					clap::log::info::major << "Character: '" << code_point << "' (0x" << std::hex << size_t(code_point) << std::dec << ").";
					clap::log::info::major << "Index: " << index << ".";
				} else {
					auto &target = font_handle.data.at(height);

					if (target.offset_x + font_face->glyph->bitmap.width > font_handle.bitmap_width) {
						target.offset_y += height;
						target.offset_x = 0;
					}
					if (target.offset_y + height > font_handle.bitmap_height) {
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
							clap::log::warning::critical << "Bitmap of the character is too big. Part of it will be cut down. "
								"Such characters are currently not supported. It will not be rendered correctly.";
							clap::log::info::major << "Font: " << font_face->family_name << " " << font_face->style_name << ".";
							clap::log::info::major << "Character: '" << code_point << "' (0x" << std::hex << size_t(code_point) << std::dec << ").";
							clap::log::info::major << "Index: " << index << ".";
						}
						target.bitmap.data(font_face->glyph->bitmap.buffer, target.offset_x, target.offset_y,
										   font_face->glyph->bitmap.width, font_face->glyph->bitmap.rows,
										   true, 0, clap::gl::texture::external_format::red);
						target.coordinates.emplace(code_point,
												   detail::bitmap_coordinates{
													   target.offset_x, target.offset_y,
													   target.offset_x + font_face->glyph->bitmap.width,
													   target.offset_y + font_face->glyph->bitmap.rows
												   });
						target.offset_x += font_face->glyph->bitmap.width;
					}
				}
			}
		}
	}
	return;
}

void clap::render::text::render() const {
	font_handle.data.at(height).bitmap.bind();
}
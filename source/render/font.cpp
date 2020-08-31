#include "render/font.hpp"
#include "render/detail/state.hpp"

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

clap::render::font::font(detail::font_file_handle *file_handle) : file_handle(file_handle),
	bitmap_width(gl::texture::_2d::maximum_width()),
	bitmap_height(gl::texture::_2d::maximum_height()) {}

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
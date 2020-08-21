#include "render/font.hpp"
#include "render/detail/state.hpp"

#include "essential/log.hpp"

namespace clap::render::detail {
	class font_face_t {
	public:
		FT_Face handle;
	};
}

clap::render::font::font(detail::font_face_t const &face) : face(new detail::font_face_t(face)) {}

clap::render::font::~font() {
	delete face;
}

clap::render::font clap::render::font::load(std::string const &filename) {
	detail::state::ensure_initialized();

	detail::font_face_t face;
	auto error = FT_New_Face(clap::render::detail::state::library(), filename.c_str(), 0, &face.handle);
	if (error == FT_Err_Unknown_File_Format) {
		log::error::major << "Unsupported font file cannot be loaded.";
		log::info::critical << "Path: " << filename << ".";
	}
	else if (error) {
		log::error::major << "Cannot open font file.";
		log::info::critical << "Path: " << filename << ".";
	}
	return face;
}
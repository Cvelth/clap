#include "render/detail/state.hpp"

#include "essential/log.hpp"

#include "glad/glad.h"

bool clap::render::detail::state::initialized = false;
FT_Library clap::render::detail::state::library_handle{};

void clap::render::detail::state::initialize_freetype() {
	if (!initialized) {
		if (FT_Init_FreeType(&clap::render::detail::state::library_handle))
			clap::log::error::critical << "Unable to initialize freetype.";
		clap::render::detail::state::initialized = true;
	}

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}
void clap::render::detail::state::finilize_freetype() {
	if (initialized) {
		if (FT_Done_FreeType(clap::render::detail::state::library_handle))
			clap::log::error::critical << "Unable to finalize freetype.";
		clap::render::detail::state::initialized = false;
	}
}
void clap::render::detail::state::ensure_initialized() {
	initialize_freetype();
}
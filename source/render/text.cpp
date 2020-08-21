#include "render/text.hpp"

#include "essential/log.hpp"

#include "ft2build.h"
#include FT_FREETYPE_H

void clap::render::initialize_freetype() {
	FT_Library  library;
	if (FT_Init_FreeType(&library))
		clap::log::error::critical << "Unable to initialize freetype.";
}
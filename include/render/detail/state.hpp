#pragma once
#include "ft2build.h"
#include FT_FREETYPE_H

namespace clap::render::detail {
	class state {
	public:
		static void initialize_freetype();
		static void finilize_freetype();
		static void ensure_initialized();

		static auto is_initialized() {
			return initialized;
		}
		static auto const &library() {
			return library_handle;
		}

	protected:
		static bool initialized;
		static FT_Library library_handle;
	};

}
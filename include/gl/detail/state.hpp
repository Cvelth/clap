#pragma once
#include <optional>
#include "gl/texture.hpp"
#include "gl/vertex.hpp"

namespace clap::gl::shader {
	class program;
}
namespace clap::gl::detail {
	void load_gl();
	class state {
		friend void load_gl();
	public:
		static void verify_loaded();

		state() = delete;
		state(state const &other) = delete;
		state(state &&other) = delete;

	private:
		static bool was_loaded;
	};
}


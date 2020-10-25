﻿#pragma once
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

		static void bind(texture::target const &target, texture::detail::interface *texture);
		static texture::detail::interface *unbind(texture::target const &target);
		static texture::detail::interface const *const bound(texture::target const &target);
		static std::optional<texture::target> is_bound(texture::detail::interface const *const texture);

		state() = delete;
		state(state const &other) = delete;
		state(state &&other) = delete;

	private:
		static bool was_loaded;

		static constexpr size_t texture_target_count = 11;
		static texture::detail::interface *bound_textures[texture_target_count];
	};
}


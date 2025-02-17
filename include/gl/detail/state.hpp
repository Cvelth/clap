﻿#pragma once
#include <optional>
#include "gl/buffer.hpp"
#include "gl/texture.hpp"
#include "gl/vertex_array.hpp"

namespace clap::gl::shader {
	class program;
}

namespace clap::gl::detail {
	class state {
	public:
		static bool load();
		static void ensure_loaded();

		static void bind(buffer::target const &target, buffer::detail::indexed &&buffer);
		static std::optional<buffer::detail::indexed> const unbind(buffer::target const &target);
		static std::optional<buffer::detail::indexed> const &bound(buffer::target const &target);
		static std::optional<buffer::target> is_bound(buffer::detail::indexed const &buffer);

		static void bind(vertex_array::detail::indexed &&vertex_array);
		static std::optional<vertex_array::detail::indexed> const unbind();
		static std::optional<vertex_array::detail::indexed> const &bound();
		static bool is_bound(vertex_array::detail::indexed const &vertex_array);

		static void use(shader::program *program);
		static shader::program *being_used();
		static bool is_used(shader::program *program);

		static void bind(texture::target const &target, texture::detail::interface *texture);
		static texture::detail::interface* unbind(texture::target const &target);
		static texture::detail::interface const*const bound(texture::target const &target);
		static std::optional<texture::target> is_bound(texture::detail::interface const*const texture);

		state() = delete;
		state(state const &other) = delete;
		state(state &&other) = delete;

	private:
		static constexpr size_t buffer_target_count = 14;
		static std::optional<buffer::detail::indexed> bound_buffers[buffer_target_count];
		static std::optional<vertex_array::detail::indexed> bound_vertex_array;
		static shader::program *program_used;

		static constexpr size_t texture_target_count = 11;
		static texture::detail::interface *bound_textures[texture_target_count];
	};
}


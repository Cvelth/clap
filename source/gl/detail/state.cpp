#include "gl/detail/state.hpp"

#include "glad/glad.h"

#include "gl/shader.hpp"
#include "essential/log.hpp"

bool clap::gl::detail::state::was_loaded = false;
void clap::gl::detail::state::verify_loaded() { 
	if (!was_loaded)
		clap::log::error::critical << "An attempt to use OpenGL before it was loaded.";
}

clap::gl::texture::detail::interface *clap::gl::detail::state::bound_textures[texture_target_count]{ nullptr };
void clap::gl::detail::state::bind(texture::target const &target, texture::detail::interface *texture) {
	if (!bound_textures[size_t(target)] || bound_textures[size_t(target)] != texture)
		if (texture) {
			log::message::minor << "A new texture is bound to '" << target << "'.";
			glBindTexture(detail::convert::to_gl(target),
						 **(bound_textures[size_t(target)] = std::move(texture)));
		} else
			log::warning::critical << "A texture passed to 'gl::state::bind' is not a valid OpenGL texture object.";
}
clap::gl::texture::detail::interface *clap::gl::detail::state::unbind(texture::target const &target) {
	glBindTexture(detail::convert::to_gl(target), 0);
	auto out = std::move(bound_textures[size_t(target)]);
	bound_textures[size_t(target)] = nullptr;
	if (out)
		log::message::minor << "\"" << target << "\" was unbound.";
	return std::move(out);
}
clap::gl::texture::detail::interface const*const clap::gl::detail::state::bound(texture::target const &target) {
	return bound_textures[size_t(target)];
}
std::optional<clap::gl::texture::target> clap::gl::detail::state::is_bound(clap::gl::texture::detail::interface const *const texture) {
	for (size_t i = 0; i < texture_target_count; i++)
		if (bound_textures[i] && bound_textures[i] == texture)
			return clap::gl::texture::target(i);
	return std::nullopt;
}
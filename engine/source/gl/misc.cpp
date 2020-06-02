#include "gl/misc.hpp"

#include "glad/glad.h"

#include "error.hpp"

void engine::gl::clear::set_color(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}
void engine::gl::clear::set_depth(double d) {
	glClearDepth(d);
}
void engine::gl::clear::set_stencil(int d) {
	glClearStencil(d);
}

void engine::gl::clear::target(unsigned target_mask) {
	if (target_mask > (target_mask::color_buffer | target_mask::depth_buffer | target_mask::stencil_buffer))
		error::critical("Invalid target mask was passed to 'engine::gl::clear::target'");
	glClear(
		(target_mask & target_mask::color_buffer ? GL_COLOR_BUFFER_BIT : 0) |
		(target_mask & target_mask::depth_buffer ? GL_DEPTH_BUFFER_BIT : 0) |
		(target_mask & target_mask::stencil_buffer ? GL_STENCIL_BUFFER_BIT : 0)
	);
}

void engine::gl::clear::color() {
	glClear(GL_COLOR_BUFFER_BIT);
}
void engine::gl::clear::depth() {
	glClear(GL_DEPTH_BUFFER_BIT);
}
void engine::gl::clear::stencil() {
	glClear(GL_DEPTH_BUFFER_BIT);
}
void engine::gl::clear::color_depth() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void engine::gl::clear::all() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

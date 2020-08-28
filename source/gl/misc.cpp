#include "gl/misc.hpp"

#include <algorithm>
#include <iomanip>

#include "glad/glad.h"

#include "essential/log.hpp"

void clap::gl::clear::set_color(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	log::message::minor << "Default (background) color was changed to #"
		<< std::hex
		<< std::setw(2) << std::setfill('0') << std::clamp(static_cast<unsigned>(a * 255.f), 0u, 255u)
		<< std::setw(2) << std::setfill('0') << std::clamp(static_cast<unsigned>(r * 255.f), 0u, 255u)
		<< std::setw(2) << std::setfill('0') << std::clamp(static_cast<unsigned>(g * 255.f), 0u, 255u)
		<< std::setw(2) << std::setfill('0') << std::clamp(static_cast<unsigned>(b * 255.f), 0u, 255u)
		<< std::dec << '.';
}
void clap::gl::clear::set_depth(double d) {
	glClearDepth(d);
	log::message::minor << "Default depth was changed to " << d << '.';
}
void clap::gl::clear::set_stencil(int d) {
	glClearStencil(d);
	log::message::minor << "Default stencil was changed to " << d << '.';
}

void clap::gl::clear::target(unsigned target_mask) {
	if (target_mask > (target_mask::color_buffer | target_mask::depth_buffer | target_mask::stencil_buffer)) {
		log::warning::critical << "Invalid target mask was passed to 'clap::gl::clear::target'";
		return;
	}

	glClear(
		(target_mask & target_mask::color_buffer ? GL_COLOR_BUFFER_BIT : 0) |
		(target_mask & target_mask::depth_buffer ? GL_DEPTH_BUFFER_BIT : 0) |
		(target_mask & target_mask::stencil_buffer ? GL_STENCIL_BUFFER_BIT : 0)
	);

	if (target_mask & target_mask::color_buffer)
		log::message::negligible << "Color buffer was cleared.";
	if (target_mask & target_mask::depth_buffer)
		log::message::negligible << "Depth buffer was cleared.";
	if (target_mask & target_mask::stencil_buffer)
		log::message::negligible << "Stencil buffer was cleared.";
}

void clap::gl::clear::color() {
	glClear(GL_COLOR_BUFFER_BIT);
	log::message::negligible << "Clear all the colors.";
}
void clap::gl::clear::depth() {
	glClear(GL_DEPTH_BUFFER_BIT);
	log::message::negligible << "Clear all the depth.";
}
void clap::gl::clear::stencil() {
	glClear(GL_DEPTH_BUFFER_BIT);
	log::message::negligible << "Clear all the stencils.";
}
void clap::gl::clear::color_depth() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	log::message::negligible << "Clear all the colors and depth.";
}
void clap::gl::clear::all() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	log::message::negligible << "Clear all the colors, depth and stencils.";
}

﻿#include "gl/misc.hpp"
#include "gl/detail/context.hpp"

#include <algorithm>
#include <iomanip>

#include "glad/glad.h"

#include "essential/log.hpp"

void clap::gl::clear::set_color(float r, float g, float b, float a) {
	if (!gl::detail::verify_context()) return;
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
	if (!gl::detail::verify_context()) return;
	glClearDepth(d);
	log::message::minor << "Default depth was changed to " << d << '.';
}
void clap::gl::clear::set_stencil(int d) {
	if (!gl::detail::verify_context()) return;
	glClearStencil(d);
	log::message::minor << "Default stencil was changed to " << d << '.';
}

void clap::gl::clear::target(unsigned target_mask) {
	if (!gl::detail::verify_context()) return;
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
	if (!gl::detail::verify_context()) return;
	glClear(GL_COLOR_BUFFER_BIT);
	log::message::negligible << "Clear all the colors.";
}
void clap::gl::clear::depth() {
	if (!gl::detail::verify_context()) return;
	glClear(GL_DEPTH_BUFFER_BIT);
	log::message::negligible << "Clear all the depth.";
}
void clap::gl::clear::stencil() {
	if (!gl::detail::verify_context()) return;
	glClear(GL_DEPTH_BUFFER_BIT);
	log::message::negligible << "Clear all the stencils.";
}
void clap::gl::clear::color_depth() {
	if (!gl::detail::verify_context()) return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	log::message::negligible << "Clear all the colors and depth.";
}
void clap::gl::clear::all() {
	if (!gl::detail::verify_context()) return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	log::message::negligible << "Clear all the colors, depth and stencils.";
}

void clap::gl::enable::blend() {
	if (!gl::detail::verify_context()) return;
	glEnable(GL_BLEND);
	clap::log::message::minor << "Framebuffer blending was enabled.";
}
void clap::gl::enable::cull_face() {
	if (!gl::detail::verify_context()) return;
	glEnable(GL_CULL_FACE);
	clap::log::message::minor << "Face culling was enabled.";
}
void clap::gl::enable::depth_clamp() {
	if (!gl::detail::verify_context()) return;
	glEnable(GL_DEPTH_CLAMP);
	clap::log::message::minor << "Depth value clamping was enabled.";
}
void clap::gl::enable::depth_test() {
	if (!gl::detail::verify_context()) return;
	glEnable(GL_DEPTH_TEST);
	clap::log::message::minor << "Depth testing was enabled.";
}
void clap::gl::enable::dither() {
	if (!gl::detail::verify_context()) return;
	glEnable(GL_DITHER);
	clap::log::message::minor << "Color value dithering was enabled.";
}
void clap::gl::enable::framebuffer_srgb() {
	if (!gl::detail::verify_context()) return;
	glEnable(GL_FRAMEBUFFER_SRGB);
	clap::log::message::minor << "SRGB framebuffers were enabled.";
}
void clap::gl::enable::line_smooth() {
	if (!gl::detail::verify_context()) return;
	glEnable(GL_LINE_SMOOTH);
	clap::log::message::minor << "Line smothing was enabled.";
}
void clap::gl::enable::multisample() {
	if (!gl::detail::verify_context()) return;
	glEnable(GL_MULTISAMPLE);
	clap::log::message::minor << "Multisampling was enabled.";
}
void clap::gl::enable::polygon_smooth() {
	if (!gl::detail::verify_context()) return;
	glEnable(GL_POLYGON_SMOOTH);
	clap::log::message::minor << "Polygon smothing was enabled.";
}
void clap::gl::enable::scissor_test() {
	if (!gl::detail::verify_context()) return;
	glEnable(GL_SCISSOR_TEST);
	clap::log::message::minor << "Scissor testing was enabled.";
}
void clap::gl::disable::blend() {
	if (!gl::detail::verify_context()) return;
	glDisable(GL_BLEND);
	clap::log::message::minor << "Framebuffer blending was disabled.";
}
void clap::gl::disable::cull_face() {
	if (!gl::detail::verify_context()) return;
	glDisable(GL_CULL_FACE);
	clap::log::message::minor << "Face culling was disabled";
}
void clap::gl::disable::depth_clamp() {
	if (!gl::detail::verify_context()) return;
	glDisable(GL_DEPTH_CLAMP);
	clap::log::message::minor << "Depth value clamping was disabled.";
}
void clap::gl::disable::depth_test() {
	if (!gl::detail::verify_context()) return;
	glDisable(GL_DEPTH_TEST);
	clap::log::message::minor << "Depth testing was disabled.";
}
void clap::gl::disable::dither() {
	if (!gl::detail::verify_context()) return;
	glDisable(GL_DITHER);
	clap::log::message::minor << "Color value dithering was disabled.";
}
void clap::gl::disable::framebuffer_srgb() {
	if (!gl::detail::verify_context()) return;
	glDisable(GL_FRAMEBUFFER_SRGB);
	clap::log::message::minor << "SRGB framebuffers were disabled.";
}
void clap::gl::disable::line_smooth() {
	if (!gl::detail::verify_context()) return;
	glDisable(GL_LINE_SMOOTH);
	clap::log::message::minor << "Line smothing was disabled.";
}
void clap::gl::disable::multisample() {
	if (!gl::detail::verify_context()) return;
	glDisable(GL_MULTISAMPLE);
	clap::log::message::minor << "Multisampling was disabled.";
}
void clap::gl::disable::polygon_smooth() {
	if (!gl::detail::verify_context()) return;
	glDisable(GL_POLYGON_SMOOTH);
	clap::log::message::minor << "Polygon smothing was disabled.";
}
void clap::gl::disable::scissor_test() {
	if (!gl::detail::verify_context()) return;
	glDisable(GL_SCISSOR_TEST);
	clap::log::message::minor << "Scissor testing was disabled.";
}

void clap::gl::enable::blend(detail::blend_function source, detail::blend_function destination) {
	if (!gl::detail::verify_context()) return;
	clap::gl::enable::blend();
	glBlendFunc(clap::gl::detail::convert::to_gl(source), 
				clap::gl::detail::convert::to_gl(destination));
	clap::log::message::minor << "Framebuffer blending function was changed.";
	clap::log::info::critical << "New source function: " << source << ".";
	clap::log::info::critical << "New destination function: " << destination << ".";
}

void clap::gl::enable::cull_face(face face) {
	if (!gl::detail::verify_context()) return;
	clap::gl::enable::cull_face();
	glCullFace(clap::gl::detail::convert::to_gl(face));
	clap::log::message::minor << "Face culling candidates were changed to " << face << ".";
}

void clap::gl::enable::depth_clamp(double near, double far) {
	if (!gl::detail::verify_context()) return;
	clap::gl::enable::depth_clamp();
	glDepthRange(near, far);
	clap::log::message::minor << "Depth clamping range was set to (" << near << ", " << far << ").";
}
void clap::gl::enable::depth_test(depth_function function) {
	if (!gl::detail::verify_context()) return;
	clap::gl::enable::depth_clamp();
	glDepthFunc(clap::gl::detail::convert::to_gl(function));
	clap::log::message::minor << "Depth test was changed. It's now considered to " << function << ".";
}

void clap::gl::enable::scissor_test(signed x, signed y, unsigned width, unsigned height) {
	if (!gl::detail::verify_context()) return;
	clap::gl::enable::scissor_test();
	glScissor(x, y, width, height);
	clap::log::message::minor << "Scissor test rectangle was changed to "
		"(" << x << ", " << y << "), " 
		"(" << x + width << ", " << y + height << ").";
}

void clap::gl::set_line_width(float width) {
	if (!gl::detail::verify_context()) return;
	glLineWidth(width);
	clap::log::message::minor << "Line width was changed to " << width << ".";
}

void clap::gl::update_viewport(int offset_x, int offset_y, size_t width, size_t height) {
	if (!gl::detail::verify_context()) return;
	glViewport(offset_x, offset_y, GLsizei(width), GLsizei(height));
	clap::log::message::minor << "Viewport was changed to (" << width << ", " << height << ").";
}

void clap::gl::update_viewport(size_t width, size_t height) {
	update_viewport(0, 0, width, height);
}

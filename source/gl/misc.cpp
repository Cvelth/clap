#include "gl/misc.hpp"
#include "gl/detail/context.hpp"
#include "gl/interface.hpp"

#include <algorithm>
#include <iomanip>

#include "glad/glad.h"

#include "essential/log.hpp"

void clap::gl::clear::set_color(float r, float g, float b, float a) {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glClearColor(r, g, b, a);
		log::message::minor << "Change default (background) color to #"
			<< std::hex
			<< std::setw(2) << std::setfill('0') << std::clamp(static_cast<unsigned>(a * 255.f), 0u, 255u)
			<< std::setw(2) << std::setfill('0') << std::clamp(static_cast<unsigned>(r * 255.f), 0u, 255u)
			<< std::setw(2) << std::setfill('0') << std::clamp(static_cast<unsigned>(g * 255.f), 0u, 255u)
			<< std::setw(2) << std::setfill('0') << std::clamp(static_cast<unsigned>(b * 255.f), 0u, 255u)
			<< std::dec << '.';
	}
}
void clap::gl::clear::set_depth(double d) {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glClearDepth(d);
		log::message::minor << "Change default depth to " << d << '.';
	}
}
void clap::gl::clear::set_stencil(int d) {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glClearStencil(d);
		log::message::minor << "Change default stencil to " << d << '.';
	}
}

void clap::gl::clear::target(unsigned target_mask) {
	if (auto context = detail::object_interface::access_context_s(); context) {
		if (target_mask > (target_mask::color_buffer | target_mask::depth_buffer | target_mask::stencil_buffer)) {
			log::warning::critical << "Call 'clap::gl::clear::target' with invalid target mask.";
			return;
		}

		glClear(
			(target_mask & target_mask::color_buffer ? GL_COLOR_BUFFER_BIT : 0) |
			(target_mask & target_mask::depth_buffer ? GL_DEPTH_BUFFER_BIT : 0) |
			(target_mask & target_mask::stencil_buffer ? GL_STENCIL_BUFFER_BIT : 0)
		);

		if (target_mask & target_mask::color_buffer)
			log::message::negligible << "Clear the color buffer.";
		if (target_mask & target_mask::depth_buffer)
			log::message::negligible << "Clear the depth buffer.";
		if (target_mask & target_mask::stencil_buffer)
			log::message::negligible << "Clear the stencil buffer.";
	}
}

void clap::gl::clear::color() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glClear(GL_COLOR_BUFFER_BIT);
		log::message::negligible << "Clear all the color values.";
	}
}
void clap::gl::clear::depth() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glClear(GL_DEPTH_BUFFER_BIT);
		log::message::negligible << "Clear all the depth values.";
	}
}
void clap::gl::clear::stencil() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glClear(GL_DEPTH_BUFFER_BIT);
		log::message::negligible << "Clear all the stencil values.";
	}
}
void clap::gl::clear::color_depth() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		log::message::negligible << "Clear all the color and depth values.";
	}
}
void clap::gl::clear::all() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		log::message::negligible << "Clear all the color, depth and stencil values.";
	}
}

void clap::gl::enable::blend() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glEnable(GL_BLEND);
		clap::log::message::minor << "Enable framebuffer blending.";
	}
}
void clap::gl::enable::cull_face() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glEnable(GL_CULL_FACE);
		clap::log::message::minor << "Enable face culling.";
	}
}
void clap::gl::enable::depth_clamp() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glEnable(GL_DEPTH_CLAMP);
		clap::log::message::minor << "Enable depth value clamping.";
	}
}
void clap::gl::enable::depth_test() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glEnable(GL_DEPTH_TEST);
		clap::log::message::minor << "Enable depth testing.";
	}
}
void clap::gl::enable::dither() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glEnable(GL_DITHER);
		clap::log::message::minor << "Enable color dithering.";
	}
}
void clap::gl::enable::framebuffer_srgb() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glEnable(GL_FRAMEBUFFER_SRGB);
		clap::log::message::minor << "Enable SRGB framebuffers.";
	}
}
void clap::gl::enable::line_smooth() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glEnable(GL_LINE_SMOOTH);
		clap::log::message::minor << "Enable line smothing.";
	}
}
void clap::gl::enable::multisample() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glEnable(GL_MULTISAMPLE);
		clap::log::message::minor << "Enable ,ultisampling.";
	}
}
void clap::gl::enable::polygon_smooth() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glEnable(GL_POLYGON_SMOOTH);
		clap::log::message::minor << "Enable polygon smothing.";
	}
}
void clap::gl::enable::scissor_test() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glEnable(GL_SCISSOR_TEST);
		clap::log::message::minor << "Enable scissor testing.";
	}
}
void clap::gl::disable::blend() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDisable(GL_BLEND);
		clap::log::message::minor << "Disable framebuffer blending.";
	}
}
void clap::gl::disable::cull_face() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDisable(GL_CULL_FACE);
		clap::log::message::minor << "Disable face culling";
	}
}
void clap::gl::disable::depth_clamp() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDisable(GL_DEPTH_CLAMP);
		clap::log::message::minor << "Disable depth value clamping.";
	}
}
void clap::gl::disable::depth_test() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDisable(GL_DEPTH_TEST);
		clap::log::message::minor << "Disable depth testing.";
	}
}
void clap::gl::disable::dither() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDisable(GL_DITHER);
		clap::log::message::minor << "Disable color value dithering.";
	}
}
void clap::gl::disable::framebuffer_srgb() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDisable(GL_FRAMEBUFFER_SRGB);
		clap::log::message::minor << "Disable SRGB framebuffers.";
	}
}
void clap::gl::disable::line_smooth() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDisable(GL_LINE_SMOOTH);
		clap::log::message::minor << "Disable line smothing.";
	}
}
void clap::gl::disable::multisample() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDisable(GL_MULTISAMPLE);
		clap::log::message::minor << "Disable multisampling.";
	}
}
void clap::gl::disable::polygon_smooth() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDisable(GL_POLYGON_SMOOTH);
		clap::log::message::minor << "Disable polygon smothing.";
	}
}
void clap::gl::disable::scissor_test() {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDisable(GL_SCISSOR_TEST);
		clap::log::message::minor << "Disable scissor testing.";
	}
}

void clap::gl::enable::blend(detail::blend_function source, detail::blend_function destination) {
	clap::gl::enable::blend();
	if (auto context = detail::object_interface::access_context_s(); context) {
		glBlendFunc(clap::gl::detail::convert::to_gl(source),
					clap::gl::detail::convert::to_gl(destination));
		clap::log::message::minor << "Change framebuffer blending function.";
		clap::log::info::critical << "New source function: " << source << ".";
		clap::log::info::critical << "New destination function: " << destination << ".";
	}
}

void clap::gl::enable::cull_face(face face) {
	clap::gl::enable::cull_face();
	if (auto context = detail::object_interface::access_context_s(); context) {
		glCullFace(clap::gl::detail::convert::to_gl(face));
		clap::log::message::minor << "Change face culling candidates to " << face << ".";
	}
}

void clap::gl::enable::depth_clamp(double near, double far) {
	clap::gl::enable::depth_clamp();
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDepthRange(near, far);
		clap::log::message::minor << "Set depth clamping range to (" << near << ", " << far << ").";
	}
}
void clap::gl::enable::depth_test(depth_function function) {
	clap::gl::enable::depth_clamp();
	if (auto context = detail::object_interface::access_context_s(); context) {
		glDepthFunc(clap::gl::detail::convert::to_gl(function));
		clap::log::message::minor << "Change depth test.";
		clap::log::info::major << "The test is " << function << ".";
	}
}

void clap::gl::enable::scissor_test(signed x, signed y, unsigned width, unsigned height) {
	clap::gl::enable::scissor_test();
	if (auto context = detail::object_interface::access_context_s(); context) {
		glScissor(x, y, width, height);
		clap::log::message::minor << "Changle scissor to "
			"((" << x << ", " << y << "), "
			"(" << x + width << ", " << y + height << ")).";
	}
}

void clap::gl::set_line_width(float width) {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glLineWidth(width);
		clap::log::message::minor << "Changle line width to " << width << ".";
	}
}

void clap::gl::update_viewport(int offset_x, int offset_y, size_t width, size_t height) {
	if (auto context = detail::object_interface::access_context_s(); context) {
		glViewport(offset_x, offset_y, GLsizei(width), GLsizei(height));
		clap::log::message::minor << "Changle viewport to "
			"((" << offset_x << ", " << offset_y << "), "
			"(" << offset_x + width << ", " << offset_y + height << ")).";
	}
}

void clap::gl::update_viewport(size_t width, size_t height) {
	update_viewport(0, 0, width, height);
}

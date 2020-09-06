#include "window.hpp"
#include "resource/resource.hpp"

#include <chrono>
#include <thread>

//FORCE_NVIDIA_GPU_ON_OPTIMUS;

void window::initialize() {
	clap::resource::load();

	using namespace clap::gl;
	clear::set_color(0.4f, 0.0f, 0.5f, 1.0f);

	auto &texture = clap::resource::texture::_2d["logo"];
	texture.set_min_filter(texture::min_filter::linear_mipmap_linear);
	texture.set_mag_filter(texture::mag_filter::linear);
	texture.bind();
	
	program.add(clap::resource::shader::fragment["triangle"], 
				clap::resource::shader::vertex["triangle"]);
	program.link();

	uniforms = program.getUniforms();
	auto attributes = program.getAttributes();

	buffer::single vertices;
	static const float vertex_data[] = {
		-1.0f, -1.0f,
		1.0f, -1.0f, 
		0.0f,  1.0f, 
	};
	vertices.data(vertex_data, sizeof(vertex_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(vertices, attributes["position"]);

	buffer::single colors;
	static const float color_data[] = {
		1.f, 0.5f, 0.5f,
		0.5f, 1.f, 0.5f,
		0.5f, 0.5f, 1.f,
	};
	colors.data(color_data, sizeof(color_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(colors, attributes["color"]);

	buffer::single texture_coordinates;
	static const float texture_coordinate_data[] = {
		0.f,  1.f,
		1.f,  1.f,
		0.5f, 0.f
	};
	texture_coordinates.data(texture_coordinate_data, sizeof(texture_coordinate_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(texture_coordinates, attributes["texture_coordinates"]);

	mode = 0;
	program.use();
	program.set(uniforms["mode"], { mode });
	update();
}

void window::render() {
	using namespace clap::gl;
	auto time = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(10);

	poll_events();
	if (should_update()) {
		clear::color();
		vertex_array.draw(vertex_array::connection::triangles, 3);
		redraw();
	}

	std::this_thread::sleep_until(time);
}

void window::cleanup() {
	using namespace clap::gl;

	clap::resource::clear();
}

bool window::on_mouse_button(clap::event::mouse_button button, clap::event::mouse_button_action action, clap::event::modificator_mask modificators) {
	static unsigned old_mode = 0;
	if (action == clap::event::mouse_button_action::press) {
		old_mode = !mode;
		mode = 2;
	} else if (action == clap::event::mouse_button_action::release) {
		mode = old_mode;
		program.set(uniforms["mode"], { mode });
	}
	update();

	button; action; modificators; // To avoid "Unreferenced formal parameter" warning.
	
	return true;
}

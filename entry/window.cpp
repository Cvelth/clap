#include "window.hpp"
#include <iostream>

//#include "../dependencies/glad/glad.h"

void window::initialize() {
	using namespace engine::gl;
	clear::set_color(0.4f, 0.0f, 0.5f, 1.0f);

	program.add(shader::from_file(shader::type::vertex, "../shaders/vertex.glsl"));
	program.add(shader::from_file(shader::type::fragment, "../shaders/fragment.glsl"));
	program.link();

	//auto uniforms = program.getUniforms();
	auto attributes = program.getAttributes();

	buffer::single vertices;
	static const float vertex_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};
	vertices.data(vertex_data, sizeof(vertex_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(vertices, attributes.at("position"), 0, 0);

	buffer::single colors;
	static const float color_data[] = {
		1.f, 0.5f, 0.5f, 1.f,
		0.5f, 1.f, 0.5f, 1.f,
		0.5f, 0.5f, 1.f, 1.f,
	};
	colors.data(color_data, sizeof(color_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(colors, attributes.at("color"), 0, 0);

	program.use();
}

void window::render() {
	using namespace engine::gl;

	poll_events();
	clear::color();
	vertex_array.draw(vertex_array::connection::triangles, 3);
}

void window::cleanup() {
	using namespace engine::gl;
}

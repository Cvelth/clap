#include "window.hpp"

#include <chrono>
#include <thread>

//#include <iostream> 
//#include "../../dependencies/include/glad/glad.h"

//FORCE_NVIDIA_GPU_ON_OPTIMUS;

void window::initialize() {
	using namespace engine::gl;
	clear::set_color(0.4f, 0.0f, 0.5f, 1.0f);

	program.add(shader::from_file(shader::type::vertex, "../../resource/shader/vertex/simple_position_color.glsl"));
	program.add(shader::from_file(shader::type::fragment, "../../resource/shader/fragment/simple_color.glsl"));
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
	vertex_array.attribute_pointer(vertices, attributes["position"], 0, 0);

	buffer::single colors;
	static const float color_data[] = {
		1.f, 0.5f, 0.5f, 1.f,
		0.5f, 1.f, 0.5f, 1.f,
		0.5f, 0.5f, 1.f, 1.f,
	};
	colors.data(color_data, sizeof(color_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(colors, attributes["color"], 0, 0);

	program.use();
}

void window::render() {
	using namespace engine::gl;
	auto time = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(100);

	poll_events();
	clear::color();
	vertex_array.draw(vertex_array::connection::triangles, 3);

	std::this_thread::sleep_until(time);
}

void window::cleanup() {
	using namespace engine::gl;
}

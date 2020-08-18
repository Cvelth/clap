#include "window.hpp"
#include "resource/resource.hpp"

#include <chrono>
#include <thread>

#include "gl/texture.hpp"

//FORCE_NVIDIA_GPU_ON_OPTIMUS;

void window::initialize() {
	clap::resource::load();

	using namespace clap::gl;
	clear::set_color(0.4f, 0.0f, 0.5f, 1.0f);

	program.add(clap::resource::shader::fragment["texture_2d"],
				clap::resource::shader::vertex["texture_2d"]);
	program.link();

	auto &texture = clap::resource::texture::_2d["logo"];
	texture.set_min_filter(texture::min_filter::linear_mipmap_linear);
	texture.set_mag_filter(texture::mag_filter::linear);
	texture.bind();

	auto uniforms = program.getUniforms();
	auto attributes = program.getAttributes();

	buffer::single vertices;
	static const float vertex_data[] = {
		-1.0f, -1.0f,  0.0f,
		 1.0f, -1.0f,  0.0f,
		 0.0f,  1.0f,  0.0f,
	};
	vertices.data(vertex_data, sizeof(vertex_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(vertices, attributes["position"], 0, 0);

	buffer::single texture_coordinates;
	static const float texture_coordinate_data[] = {
		0.f,  1.f,
		1.f,  1.f,
		0.5f, 0.f
	};
	texture_coordinates.data(texture_coordinate_data, sizeof(texture_coordinate_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(texture_coordinates, attributes["texture_coordinates"], 0, 0);

	program.use();
}

void window::render() {
	using namespace clap::gl;
	auto time = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(100);

	poll_events();
	clear::color();
	vertex_array.draw(vertex_array::connection::triangles, 3);

	std::this_thread::sleep_until(time);
}

void window::cleanup() {
	using namespace clap::gl;

	clap::resource::clear();
}

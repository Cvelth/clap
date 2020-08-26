#include "window.hpp"
#include "resource/resource.hpp"

#include <chrono>
#include <thread>

#include "gl/texture.hpp"
#include "render/font.hpp"

//FORCE_NVIDIA_GPU_ON_OPTIMUS;

void window::initialize() {
	using namespace clap::gl;

	clap::resource::load();
	clear::set_color(0.4f, 0.0f, 0.5f, 1.0f);

	program.add(clap::resource::shader::fragment["text"],
				clap::resource::shader::vertex["text"]);
	program.link();
	variables = program.getVariables();
	program.use();

	auto &texture = clap::resource::texture::_2d["logo"];
	texture.set_min_filter(texture::min_filter::linear_mipmap_linear);
	texture.set_mag_filter(texture::mag_filter::linear);
	texture.bind();

	buffer::single vertices;
	static const float vertex_data[] = {
		20, 20,
		20, height() - 20,
		width() - 20, 20,
		width() - 20, height() - 20
	};
	vertices.data(vertex_data, sizeof(vertex_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(vertices, variables["position"], 0, 0);

	buffer::single texture_coordinates;
	static const float texture_coordinate_data[] = {
		0.f, 1.f,
		0.f, 0.f,
		1.f, 1.f,
		1.f, 0.f,
	};
	texture_coordinates.data(texture_coordinate_data, sizeof(texture_coordinate_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(texture_coordinates, variables["texture_coordinates"], 0, 0);

	on_resize(width(), height());
}

void window::render() {
	using namespace clap::gl;
	auto time = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(100);

	poll_events();
	clear::color();
	vertex_array.draw(vertex_array::connection::triangle_strip, 4);

	std::this_thread::sleep_until(time);
}

void window::cleanup() {
	using namespace clap::gl;

	clap::resource::clear();
}

void window::on_resize(size_t width, size_t height) {
	using namespace clap::gl;

	float projection_matrix[] = {
		2.f / width,	0.f,			0.f,	0.f,
		0.f,			2.f / height,	0.f,	0.f,
		0.f,			0.f,			-1.f,	0.f,
		-1.f,			-1.f,			0.f,	1.f
	};

	program.set(variables["projection_matrix"], projection_matrix);
}

#include "window.hpp"
#include "resource/resource.hpp"

#include <chrono>
#include <thread>

//FORCE_NVIDIA_GPU_ON_OPTIMUS;

void window::initialize() {
	using namespace clap::gl;

	clap::resource::load();
	clear::set_color(0.4f, 0.0f, 0.5f, 1.0f);


	text_program.add(clap::resource::shader::fragment["text"],
					 clap::resource::shader::vertex["text"]);
	text_program.link();

	text_atlas_program.add(clap::resource::shader::fragment["text_atlas"],
						   clap::resource::shader::vertex["text_atlas"]);
	text_atlas_program.link();
	variables = text_atlas_program.getVariables();
	text_atlas_program.use();

	clap::gl::enable::blend(clap::gl::blend_function::source_alpha);
	text = std::make_unique<clap::render::text>(u8"A simple example. 日本語もここだよ!",
												clap::resource::font["GL-AntiquePlus"],
												text_program, 48);
	text->move(width() / 8, height() / 2);

	buffer::single vertices;
	static const float vertex_data[] = {
		20, 20,
		20, height() - 20,
		width() - 20, 20,
		width() - 20, height() - 20
	};
	vertices.data(vertex_data, sizeof(vertex_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(vertices, variables["position"]);

	buffer::single texture_coordinates;
	static const float texture_coordinate_data[] = {
		0.f, 1.f * (height() - 40) / clap::gl::texture::_2d::maximum_height(),
		0.f, 0.f,
		1.f * (width() - 40) / clap::gl::texture::_2d::maximum_width(), 1.f * (height() - 40) / clap::gl::texture::_2d::maximum_height(),
		1.f * (width() - 40) / clap::gl::texture::_2d::maximum_width(), 0.f,
	};
	texture_coordinates.data(texture_coordinate_data, sizeof(texture_coordinate_data), buffer::usage::static_draw);
	vertex_array.attribute_pointer(texture_coordinates, variables["texture_coordinates"]);

	on_resize(width(), height());
}

void window::render() {
	using namespace clap::gl;
	auto time = std::chrono::high_resolution_clock::now() + std::chrono::milliseconds(10);

	poll_events();
	if (should_update()) {
		clear::color();

		text->draw();

		text_atlas_program.use();
		vertex_array.draw(vertex_array::connection::triangle_strip, 4);
	}

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

	text_atlas_program.set(variables["projection_matrix"], projection_matrix);
	text_program.set(variables["projection_matrix"], projection_matrix);

	update();
}

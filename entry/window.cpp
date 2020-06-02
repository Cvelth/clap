#include "window.hpp"
#include "../dependencies/glad/glad.h"
#include "../engine/gl/buffer.hpp"
#include "../engine/gl/shader.hpp"

#include <iostream>

void window::initialize() {
	using namespace engine::gl;

	glClearColor(0.4f, 0.0f, 0.5f, 1.0f);

	shader::program program;
	program.add(shader::from_file(shader::type::vertex, "../shaders/vertex.glsl"));
	program.add(shader::from_file(shader::type::fragment, "../shaders/fragment.glsl"));

	program.link();

	auto uniforms = program.getUniforms();
	auto attributes = program.getAttributes();

	GLuint vertex_array;
	glGenVertexArrays(1, &vertex_array);
	glBindVertexArray(vertex_array);

	buffer::single vertices;
	static const GLfloat vertex_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};
	vertices.data(vertex_data, sizeof(vertex_data), buffer::usage::static_draw);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	buffer::single colors;
	static const GLfloat color_data[] = {
		1.f, 0.5f, 0.5f, 1.f,
		0.5f, 1.f, 0.5f, 1.f,
		0.5f, 0.5f, 1.f, 1.f,
	};
	colors.data(color_data, sizeof(color_data), buffer::usage::static_draw);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);

	program.use();
}

void window::render() {
	poll_events();

	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void window::cleanup() {
	glDisableVertexAttribArray(0);
}

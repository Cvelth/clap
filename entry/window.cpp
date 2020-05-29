#include "window.hpp"
#include "../dependencies/glad/glad.h"
#include "../engine/gl/buffer.hpp"

void window::initialize() {
	glClearColor(0.6f, 0.1f, 0.7f, 1.0f);

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	};

	using namespace engine::gl;

	buffer::single vertexbuffer;

	vertexbuffer.data(g_vertex_buffer_data, sizeof(g_vertex_buffer_data), buffer::usage::static_draw);

	glEnableVertexAttribArray(0);
	vertexbuffer.bind();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
}

void window::render() {
	poll_events();

	glClear(GL_COLOR_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void window::cleanup() {
	glDisableVertexAttribArray(0);
}

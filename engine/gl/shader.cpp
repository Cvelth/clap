#include "shader.hpp"
#include "state.hpp"
#include "../error.hpp"

#include "../../dependencies/glad/glad.h"

#include <fstream>
#include <string>

engine::gl::shader::detail::object::object(type type) : id(uint32_t(-1)) {
	gl::state::ensure_loaded();

	id = glCreateShader(gl::detail::convert::to_gl(type));
	if (id == 0 || id == -1)
		error::critical("Shader Object creation failed.");
}

engine::gl::shader::detail::object::~object() {
	glDeleteShader(id);
}

engine::gl::shader::detail::object::object(type type, std::string source) : object(type) {
	if (source == "")
		error::critical("Shader source is empty.");

	const GLchar *_source = static_cast<const GLchar *>(source.c_str());
	glShaderSource(id, 1, &_source, NULL);
	glCompileShader(id);

	GLint isCompiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

	if (!isCompiled) {
		GLsizei len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

		GLchar *log = new GLchar[len + 1];
		glGetShaderInfoLog(id, len, &len, log);
		error::critical("(during shader compilation) " + std::string(log));
		delete[] log;
	}
}

engine::gl::shader::detail::object::object(uint32_t id) : id(id) {
	gl::state::ensure_loaded();

	if (id == 0 || !glIsShader(id))
		error::critical("Unable to perform a shader object move operation. "
						"Passed shader object seems to be corrupted.");
}

engine::gl::shader::detail::object engine::gl::shader::from_source(type type, std::string source) {
	return detail::object(type, source);
}
engine::gl::shader::detail::object engine::gl::shader::from_source(type type, std::string_view source) {
	return from_source(type, std::string(source));
}

engine::gl::shader::detail::object engine::gl::shader::from_source(type type, char const *source) {
	return from_source(type, std::string(source));
}

engine::gl::shader::detail::object engine::gl::shader::from_file(type type, std::string filename) {
	std::ifstream filestream;
	filestream.open(filename);
	if (!filestream)
		error::critical("File " + filename + " cannot be oppened. Make sure it exists.");
	std::string source{
		std::istreambuf_iterator<char>(filestream),
		std::istreambuf_iterator<char>()
	};

	return from_source(type, source);
}
engine::gl::shader::detail::object engine::gl::shader::from_file(type type, std::string_view filename) {
	return from_file(type, std::string(filename));
}

engine::gl::shader::detail::object engine::gl::shader::from_file(type type, char const *filename) {
	return from_file(type, std::string(filename));
}

GLenum engine::gl::detail::convert::to_gl(engine::gl::shader::type v) {
	switch (v) {
		case engine::gl::shader::type::fragment:				return GL_FRAGMENT_SHADER;
		case engine::gl::shader::type::vertex:					return GL_VERTEX_SHADER;
		case engine::gl::shader::type::geometry:				return GL_GEOMETRY_SHADER;
		case engine::gl::shader::type::compute:					return GL_COMPUTE_SHADER;
		case engine::gl::shader::type::tesselation_control:		return GL_TESS_CONTROL_SHADER;
		case engine::gl::shader::type::tesselation_evaluation:	return GL_TESS_EVALUATION_SHADER;
	}
	engine::error::critical("Unsupported enum value.");
}
engine::gl::shader::type engine::gl::detail::convert::to_shader_type(GLenum v) {
	switch (v) {
		case GL_FRAGMENT_SHADER:		return engine::gl::shader::type::fragment;
		case GL_VERTEX_SHADER:			return engine::gl::shader::type::vertex;
		case GL_GEOMETRY_SHADER:		return engine::gl::shader::type::geometry;
		case GL_COMPUTE_SHADER:			return engine::gl::shader::type::compute;
		case GL_TESS_CONTROL_SHADER:	return engine::gl::shader::type::tesselation_control;
		case GL_TESS_EVALUATION_SHADER: return engine::gl::shader::type::tesselation_evaluation;
	}
	engine::error::critical("Unsupported enum value.");
}

engine::gl::shader::program::program() : id(uint32_t(-1)), needs_linking(true) {
	gl::state::ensure_loaded();

	id = glCreateProgram();
	if (id == 0 || id == -1)
		error::critical("Shader Program Object creation failed.");
}
engine::gl::shader::program::~program() {
	glDeleteProgram(id);
}

void engine::gl::shader::program::add(detail::object &&object) {
	glAttachShader(id, object.id);
	needs_linking = true;
}

void engine::gl::shader::program::link() {
	if (needs_linking) {
		glLinkProgram(id);

		GLint isLinked;
		glGetProgramiv(id, GL_LINK_STATUS, &isLinked);
		if (!isLinked) {
			GLsizei len;
			glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);

			GLchar *log = new GLchar[len + 1];
			glGetProgramInfoLog(id, len, &len, log);
			error::critical("(during program linking) " + std::string(log));
			delete[] log;
		}
		needs_linking = false;
	} else
		error::warn("Program is already linked. Repetition is not needed.");
}

void engine::gl::shader::program::use() {
	if (!needs_linking)
		state::use(this);
	else
		error::warn("Attempting to use an unlinked program.");
}

engine::gl::shader::program::program(uint32_t id) : id(id), needs_linking(true) {
	gl::state::ensure_loaded();

	if (id == 0 || !glIsProgram(id))
		error::critical("Unable to perform a shader program move operation. "
						"Passed shader program seems to be corrupted.");
}
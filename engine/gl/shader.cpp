#include "shader.hpp"
#include "state.hpp"
#include "../error.hpp"

#include "../../dependencies/glad/glad.h"

#include <fstream>
#include <string>
#include <vector>

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

size_t engine::gl::shader::variable::size() const {
	return dimentions.x * dimentions.y * gl::detail::convert::to_size(datatype);
}

engine::gl::shader::variable::variable(std::string const &name, storage_type const &storage,
									   uint32_t location, datatype_t const &datatype_name,
									   dimentions_t const &dimentions)
	: name(name), type(storage), location(location),
	datatype(datatype_name), dimentions(dimentions) {
	if (dimentions.x < 1 || dimentions.x > 4 || dimentions.y < 1 || dimentions.y > 4)
		error::critical("Variable dimentions must be in [1; 4] range.");
	if (dimentions.x > 1 && (datatype_name != datatype_t::_float || datatype_name != datatype_t::_double))
		error::critical("Matrix variables can only be 'float's or 'double's.");
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

template<typename gl_get_lambda_t, typename create_variable_lambda_t>
std::map<std::string, engine::gl::shader::variable> local_get(uint32_t program_id, GLenum active_type,
															  GLenum active_type_length, gl_get_lambda_t gl_get_lambda,
															  create_variable_lambda_t create_variable_lambda) {
	GLint number, max_length, name_length, size;
	GLenum type;

	std::map<std::string, engine::gl::shader::variable> out;

	glGetProgramiv(program_id, active_type, &number);
	glGetProgramiv(program_id, active_type_length, &max_length);
	for (int i = 0; i < number; i++) {
		std::vector<GLchar> name(max_length);
		gl_get_lambda(program_id, i, max_length, &name_length, &size, &type, name.data());
		out.insert(std::make_pair(name.data(), create_variable_lambda(name.data(), i, type)));
	}
	return out;
}
std::map<std::string, engine::gl::shader::variable> engine::gl::shader::program::getUniforms() {
	return local_get(id, GL_ACTIVE_UNIFORMS, GL_ACTIVE_UNIFORM_MAX_LENGTH,
					 glGetActiveUniform, [](auto name, auto location, auto datatype) {
						 auto datatype_pair = gl::detail::convert::to_variable_datatype_pair(datatype);
						 return shader::variable(name, variable::storage_type::uniform, location,
												 datatype_pair.first, datatype_pair.second);
					 });
}
std::map<std::string, engine::gl::shader::variable> engine::gl::shader::program::getAttributes() {
	return local_get(id, GL_ACTIVE_ATTRIBUTES, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
					 glGetActiveAttrib, [](auto name, auto location, auto datatype) {
						 auto datatype_pair = gl::detail::convert::to_variable_datatype_pair(datatype);
						 return shader::variable(name, variable::storage_type::attribute, location,
												 datatype_pair.first, datatype_pair.second);
					 });
}
std::map<std::string, engine::gl::shader::variable> engine::gl::shader::program::getVariables() {
	auto out = getUniforms();
	out.merge(getAttributes());
	return out;
}

engine::gl::shader::program::program(uint32_t id) : id(id), needs_linking(true) {
	gl::state::ensure_loaded();

	if (id == 0 || !glIsProgram(id))
		error::critical("Unable to perform a shader program move operation. "
						"Passed shader program seems to be corrupted.");
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

GLenum engine::gl::detail::convert::to_gl(shader::variable::datatype_t datatype,
										  shader::variable::dimentions_t dimentions) {
	switch (datatype) {
		case engine::gl::shader::variable::datatype_t::_float:
			switch (dimentions.x) {
				case 1:
					switch (dimentions.y) {
						case 1: return GL_FLOAT;
						case 2: return GL_FLOAT_VEC2;
						case 3: return GL_FLOAT_VEC3;
						case 4: return GL_FLOAT_VEC4;
					}
				case 2:
					switch (dimentions.y) {
						case 2: return GL_FLOAT_MAT2;
						case 3: return GL_FLOAT_MAT2x3;
						case 4: return GL_FLOAT_MAT2x4;
					}
				case 3:
					switch (dimentions.y) {
						case 2: return GL_FLOAT_MAT3x2;
						case 3: return GL_FLOAT_MAT3;
						case 4: return GL_FLOAT_MAT3x4;
					}
				case 4:
					switch (dimentions.y) {
						case 2: return GL_FLOAT_MAT4x2;
						case 3: return GL_FLOAT_MAT4x3;
						case 4: return GL_FLOAT_MAT4;
					}
			}
		case engine::gl::shader::variable::datatype_t::_double:
			switch (dimentions.x) {
				case 1:
					switch (dimentions.y) {
						case 1: return GL_DOUBLE;
						case 2: return GL_DOUBLE_VEC2;
						case 3: return GL_DOUBLE_VEC3;
						case 4: return GL_DOUBLE_VEC4;
					}
				case 2:
					switch (dimentions.y) {
						case 2: return GL_DOUBLE_MAT2;
						case 3: return GL_DOUBLE_MAT2x3;
						case 4: return GL_DOUBLE_MAT2x4;
					}
				case 3:
					switch (dimentions.y) {
						case 2: return GL_DOUBLE_MAT3x2;
						case 3: return GL_DOUBLE_MAT3;
						case 4: return GL_DOUBLE_MAT3x4;
					}
				case 4:
					switch (dimentions.y) {
						case 2: return GL_DOUBLE_MAT4x2;
						case 3: return GL_DOUBLE_MAT4x3;
						case 4: return GL_DOUBLE_MAT4;
					}
			}
		case engine::gl::shader::variable::datatype_t::_int:
			switch (dimentions.x) {
				case 1:
					switch (dimentions.y) {
						case 1: return GL_INT;
						case 2: return GL_INT_VEC2;
						case 3: return GL_INT_VEC3;
						case 4: return GL_INT_VEC4;
					}
			}
		case engine::gl::shader::variable::datatype_t::_unsigned:
			switch (dimentions.x) {
				case 1:
					switch (dimentions.y) {
						case 1: return GL_UNSIGNED_INT;
						case 2: return GL_UNSIGNED_INT_VEC2;
						case 3: return GL_UNSIGNED_INT_VEC3;
						case 4: return GL_UNSIGNED_INT_VEC4;
					}
			}
		case engine::gl::shader::variable::datatype_t::_bool:
			switch (dimentions.x) {
				case 1:
					switch (dimentions.y) {
						case 1: return GL_BOOL;
						case 2: return GL_BOOL_VEC2;
						case 3: return GL_BOOL_VEC3;
						case 4: return GL_BOOL_VEC4;
					}
			}
	}
	engine::error::critical("Unsupported enum value.");
}

std::pair<engine::gl::shader::variable::datatype_t, engine::gl::shader::variable::dimentions_t>
engine::gl::detail::convert::to_variable_datatype_pair(GLenum v) {
	auto make_pair = [](shader::variable::datatype_t datatype, size_t x, size_t y) {
		return std::make_pair(datatype, shader::variable::dimentions_t{ x, y });
	};
	switch (v) {
		case GL_FLOAT: return make_pair(shader::variable::datatype_t::_float, 1, 1);
		case GL_FLOAT_VEC2: return make_pair(shader::variable::datatype_t::_float, 1, 2);
		case GL_FLOAT_VEC3: return make_pair(shader::variable::datatype_t::_float, 1, 3);
		case GL_FLOAT_VEC4: return make_pair(shader::variable::datatype_t::_float, 1, 4);

		case GL_DOUBLE: return make_pair(shader::variable::datatype_t::_double, 1, 1);
		case GL_DOUBLE_VEC2: return make_pair(shader::variable::datatype_t::_double, 1, 2);
		case GL_DOUBLE_VEC3: return make_pair(shader::variable::datatype_t::_double, 1, 3);
		case GL_DOUBLE_VEC4: return make_pair(shader::variable::datatype_t::_double, 1, 4);

		case GL_INT: return make_pair(shader::variable::datatype_t::_int, 1, 1);
		case GL_INT_VEC2: return make_pair(shader::variable::datatype_t::_int, 1, 2);
		case GL_INT_VEC3: return make_pair(shader::variable::datatype_t::_int, 1, 3);
		case GL_INT_VEC4: return make_pair(shader::variable::datatype_t::_int, 1, 4);

		case GL_UNSIGNED_INT: return make_pair(shader::variable::datatype_t::_unsigned, 1, 1);
		case GL_UNSIGNED_INT_VEC2: return make_pair(shader::variable::datatype_t::_unsigned, 1, 2);
		case GL_UNSIGNED_INT_VEC3: return make_pair(shader::variable::datatype_t::_unsigned, 1, 3);
		case GL_UNSIGNED_INT_VEC4: return make_pair(shader::variable::datatype_t::_unsigned, 1, 4);

		case GL_BOOL: return make_pair(shader::variable::datatype_t::_bool, 1, 1);
		case GL_BOOL_VEC2: return make_pair(shader::variable::datatype_t::_bool, 1, 2);
		case GL_BOOL_VEC3: return make_pair(shader::variable::datatype_t::_bool, 1, 3);
		case GL_BOOL_VEC4: return make_pair(shader::variable::datatype_t::_bool, 1, 4);

		case GL_FLOAT_MAT2: return make_pair(shader::variable::datatype_t::_float, 2, 2);
		case GL_FLOAT_MAT3: return make_pair(shader::variable::datatype_t::_float, 3, 3);
		case GL_FLOAT_MAT4: return make_pair(shader::variable::datatype_t::_float, 4, 4);

		case GL_FLOAT_MAT2x3: return make_pair(shader::variable::datatype_t::_float, 2, 3);
		case GL_FLOAT_MAT3x2: return make_pair(shader::variable::datatype_t::_float, 3, 2);
		case GL_FLOAT_MAT4x2: return make_pair(shader::variable::datatype_t::_float, 4, 2);

		case GL_FLOAT_MAT2x4: return make_pair(shader::variable::datatype_t::_float, 2, 4);
		case GL_FLOAT_MAT3x4: return make_pair(shader::variable::datatype_t::_float, 3, 4);
		case GL_FLOAT_MAT4x3: return make_pair(shader::variable::datatype_t::_float, 4, 3);

		case GL_DOUBLE_MAT2: return make_pair(shader::variable::datatype_t::_double, 2, 2);
		case GL_DOUBLE_MAT3: return make_pair(shader::variable::datatype_t::_double, 3, 3);
		case GL_DOUBLE_MAT4: return make_pair(shader::variable::datatype_t::_double, 4, 4);

		case GL_DOUBLE_MAT2x3: return make_pair(shader::variable::datatype_t::_double, 2, 3);
		case GL_DOUBLE_MAT3x2: return make_pair(shader::variable::datatype_t::_double, 3, 2);
		case GL_DOUBLE_MAT4x2: return make_pair(shader::variable::datatype_t::_double, 4, 2);

		case GL_DOUBLE_MAT2x4: return make_pair(shader::variable::datatype_t::_double, 2, 4);
		case GL_DOUBLE_MAT3x4: return make_pair(shader::variable::datatype_t::_double, 3, 4);
		case GL_DOUBLE_MAT4x3: return make_pair(shader::variable::datatype_t::_double, 4, 3);
	}
	engine::error::critical("Unsupported enum value.");
}

GLenum engine::gl::detail::convert::to_gl(shader::variable::datatype_t datatype) {
	switch (datatype) {
		case engine::gl::shader::variable::datatype_t::_float: return GL_FLOAT;
		case engine::gl::shader::variable::datatype_t::_double: return GL_DOUBLE;
		case engine::gl::shader::variable::datatype_t::_int: return GL_INT;
		case engine::gl::shader::variable::datatype_t::_unsigned: return GL_UNSIGNED_INT;
		case engine::gl::shader::variable::datatype_t::_bool: return GL_BOOL;
	}
	engine::error::critical("Unsupported enum value.");
}

size_t engine::gl::detail::convert::to_size(shader::variable::datatype_t datatype) {
	switch (datatype) {
		case engine::gl::shader::variable::datatype_t::_float: return sizeof(float);
		case engine::gl::shader::variable::datatype_t::_double: return sizeof(double);
		case engine::gl::shader::variable::datatype_t::_int: return sizeof(int);
		case engine::gl::shader::variable::datatype_t::_unsigned: return sizeof(unsigned);
		case engine::gl::shader::variable::datatype_t::_bool: return sizeof(bool);
	}
	engine::error::critical("Unsupported enum value.");
}

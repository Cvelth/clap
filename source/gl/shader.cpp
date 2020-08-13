#include "gl/shader.hpp"

#include <fstream>
#include <string>
#include <vector>

#include "glad/gl.h"

#include "gl/detail/state.hpp"
#include "essential/log.hpp"

engine::gl::shader::detail::object::object(type type) : id(uint32_t(-1)) {
	gl::detail::state::ensure_loaded();

	id = glCreateShader(gl::detail::convert::to_gl(type));
	if (id == 0 || id == -1)
		log::warning::critical << "Shader object creation failed.";
	else
		log::message::minor << "A " << type << " shader object was created.";
}

engine::gl::shader::detail::object::~object() {
	glDeleteShader(id);
	log::message::minor << "A shader object was destroyed.";
}

engine::gl::shader::detail::object::object(type type, std::string source) : object(type) {
	if (source == "") {
		log::warning::critical << "Shader source is empty.";
		return;
	}

	const GLchar *_source = static_cast<const GLchar *>(source.c_str());
	glShaderSource(id, 1, &_source, NULL);
	glCompileShader(id);

	GLint isCompiled;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

	if (!isCompiled) {
		GLsizei len;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);

		GLchar *log = new GLchar[len + GLsizei(1)];
		glGetShaderInfoLog(id, len, &len, log);
		log::warning::critical << "During shader compilation: " << std::string(log);
		delete[] log;
	} else
		log::message::minor << "A shader was compiled successfully.";
}

engine::gl::shader::detail::object::object(uint32_t id) : id(id) {
	gl::detail::state::ensure_loaded();

	if (id == 0 || !glIsShader(id))
		log::warning::critical << "Unable to perform a shader object move operation. "
		"Passed shader object seems to be corrupted.";
	else
		log::message::negligible << "A shader object was moved.";
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
	if (!filestream) {
		log::warning::critical << "File " << filename << " cannot be oppened. Make sure it exists.";
		return detail::object(type);
	}

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

size_t engine::gl::shader::detail::variable::size() const {
	return dimentions.x * dimentions.y * gl::detail::convert::to_size(datatype);
}

engine::gl::shader::detail::variable::variable(std::string const &name, storage_type const &storage,
											   uint32_t location, datatype_t const &datatype_name,
											   dimentions_t const &dimentions)
	: name(name), type(storage), location(location), datatype(datatype_name), dimentions(dimentions) {
	if (dimentions.x < 1 || dimentions.x > 4 || dimentions.y < 1 || dimentions.y > 4)
		log::warning::critical << "Variable dimentions must be in [1; 4] range.";
	if (dimentions.x > 1 && !(datatype_name == datatype_t::_float || datatype_name == datatype_t::_double))
		log::warning::critical << "Matrix variables can only be 'float's or 'double's.";
}

engine::gl::shader::program::program() : id(uint32_t(-1)), needs_linking(true) {
	gl::detail::state::ensure_loaded();

	id = glCreateProgram();
	if (id == 0 || id == -1)
		log::warning::critical << "Shader program creation failed.";
	else
		log::message::minor << "A shader program was created.";
}
engine::gl::shader::program::~program() {
	glDeleteProgram(id);
	log::message::minor << "A shader program was destroyed.";
}

void engine::gl::shader::program::add(detail::object &&object) {
	glAttachShader(id, object.id);
	log::message::negligible << "A shader object was attached to a program.";	
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

			GLchar *log = new GLchar[len + GLsizei(1)];
			glGetProgramInfoLog(id, len, &len, log);
			log::warning::critical << "During program linking: " << std::string(log);
			delete[] log;
		} else
			log::message::minor << "A shader program was linked successfully.";
		needs_linking = false;
	} else
		log::warning::negligible << "Program is already linked. Repeated linkage is not needed.";
}

void engine::gl::shader::program::use() {
	if (!needs_linking)
		gl::detail::state::use(this);
	else
		log::warning::major << "Attempting to use an unlinked program.";
}

template<typename gl_get_lambda_t, typename create_variable_lambda_t>
void local_get(uint32_t program_id, GLenum active_type, GLenum active_type_length,
			   gl_get_lambda_t gl_get_lambda, create_variable_lambda_t create_variable_lambda) {
	GLint number, max_length, name_length, size;
	GLenum type;
	glGetProgramiv(program_id, active_type, &number);
	glGetProgramiv(program_id, active_type_length, &max_length);
	for (int i = 0; i < number; i++) {
		std::vector<GLchar> name(max_length);
		gl_get_lambda(program_id, i, max_length, &name_length, &size, &type, name.data());
		create_variable_lambda(name.data(), glGetAttribLocation(program_id, name.data()), type);
	}
}
engine::gl::shader::variables engine::gl::shader::program::getUniforms() {
	engine::gl::shader::variables out;
	local_get(id, GL_ACTIVE_UNIFORMS, GL_ACTIVE_UNIFORM_MAX_LENGTH,
			  glGetActiveUniform, [&out](auto name, auto location, auto datatype) {
				  auto datatype_pair = gl::detail::convert::to_variable_datatype_pair(datatype);
				  out.insert(std::make_pair(name, detail::variable(name,
																   detail::variable::storage_type::uniform,
																   location,
																   datatype_pair.first,
																   datatype_pair.second)));
			  });
	log::message::minor << "Uniforms (" << out.size() << ") were requested from the shader program.";
	return out;
}
engine::gl::shader::variables engine::gl::shader::program::getAttributes() {
	engine::gl::shader::variables out;
	local_get(id, GL_ACTIVE_ATTRIBUTES, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH,
			  glGetActiveAttrib, [&out](auto name, auto location, auto datatype) {
				  auto datatype_pair = gl::detail::convert::to_variable_datatype_pair(datatype);
				  out.insert(std::make_pair(name, detail::variable(name,
																   detail::variable::storage_type::attribute,
																   location,
																   datatype_pair.first,
																   datatype_pair.second)));
			  });
	log::message::minor << "Attributes (" << out.size() << ") were requested from the shader program.";
	return out;
}
engine::gl::shader::variables engine::gl::shader::program::getVariables() {
	auto out = getUniforms();
	out.merge(getAttributes());
	return out;
}

engine::gl::shader::program::program(uint32_t id) : id(id), needs_linking(true) {
	gl::detail::state::ensure_loaded();

	if (id == 0 || !glIsProgram(id))
		log::warning::critical << "Unable to perform a shader program move operation. "
			"Passed shader program seems to be corrupted.";
	else
		log::message::negligible << "A shader program was moved.";
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
	log::error::critical << "Unsupported enum value.";
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
	log::error::critical << "Unsupported enum value.";
}

GLenum engine::gl::detail::convert::to_gl(shader::detail::variable::datatype_t datatype,
										  shader::detail::variable::dimentions_t dimentions) {
	switch (datatype) {
		case engine::gl::shader::detail::variable::datatype_t::_float:
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
		case engine::gl::shader::detail::variable::datatype_t::_double:
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
		case engine::gl::shader::detail::variable::datatype_t::_int:
			switch (dimentions.x) {
				case 1:
					switch (dimentions.y) {
						case 1: return GL_INT;
						case 2: return GL_INT_VEC2;
						case 3: return GL_INT_VEC3;
						case 4: return GL_INT_VEC4;
					}
			}
		case engine::gl::shader::detail::variable::datatype_t::_unsigned:
			switch (dimentions.x) {
				case 1:
					switch (dimentions.y) {
						case 1: return GL_UNSIGNED_INT;
						case 2: return GL_UNSIGNED_INT_VEC2;
						case 3: return GL_UNSIGNED_INT_VEC3;
						case 4: return GL_UNSIGNED_INT_VEC4;
					}
			}
		case engine::gl::shader::detail::variable::datatype_t::_bool:
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
	log::error::critical << "Unsupported enum value.";
}

std::pair<engine::gl::shader::detail::variable::datatype_t,
	engine::gl::shader::detail::variable::dimentions_t>
	engine::gl::detail::convert::to_variable_datatype_pair(GLenum v) {
	auto make_pair = [](shader::detail::variable::datatype_t datatype, size_t x, size_t y) {
		return std::make_pair(datatype, shader::detail::variable::dimentions_t{ x, y });
	};
	switch (v) {
		case GL_FLOAT: return make_pair(shader::detail::variable::datatype_t::_float, 1, 1);
		case GL_FLOAT_VEC2: return make_pair(shader::detail::variable::datatype_t::_float, 1, 2);
		case GL_FLOAT_VEC3: return make_pair(shader::detail::variable::datatype_t::_float, 1, 3);
		case GL_FLOAT_VEC4: return make_pair(shader::detail::variable::datatype_t::_float, 1, 4);

		case GL_DOUBLE: return make_pair(shader::detail::variable::datatype_t::_double, 1, 1);
		case GL_DOUBLE_VEC2: return make_pair(shader::detail::variable::datatype_t::_double, 1, 2);
		case GL_DOUBLE_VEC3: return make_pair(shader::detail::variable::datatype_t::_double, 1, 3);
		case GL_DOUBLE_VEC4: return make_pair(shader::detail::variable::datatype_t::_double, 1, 4);

		case GL_INT: return make_pair(shader::detail::variable::datatype_t::_int, 1, 1);
		case GL_INT_VEC2: return make_pair(shader::detail::variable::datatype_t::_int, 1, 2);
		case GL_INT_VEC3: return make_pair(shader::detail::variable::datatype_t::_int, 1, 3);
		case GL_INT_VEC4: return make_pair(shader::detail::variable::datatype_t::_int, 1, 4);

		case GL_UNSIGNED_INT: return make_pair(shader::detail::variable::datatype_t::_unsigned, 1, 1);
		case GL_UNSIGNED_INT_VEC2: return make_pair(shader::detail::variable::datatype_t::_unsigned, 1, 2);
		case GL_UNSIGNED_INT_VEC3: return make_pair(shader::detail::variable::datatype_t::_unsigned, 1, 3);
		case GL_UNSIGNED_INT_VEC4: return make_pair(shader::detail::variable::datatype_t::_unsigned, 1, 4);

		case GL_BOOL: return make_pair(shader::detail::variable::datatype_t::_bool, 1, 1);
		case GL_BOOL_VEC2: return make_pair(shader::detail::variable::datatype_t::_bool, 1, 2);
		case GL_BOOL_VEC3: return make_pair(shader::detail::variable::datatype_t::_bool, 1, 3);
		case GL_BOOL_VEC4: return make_pair(shader::detail::variable::datatype_t::_bool, 1, 4);

		case GL_FLOAT_MAT2: return make_pair(shader::detail::variable::datatype_t::_float, 2, 2);
		case GL_FLOAT_MAT3: return make_pair(shader::detail::variable::datatype_t::_float, 3, 3);
		case GL_FLOAT_MAT4: return make_pair(shader::detail::variable::datatype_t::_float, 4, 4);

		case GL_FLOAT_MAT2x3: return make_pair(shader::detail::variable::datatype_t::_float, 2, 3);
		case GL_FLOAT_MAT3x2: return make_pair(shader::detail::variable::datatype_t::_float, 3, 2);
		case GL_FLOAT_MAT4x2: return make_pair(shader::detail::variable::datatype_t::_float, 4, 2);

		case GL_FLOAT_MAT2x4: return make_pair(shader::detail::variable::datatype_t::_float, 2, 4);
		case GL_FLOAT_MAT3x4: return make_pair(shader::detail::variable::datatype_t::_float, 3, 4);
		case GL_FLOAT_MAT4x3: return make_pair(shader::detail::variable::datatype_t::_float, 4, 3);

		case GL_DOUBLE_MAT2: return make_pair(shader::detail::variable::datatype_t::_double, 2, 2);
		case GL_DOUBLE_MAT3: return make_pair(shader::detail::variable::datatype_t::_double, 3, 3);
		case GL_DOUBLE_MAT4: return make_pair(shader::detail::variable::datatype_t::_double, 4, 4);

		case GL_DOUBLE_MAT2x3: return make_pair(shader::detail::variable::datatype_t::_double, 2, 3);
		case GL_DOUBLE_MAT3x2: return make_pair(shader::detail::variable::datatype_t::_double, 3, 2);
		case GL_DOUBLE_MAT4x2: return make_pair(shader::detail::variable::datatype_t::_double, 4, 2);

		case GL_DOUBLE_MAT2x4: return make_pair(shader::detail::variable::datatype_t::_double, 2, 4);
		case GL_DOUBLE_MAT3x4: return make_pair(shader::detail::variable::datatype_t::_double, 3, 4);
		case GL_DOUBLE_MAT4x3: return make_pair(shader::detail::variable::datatype_t::_double, 4, 3);
	}
	log::error::critical << "Unsupported enum value.";
}

GLenum engine::gl::detail::convert::to_gl(shader::detail::variable::datatype_t datatype) {
	switch (datatype) {
		case engine::gl::shader::detail::variable::datatype_t::_float: return GL_FLOAT;
		case engine::gl::shader::detail::variable::datatype_t::_double: return GL_DOUBLE;
		case engine::gl::shader::detail::variable::datatype_t::_int: return GL_INT;
		case engine::gl::shader::detail::variable::datatype_t::_unsigned: return GL_UNSIGNED_INT;
		case engine::gl::shader::detail::variable::datatype_t::_bool: return GL_BOOL;
	}
	log::error::critical << "Unsupported enum value.";
}

size_t engine::gl::detail::convert::to_size(shader::detail::variable::datatype_t datatype) {
	switch (datatype) {
		case engine::gl::shader::detail::variable::datatype_t::_float: return sizeof(float);
		case engine::gl::shader::detail::variable::datatype_t::_double: return sizeof(double);
		case engine::gl::shader::detail::variable::datatype_t::_int: return sizeof(int);
		case engine::gl::shader::detail::variable::datatype_t::_unsigned: return sizeof(unsigned);
		case engine::gl::shader::detail::variable::datatype_t::_bool: return sizeof(bool);
	}
	log::error::critical << "Unsupported enum value.";
}

std::ostream &operator<<(std::ostream &stream, engine::gl::shader::type type) {
	switch (type) {
		case engine::gl::shader::type::fragment:				stream << "fragment"; break;
		case engine::gl::shader::type::vertex:					stream << "vertex"; break;
		case engine::gl::shader::type::geometry:				stream << "geometry"; break;
		case engine::gl::shader::type::compute:					stream << "compute"; break;
		case engine::gl::shader::type::tesselation_control:		stream << "tesselation control"; break;
		case engine::gl::shader::type::tesselation_evaluation:	stream << "tesselation evaluation"; break;
		default: 
			engine::log::warning::major << "Unsupported enum value.";
	}
	return stream;
}


engine::gl::shader::detail::variable const &engine::gl::shader::variables::operator[](std::string name) {
	auto found = find(name);
	if (found != end())
		return found->second;
	else {
		log::error::major << "Attempting to access an non-existent variable.";
		log::info::major << "Requested name is '" << name << "'.";
	}
}
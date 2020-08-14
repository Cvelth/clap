#include "gl/shader.hpp"

#include <fstream>
#include <string>
#include <vector>

#include "glad/gl.h"

#include "gl/detail/state.hpp"
#include "essential/log.hpp"

clap::gl::shader::detail::object::object(type type) : id(uint32_t(-1)) {
	gl::detail::state::ensure_loaded();

	id = glCreateShader(gl::detail::convert::to_gl(type));
	if (id == 0 || id == -1)
		log::warning::critical << "Shader object creation failed.";
	else
		log::message::minor << "A " << type << " shader object was created.";
}

clap::gl::shader::detail::object::~object() {
	glDeleteShader(id);
	log::message::minor << "A shader object was destroyed.";
}

clap::gl::shader::detail::object::object(type type, std::string source) : object(type) {
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

clap::gl::shader::detail::object::object(uint32_t id) : id(id) {
	gl::detail::state::ensure_loaded();

	if (id == 0 || !glIsShader(id))
		log::warning::critical << "Unable to perform a shader object move operation. "
		"Passed shader object seems to be corrupted.";
	else
		log::message::negligible << "A shader object was moved.";
}

clap::gl::shader::detail::object clap::gl::shader::from_source(type type, std::string source) {
	return detail::object(type, source);
}
clap::gl::shader::detail::object clap::gl::shader::from_source(type type, std::string_view source) {
	return from_source(type, std::string(source));
}

clap::gl::shader::detail::object clap::gl::shader::from_source(type type, char const *source) {
	return from_source(type, std::string(source));
}

clap::gl::shader::detail::object clap::gl::shader::from_file(type type, std::string filename) {
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
clap::gl::shader::detail::object clap::gl::shader::from_file(type type, std::string_view filename) {
	return from_file(type, std::string(filename));
}

clap::gl::shader::detail::object clap::gl::shader::from_file(type type, char const *filename) {
	return from_file(type, std::string(filename));
}

size_t clap::gl::shader::detail::variable::count() const {
	return type.dimentions.x * type.dimentions.y;
}

size_t clap::gl::shader::detail::variable::size() const {
	if (type.structure == variable_type_t::structure::data)
		return count() * gl::detail::convert::to_size(type.datatype);
	else
		log::warning::critical << "Size can only be obtained for data variables.";
}

clap::gl::shader::detail::variable::variable(std::string const &name, uint32_t const &location,
											 variable_type_t::storage const storage,
											 variable_type const &type)
	: name(name), location(location), storage(storage), type(type) {

	if (type.structure == variable_type_t::structure::data) {
		if (type.dimentions.x < 1 || type.dimentions.x > 4 || type.dimentions.y < 1 || type.dimentions.y > 4)
			log::warning::critical << "Variable dimentions must be in [1; 4] range.";
		if (type.dimentions.x > 1 && !(type.datatype == variable_type_t::datatype::_float || type.datatype == variable_type_t::datatype::_double))
			log::warning::critical << "Matrix variables can only be 'float's or 'double's.";
		if (type.dimentions.y == 1 && type.dimentions.x != 1)
			log::warning::critical << "Variable dimentions are impossible. It's possible it has been corrupted.";
		if (type.specific != variable_type_t::specific::none)
			log::warning::critical << "Data variables must not have any specifics applied.";
	} else {
		if (type.dimentions.y != 0)
			log::warning::critical << "'y' dimention must always be '0' for non-data variables.";
		if (type.dimentions.x == 0 && !(type.specific == variable_type_t::specific::cube || type.specific == variable_type_t::specific::buffer))
			log::warning::critical << "'x' dimention must not be '0' unless 'cube' or 'buffer' specific is applied.";
		if ((type.specific == variable_type_t::specific::multisample ||
			 type.specific == variable_type_t::specific::multisample_array ||
			 type.specific == variable_type_t::specific::rect) && type.dimentions.x != 2)
			log::warning::critical << "'x' dimention must always be '2' if 'multisample', 'multisample_array' or 'rect' specific is applied.";
		if (type.specific == variable_type_t::specific::array && type.dimentions.x > 2)
			log::warning::critical << "'x' dimention must not be bigger than '2' if 'array' specific is applied.";
		if (type.dimentions.x > 3)
			log::warning::critical << "'x' dimention must not be bigger than '3' for non-data variables.";

		if (type.structure == variable_type_t::structure::shadow_sampler) {
			if (type.dimentions.x > 2)
				log::warning::critical << "'x' dimention must always be '2' or less for 'shadow_sampler' variables.";
			if (type.datatype != variable_type_t::datatype::_float)
				log::warning::critical << "'shadow_sampler' variables must always have 'float' type.";
			if (type.specific == variable_type_t::specific::buffer
				|| type.specific == variable_type_t::specific::multisample
				|| type.specific == variable_type_t::specific::multisample_array) {

				log::warning::critical << "'shadow_sampler' variables must not have 'buffer', 'multisample' or 'multisample_array' specifics applied.";
			}
		}
		if (type.datatype != variable_type_t::datatype::_float &&
			type.datatype != variable_type_t::datatype::_int &&
			type.datatype != variable_type_t::datatype::_unsigned) {

			if (type.structure == variable_type_t::structure::sampler)
				log::warning::critical << "'sampler' variables can only have 'float', 'int' or 'unsigned' type.";
			if (type.structure == variable_type_t::structure::image)
				log::warning::critical << "'image' variables can only have 'float', 'int' or 'unsigned' type.";
		}
	}
}

clap::gl::shader::program::program() : id(uint32_t(-1)), needs_linking(true) {
	gl::detail::state::ensure_loaded();

	id = glCreateProgram();
	if (id == 0 || id == -1)
		log::warning::critical << "Shader program creation failed.";
	else
		log::message::minor << "A shader program was created.";
}
clap::gl::shader::program::~program() {
	glDeleteProgram(id);
	log::message::minor << "A shader program was destroyed.";
}

void clap::gl::shader::program::add(detail::object const &object) {
	glAttachShader(id, object.id);
	log::message::negligible << "A shader object was attached to a program.";
	needs_linking = true;
}

void clap::gl::shader::program::add(detail::object &&object) {
	glAttachShader(id, object.id);
	log::message::negligible << "A shader object was attached to a program.";
	needs_linking = true;
}

void clap::gl::shader::program::link() {
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

void clap::gl::shader::program::use() {
	if (!needs_linking)
		gl::detail::state::use(this);
	else
		log::warning::major << "Attempting to use an unlinked program.";
}

template<typename gl_get_lambda_t, typename gl_get_location_lambda_t, typename create_variable_lambda_t>
void local_get(uint32_t program_id, GLenum active_type, GLenum active_type_length,
			   gl_get_lambda_t gl_get_lambda, gl_get_location_lambda_t gl_get_location_lambda,
			   create_variable_lambda_t create_variable_lambda) {
	GLint number, max_length, name_length, size;
	GLenum type;
	glGetProgramiv(program_id, active_type, &number);
	glGetProgramiv(program_id, active_type_length, &max_length);
	for (int i = 0; i < number; i++) {
		std::vector<GLchar> name(max_length);
		gl_get_lambda(program_id, i, max_length, &name_length, &size, &type, name.data());
		create_variable_lambda(name.data(), gl_get_location_lambda(program_id, name.data()), type);
	}
}
clap::gl::shader::variables clap::gl::shader::program::getUniforms() {
	clap::gl::shader::variables out;
	local_get(id, GL_ACTIVE_UNIFORMS, GL_ACTIVE_UNIFORM_MAX_LENGTH, glGetActiveUniform, 
			  glad_glGetUniformLocation, [&out](auto name, auto location, auto datatype) {
				  out.insert(std::make_pair(
					  name,
					  detail::variable(name,
									   location,
									   detail::variable_type_t::storage::uniform,
									   gl::detail::convert::to_variable_type(datatype))
				  ));
			  });
	log::message::minor << "Uniforms (" << out.size() << ") were requested from the shader program.";
	return out;
}
clap::gl::shader::variables clap::gl::shader::program::getAttributes() {
	clap::gl::shader::variables out;
	local_get(id, GL_ACTIVE_ATTRIBUTES, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, glGetActiveAttrib, 
			  glGetAttribLocation, [&out](auto name, auto location, auto datatype) {
				  out.insert(std::make_pair(
					  name,
					  detail::variable(name,
									   location,
									   detail::variable_type_t::storage::attribute,
									   gl::detail::convert::to_variable_type(datatype))
				  ));
			  });
	log::message::minor << "Attributes (" << out.size() << ") were requested from the shader program.";
	return out;
}
clap::gl::shader::variables clap::gl::shader::program::getVariables() {
	auto out = getUniforms();
	out.merge(getAttributes());
	return out;
}

void clap::gl::shader::program::set(detail::variable const &variable, std::vector<float> const &values) {
	set(variable, values.size(), values.data());
}
void clap::gl::shader::program::set(detail::variable const &variable, std::initializer_list<float> const &values) {
	set(variable, std::vector<float>(values));
}

void clap::gl::shader::program::set(detail::variable const &variable, std::vector<double> const &values) {
	set(variable, values.size(), values.data());
}
void clap::gl::shader::program::set(detail::variable const &variable, std::initializer_list<double> const &values) {
	set(variable, std::vector<double>(values));
}

void clap::gl::shader::program::set(detail::variable const &variable, std::vector<int> const &values) {
	set(variable, values.size(), values.data());
}
void clap::gl::shader::program::set(detail::variable const &variable, std::initializer_list<int> const &values) {
	set(variable, std::vector<int>(values));
}

void clap::gl::shader::program::set(detail::variable const &variable, std::vector<unsigned> const &values) {
	set(variable, values.size(), values.data());
}
void clap::gl::shader::program::set(detail::variable const &variable, std::initializer_list<unsigned> const &values) {
	set(variable, std::vector<unsigned>(values));
}

bool check_uniform_variable(uint32_t const &location,
							clap::gl::shader::detail::variable_type_t::storage const &storage,
							clap::gl::shader::detail::variable_type const &type,
							clap::gl::shader::detail::variable_type_t::datatype expected_datatype) {
	if (location == -1) {
		clap::log::warning::major << "Uniform variable is silently ignored by OpenGL. It's location value is '-1'.";
		return false;
	}

	if (storage != clap::gl::shader::detail::variable_type_t::storage::uniform
		|| type.structure != clap::gl::shader::detail::variable_type_t::structure::data) {

		clap::log::warning::major << "Only data uniform variables can be directly set.";
		return false;
	}

	if (type.datatype != expected_datatype && type.datatype != clap::gl::shader::detail::variable_type_t::datatype::_bool) {
		clap::log::warning::major << "Cannot set value to a variable of a different type.";
		return false;
	}

	return true;
}

void clap::gl::shader::program::set(detail::variable const &variable, size_t n, const float *values) {
	if (!check_uniform_variable(variable.location, variable.storage, variable.type,
								clap::gl::shader::detail::variable_type_t::datatype::_float))
		return;
	if (variable.count() != n) {
		clap::log::warning::major << "The number of values passed isn't suitable for the variable.";
		return;
	}

	use();
	switch (variable.type.dimentions.x) {
		case 1:
			switch (variable.type.dimentions.y) {
				case 1: glUniform1fv(variable.location, 1, values); return;
				case 2: glUniform2fv(variable.location, 1, values); return;
				case 3: glUniform3fv(variable.location, 1, values); return;
				case 4: glUniform4fv(variable.location, 1, values); return;
			} break;
		case 2:
			switch (variable.type.dimentions.y) {
				case 2: glUniformMatrix2fv(variable.location, 1, false, values); return;
				case 3: glUniformMatrix2x3fv(variable.location, 1, false, values); return;
				case 4: glUniformMatrix2x4fv(variable.location, 1, false, values); return;
			} break;
		case 3:
			switch (variable.type.dimentions.y) {
				case 2: glUniformMatrix3x2fv(variable.location, 1, false, values); return;
				case 3: glUniformMatrix3fv(variable.location, 1, false, values); return;
				case 4: glUniformMatrix3x4fv(variable.location, 1, false, values); return;
			} break;
		case 4:
			switch (variable.type.dimentions.y) {
				case 2: glUniformMatrix4x2fv(variable.location, 1, false, values); return;
				case 3: glUniformMatrix4x3fv(variable.location, 1, false, values); return;
				case 4: glUniformMatrix4fv(variable.location, 1, false, values); return;
			}
			break;
	}
	clap::log::error::critical << "Cannot set the variable. It's either unsupported or corrupted.";
}

void clap::gl::shader::program::set(detail::variable const &variable, size_t n, const double *values) {
	if (!check_uniform_variable(variable.location, variable.storage, variable.type,
								clap::gl::shader::detail::variable_type_t::datatype::_double))
		return;
	if (variable.count() != n) {
		clap::log::warning::major << "The number of values passed isn't suitable for the variable.";
		return;
	}

	use();
	switch (variable.type.dimentions.x) {
		case 1:
			switch (variable.type.dimentions.y) {
				case 1: glUniform1dv(variable.location, 1, values); return;
				case 2: glUniform2dv(variable.location, 1, values); return;
				case 3: glUniform3dv(variable.location, 1, values); return;
				case 4: glUniform4dv(variable.location, 1, values); return;
			} break;
		case 2:
			switch (variable.type.dimentions.y) {
				case 2: glUniformMatrix2dv(variable.location, 1, false, values); return;
				case 3: glUniformMatrix2x3dv(variable.location, 1, false, values); return;
				case 4: glUniformMatrix2x4dv(variable.location, 1, false, values); return;
			} break;
		case 3:
			switch (variable.type.dimentions.y) {
				case 2: glUniformMatrix3x2dv(variable.location, 1, false, values); return;
				case 3: glUniformMatrix3dv(variable.location, 1, false, values); return;
				case 4: glUniformMatrix3x4dv(variable.location, 1, false, values); return;
			} break;
		case 4:
			switch (variable.type.dimentions.y) {
				case 2: glUniformMatrix4x2dv(variable.location, 1, false, values); return;
				case 3: glUniformMatrix4x3dv(variable.location, 1, false, values); return;
				case 4: glUniformMatrix4dv(variable.location, 1, false, values); return;
			}
			break;
	}
	clap::log::error::critical << "Cannot set the variable. It's either unsupported or corrupted.";
}

void clap::gl::shader::program::set(detail::variable const &variable, size_t n, const int *values) {
	if (!check_uniform_variable(variable.location, variable.storage, variable.type,
								clap::gl::shader::detail::variable_type_t::datatype::_int))
		return;
	if (variable.count() != n) {
		clap::log::warning::major << "The number of values passed isn't suitable for the variable.";
		return;
	}

	use();
	switch (variable.type.dimentions.x) {
		case 1:
			switch (variable.type.dimentions.y) {
				case 1: glUniform1iv(variable.location, 1, values); return;
				case 2: glUniform2iv(variable.location, 1, values); return;
				case 3: glUniform3iv(variable.location, 1, values); return;
				case 4: glUniform4iv(variable.location, 1, values); return;
			} break;
	}
	clap::log::error::critical << "Cannot set the variable. It's either unsupported or corrupted.";
}

void clap::gl::shader::program::set(detail::variable const &variable, size_t n, const unsigned *values) {
	if (!check_uniform_variable(variable.location, variable.storage, variable.type,
								clap::gl::shader::detail::variable_type_t::datatype::_unsigned))
		return;
	if (variable.count() != n) {
		clap::log::warning::major << "The number of values passed isn't suitable for the variable.";
		return;
	}

	use();
	switch (variable.type.dimentions.x) {
		case 1:
			switch (variable.type.dimentions.y) {
				case 1: glUniform1uiv(variable.location, 1, values); return;
				case 2: glUniform2uiv(variable.location, 1, values); return;
				case 3: glUniform3uiv(variable.location, 1, values); return;
				case 4: glUniform4uiv(variable.location, 1, values); return;
			} break;
	}
	clap::log::error::critical << "Cannot set the variable. It's either unsupported or corrupted.";
}

clap::gl::shader::program::program(uint32_t id) : id(id), needs_linking(true) {
	gl::detail::state::ensure_loaded();

	if (id == 0 || !glIsProgram(id))
		log::warning::critical << "Unable to perform a shader program move operation. "
		"Passed shader program seems to be corrupted.";
	else
		log::message::negligible << "A shader program was moved.";
}

GLenum clap::gl::detail::convert::to_gl(clap::gl::shader::type v) {
	switch (v) {
		case clap::gl::shader::type::fragment:					return GL_FRAGMENT_SHADER;
		case clap::gl::shader::type::vertex:					return GL_VERTEX_SHADER;
		case clap::gl::shader::type::geometry:					return GL_GEOMETRY_SHADER;
		case clap::gl::shader::type::compute:					return GL_COMPUTE_SHADER;
		case clap::gl::shader::type::tesselation_control:		return GL_TESS_CONTROL_SHADER;
		case clap::gl::shader::type::tesselation_evaluation:	return GL_TESS_EVALUATION_SHADER;
	}
	log::error::critical << "Unsupported enum value.";
}
clap::gl::shader::type clap::gl::detail::convert::to_shader_type(GLenum v) {
	switch (v) {
		case GL_FRAGMENT_SHADER:		return clap::gl::shader::type::fragment;
		case GL_VERTEX_SHADER:			return clap::gl::shader::type::vertex;
		case GL_GEOMETRY_SHADER:		return clap::gl::shader::type::geometry;
		case GL_COMPUTE_SHADER:			return clap::gl::shader::type::compute;
		case GL_TESS_CONTROL_SHADER:	return clap::gl::shader::type::tesselation_control;
		case GL_TESS_EVALUATION_SHADER: return clap::gl::shader::type::tesselation_evaluation;
	}
	log::error::critical << "Unsupported enum value.";
}

clap::gl::shader::type clap::gl::detail::convert::to_shader_type_from_string(std::string const &v) {
	if (v == "fragment")
		return clap::gl::shader::type::fragment;
	else if (v == "vertex")
		return clap::gl::shader::type::vertex;
	else if (v == "geometry")
		return clap::gl::shader::type::geometry;
	else if (v == "compute")
		return clap::gl::shader::type::compute;
	else if (v == "tesselation_control")
		return clap::gl::shader::type::tesselation_control;
	else if (v == "tesselation_evaluation")
		return clap::gl::shader::type::tesselation_evaluation;

	log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(shader::detail::variable_type const &type) {
	switch (type.structure) {
		case shader::detail::variable_type_t::structure::data:
			switch (type.datatype) {
				case shader::detail::variable_type_t::datatype::_float:
					switch (type.dimentions.x) {
						case 1:
							switch (type.dimentions.y) {
								case 1: return GL_FLOAT;
								case 2: return GL_FLOAT_VEC2;
								case 3: return GL_FLOAT_VEC3;
								case 4: return GL_FLOAT_VEC4;
							}
							break;
						case 2:
							switch (type.dimentions.y) {
								case 2: return GL_FLOAT_MAT2;
								case 3: return GL_FLOAT_MAT2x3;
								case 4: return GL_FLOAT_MAT2x4;
							}
							break;
						case 3:
							switch (type.dimentions.y) {
								case 2: return GL_FLOAT_MAT3x2;
								case 3: return GL_FLOAT_MAT3;
								case 4: return GL_FLOAT_MAT3x4;
							}
							break;
						case 4:
							switch (type.dimentions.y) {
								case 2: return GL_FLOAT_MAT4x2;
								case 3: return GL_FLOAT_MAT4x3;
								case 4: return GL_FLOAT_MAT4;
							}
							break;
					}
					break;
				case shader::detail::variable_type_t::datatype::_double:
					switch (type.dimentions.x) {
						case 1:
							switch (type.dimentions.y) {
								case 1: return GL_DOUBLE;
								case 2: return GL_DOUBLE_VEC2;
								case 3: return GL_DOUBLE_VEC3;
								case 4: return GL_DOUBLE_VEC4;
							}
							break;
						case 2:
							switch (type.dimentions.y) {
								case 2: return GL_DOUBLE_MAT2;
								case 3: return GL_DOUBLE_MAT2x3;
								case 4: return GL_DOUBLE_MAT2x4;
							}
							break;
						case 3:
							switch (type.dimentions.y) {
								case 2: return GL_DOUBLE_MAT3x2;
								case 3: return GL_DOUBLE_MAT3;
								case 4: return GL_DOUBLE_MAT3x4;
							}
							break;
						case 4:
							switch (type.dimentions.y) {
								case 2: return GL_DOUBLE_MAT4x2;
								case 3: return GL_DOUBLE_MAT4x3;
								case 4: return GL_DOUBLE_MAT4;
							}
							break;
					}
					break;
				case shader::detail::variable_type_t::datatype::_int:
					switch (type.dimentions.x) {
						case 1:
							switch (type.dimentions.y) {
								case 1: return GL_INT;
								case 2: return GL_INT_VEC2;
								case 3: return GL_INT_VEC3;
								case 4: return GL_INT_VEC4;
							}
							break;
					}
					break;
				case shader::detail::variable_type_t::datatype::_unsigned:
					switch (type.dimentions.x) {
						case 1:
							switch (type.dimentions.y) {
								case 1: return GL_UNSIGNED_INT;
								case 2: return GL_UNSIGNED_INT_VEC2;
								case 3: return GL_UNSIGNED_INT_VEC3;
								case 4: return GL_UNSIGNED_INT_VEC4;
							}
							break;
					}
					break;
				case shader::detail::variable_type_t::datatype::_bool:
					switch (type.dimentions.x) {
						case 1:
							switch (type.dimentions.y) {
								case 1: return GL_BOOL;
								case 2: return GL_BOOL_VEC2;
								case 3: return GL_BOOL_VEC3;
								case 4: return GL_BOOL_VEC4;
							}
							break;
					}
					break;
			}
			break;

		case shader::detail::variable_type_t::structure::sampler:
			switch (type.datatype) {
				case shader::detail::variable_type_t::datatype::_float:
					switch (type.specific) {
						case shader::detail::variable_type_t::specific::none:
							switch (type.dimentions.x) {
								case 1: return GL_SAMPLER_1D;
								case 2: return GL_SAMPLER_2D;
								case 3: return GL_SAMPLER_3D;
							}
							break;
						case shader::detail::variable_type_t::specific::cube:
							switch (type.dimentions.x) {
								case 0: return GL_SAMPLER_CUBE;
							}
							break;
						case shader::detail::variable_type_t::specific::rect:
							switch (type.dimentions.x) {
								case 2: return GL_SAMPLER_2D_RECT;
							}
							break;
						case shader::detail::variable_type_t::specific::array:
							switch (type.dimentions.x) {
								case 1: return GL_SAMPLER_1D_ARRAY;
								case 2: return GL_SAMPLER_2D_ARRAY;
							}
							break;
						case shader::detail::variable_type_t::specific::buffer:
							switch (type.dimentions.x) {
								case 0: return GL_SAMPLER_BUFFER;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample:
							switch (type.dimentions.x) {
								case 2: return GL_SAMPLER_2D_MULTISAMPLE;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample_array:
							switch (type.dimentions.x) {
								case 2: return GL_SAMPLER_2D_MULTISAMPLE_ARRAY;
							}
							break;
					}
					break;
				case shader::detail::variable_type_t::datatype::_int:
					switch (type.specific) {
						case shader::detail::variable_type_t::specific::none:
							switch (type.dimentions.x) {
								case 1: return GL_INT_SAMPLER_1D;
								case 2: return GL_INT_SAMPLER_2D;
								case 3: return GL_INT_SAMPLER_3D;
							}
							break;
						case shader::detail::variable_type_t::specific::cube:
							switch (type.dimentions.x) {
								case 0: return GL_INT_SAMPLER_CUBE;
							}
							break;
						case shader::detail::variable_type_t::specific::rect:
							switch (type.dimentions.x) {
								case 2: return GL_INT_SAMPLER_2D_RECT;
							}
							break;
						case shader::detail::variable_type_t::specific::array:
							switch (type.dimentions.x) {
								case 1: return GL_INT_SAMPLER_1D_ARRAY;
								case 2: return GL_INT_SAMPLER_2D_ARRAY;
							}
							break;
						case shader::detail::variable_type_t::specific::buffer:
							switch (type.dimentions.x) {
								case 0: return GL_INT_SAMPLER_BUFFER;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample:
							switch (type.dimentions.x) {
								case 2: return GL_INT_SAMPLER_2D_MULTISAMPLE;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample_array:
							switch (type.dimentions.x) {
								case 2: return GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
							}
							break;
					}
					break;
				case shader::detail::variable_type_t::datatype::_unsigned:
					switch (type.specific) {
						case shader::detail::variable_type_t::specific::none:
							switch (type.dimentions.x) {
								case 1: return GL_UNSIGNED_INT_SAMPLER_1D;
								case 2: return GL_UNSIGNED_INT_SAMPLER_2D;
								case 3: return GL_UNSIGNED_INT_SAMPLER_3D;
							}
							break;
						case shader::detail::variable_type_t::specific::cube:
							switch (type.dimentions.x) {
								case 0: return GL_UNSIGNED_INT_SAMPLER_CUBE;
							}
							break;
						case shader::detail::variable_type_t::specific::rect:
							switch (type.dimentions.x) {
								case 2: return GL_UNSIGNED_INT_SAMPLER_2D_RECT;
							}
							break;
						case shader::detail::variable_type_t::specific::array:
							switch (type.dimentions.x) {
								case 1: return GL_UNSIGNED_INT_SAMPLER_1D_ARRAY;
								case 2: return GL_UNSIGNED_INT_SAMPLER_2D_ARRAY;
							}
							break;
						case shader::detail::variable_type_t::specific::buffer:
							switch (type.dimentions.x) {
								case 0: return GL_UNSIGNED_INT_SAMPLER_BUFFER;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample:
							switch (type.dimentions.x) {
								case 2: return GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample_array:
							switch (type.dimentions.x) {
								case 2: return GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY;
							}
							break;
					}
					break;
			}
			break;

		case shader::detail::variable_type_t::structure::shadow_sampler:
			switch (type.datatype) {
				case shader::detail::variable_type_t::datatype::_float:
					switch (type.specific) {
						case shader::detail::variable_type_t::specific::none:
							switch (type.dimentions.x) {
								case 1: return GL_SAMPLER_1D_SHADOW;
								case 2: return GL_SAMPLER_2D_SHADOW;
							}
							break;
						case shader::detail::variable_type_t::specific::cube:
							switch (type.dimentions.x) {
								case 0: return GL_SAMPLER_CUBE_SHADOW;
							}
							break;
						case shader::detail::variable_type_t::specific::rect:
							switch (type.dimentions.x) {
								case 2: return GL_SAMPLER_2D_RECT_SHADOW;
							}
							break;
						case shader::detail::variable_type_t::specific::array:
							switch (type.dimentions.x) {
								case 1: return GL_SAMPLER_1D_ARRAY_SHADOW;
								case 2: return GL_SAMPLER_2D_ARRAY_SHADOW;
							}
							break;
					}
					break;
			}
			break;
		case shader::detail::variable_type_t::structure::image:
			switch (type.datatype) {
				case shader::detail::variable_type_t::datatype::_float:
					switch (type.specific) {
						case shader::detail::variable_type_t::specific::none:
							switch (type.dimentions.x) {
								case 1: return GL_IMAGE_1D;
								case 2: return GL_IMAGE_2D;
								case 3: return GL_IMAGE_3D;
							}
							break;
						case shader::detail::variable_type_t::specific::cube:
							switch (type.dimentions.x) {
								case 0: return GL_IMAGE_CUBE;
							}
							break;
						case shader::detail::variable_type_t::specific::rect:
							switch (type.dimentions.x) {
								case 2: return GL_IMAGE_2D_RECT;
							}
							break;
						case shader::detail::variable_type_t::specific::array:
							switch (type.dimentions.x) {
								case 1: return GL_IMAGE_1D_ARRAY;
								case 2: return GL_IMAGE_2D_ARRAY;
							}
							break;
						case shader::detail::variable_type_t::specific::buffer:
							switch (type.dimentions.x) {
								case 0: return GL_IMAGE_BUFFER;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample:
							switch (type.dimentions.x) {
								case 2: return GL_IMAGE_2D_MULTISAMPLE;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample_array:
							switch (type.dimentions.x) {
								case 2: return GL_IMAGE_2D_MULTISAMPLE_ARRAY;
							}
							break;
					}
					break;
				case shader::detail::variable_type_t::datatype::_int:
					switch (type.specific) {
						case shader::detail::variable_type_t::specific::none:
							switch (type.dimentions.x) {
								case 1: return GL_INT_IMAGE_1D;
								case 2: return GL_INT_IMAGE_2D;
								case 3: return GL_INT_IMAGE_3D;
							}
							break;
						case shader::detail::variable_type_t::specific::cube:
							switch (type.dimentions.x) {
								case 0: return GL_INT_IMAGE_CUBE;
							}
							break;
						case shader::detail::variable_type_t::specific::rect:
							switch (type.dimentions.x) {
								case 2: return GL_INT_IMAGE_2D_RECT;
							}
							break;
						case shader::detail::variable_type_t::specific::array:
							switch (type.dimentions.x) {
								case 1: return GL_INT_IMAGE_1D_ARRAY;
								case 2: return GL_INT_IMAGE_2D_ARRAY;
							}
							break;
						case shader::detail::variable_type_t::specific::buffer:
							switch (type.dimentions.x) {
								case 0: return GL_INT_IMAGE_BUFFER;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample:
							switch (type.dimentions.x) {
								case 2: return GL_INT_IMAGE_2D_MULTISAMPLE;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample_array:
							switch (type.dimentions.x) {
								case 2: return GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
							}
							break;
					}
					break;
				case shader::detail::variable_type_t::datatype::_unsigned:
					switch (type.specific) {
						case shader::detail::variable_type_t::specific::none:
							switch (type.dimentions.x) {
								case 1: return GL_UNSIGNED_INT_IMAGE_1D;
								case 2: return GL_UNSIGNED_INT_IMAGE_2D;
								case 3: return GL_UNSIGNED_INT_IMAGE_3D;
							}
							break;
						case shader::detail::variable_type_t::specific::cube:
							switch (type.dimentions.x) {
								case 0: return GL_UNSIGNED_INT_IMAGE_CUBE;
							}
							break;
						case shader::detail::variable_type_t::specific::rect:
							switch (type.dimentions.x) {
								case 2: return GL_UNSIGNED_INT_IMAGE_2D_RECT;
							}
							break;
						case shader::detail::variable_type_t::specific::array:
							switch (type.dimentions.x) {
								case 1: return GL_UNSIGNED_INT_IMAGE_1D_ARRAY;
								case 2: return GL_UNSIGNED_INT_IMAGE_2D_ARRAY;
							}
							break;
						case shader::detail::variable_type_t::specific::buffer:
							switch (type.dimentions.x) {
								case 0: return GL_UNSIGNED_INT_IMAGE_BUFFER;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample:
							switch (type.dimentions.x) {
								case 2: return GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE;
							}
							break;
						case shader::detail::variable_type_t::specific::multisample_array:
							switch (type.dimentions.x) {
								case 2: return GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY;
							}
							break;
					}
					break;
			}
			break;
	}
	log::error::critical << "Unsupported enum value.";
}

clap::gl::shader::detail::variable_type clap::gl::detail::convert::to_variable_type(GLenum v) {
	using namespace shader::detail;
	using namespace shader::detail::variable_type_t;

	auto make = [](structure _structure, datatype _datatype, specific _specific, unsigned x, unsigned y) -> variable_type {
		return variable_type(_structure, _datatype, _specific, dimentions{ x, y });
	};

	switch (v) {
		case GL_FLOAT: return make(structure::data, datatype::_float, specific::none, 1, 1);
		case GL_FLOAT_VEC2: return make(structure::data, datatype::_float, specific::none, 1, 2);
		case GL_FLOAT_VEC3: return make(structure::data, datatype::_float, specific::none, 1, 3);
		case GL_FLOAT_VEC4: return make(structure::data, datatype::_float, specific::none, 1, 4);

		case GL_DOUBLE: return make(structure::data, datatype::_double, specific::none, 1, 1);
		case GL_DOUBLE_VEC2: return make(structure::data, datatype::_double, specific::none, 1, 2);
		case GL_DOUBLE_VEC3: return make(structure::data, datatype::_double, specific::none, 1, 3);
		case GL_DOUBLE_VEC4: return make(structure::data, datatype::_double, specific::none, 1, 4);

		case GL_INT: return make(structure::data, datatype::_int, specific::none, 1, 1);
		case GL_INT_VEC2: return make(structure::data, datatype::_int, specific::none, 1, 2);
		case GL_INT_VEC3: return make(structure::data, datatype::_int, specific::none, 1, 3);
		case GL_INT_VEC4: return make(structure::data, datatype::_int, specific::none, 1, 4);

		case GL_UNSIGNED_INT: return make(structure::data, datatype::_unsigned, specific::none, 1, 1);
		case GL_UNSIGNED_INT_VEC2: return make(structure::data, datatype::_unsigned, specific::none, 1, 2);
		case GL_UNSIGNED_INT_VEC3: return make(structure::data, datatype::_unsigned, specific::none, 1, 3);
		case GL_UNSIGNED_INT_VEC4: return make(structure::data, datatype::_unsigned, specific::none, 1, 4);

		case GL_BOOL: return make(structure::data, datatype::_bool, specific::none, 1, 1);
		case GL_BOOL_VEC2: return make(structure::data, datatype::_bool, specific::none, 1, 2);
		case GL_BOOL_VEC3: return make(structure::data, datatype::_bool, specific::none, 1, 3);
		case GL_BOOL_VEC4: return make(structure::data, datatype::_bool, specific::none, 1, 4);

		case GL_FLOAT_MAT2: return make(structure::data, datatype::_float, specific::none, 2, 2);
		case GL_FLOAT_MAT3: return make(structure::data, datatype::_float, specific::none, 3, 3);
		case GL_FLOAT_MAT4: return make(structure::data, datatype::_float, specific::none, 4, 4);

		case GL_FLOAT_MAT2x3: return make(structure::data, datatype::_float, specific::none, 2, 3);
		case GL_FLOAT_MAT3x2: return make(structure::data, datatype::_float, specific::none, 3, 2);
		case GL_FLOAT_MAT4x2: return make(structure::data, datatype::_float, specific::none, 4, 2);

		case GL_FLOAT_MAT2x4: return make(structure::data, datatype::_float, specific::none, 2, 4);
		case GL_FLOAT_MAT3x4: return make(structure::data, datatype::_float, specific::none, 3, 4);
		case GL_FLOAT_MAT4x3: return make(structure::data, datatype::_float, specific::none, 4, 3);

		case GL_DOUBLE_MAT2: return make(structure::data, datatype::_double, specific::none, 2, 2);
		case GL_DOUBLE_MAT3: return make(structure::data, datatype::_double, specific::none, 3, 3);
		case GL_DOUBLE_MAT4: return make(structure::data, datatype::_double, specific::none, 4, 4);

		case GL_DOUBLE_MAT2x3: return make(structure::data, datatype::_double, specific::none, 2, 3);
		case GL_DOUBLE_MAT3x2: return make(structure::data, datatype::_double, specific::none, 3, 2);
		case GL_DOUBLE_MAT4x2: return make(structure::data, datatype::_double, specific::none, 4, 2);

		case GL_DOUBLE_MAT2x4: return make(structure::data, datatype::_double, specific::none, 2, 4);
		case GL_DOUBLE_MAT3x4: return make(structure::data, datatype::_double, specific::none, 3, 4);
		case GL_DOUBLE_MAT4x3: return make(structure::data, datatype::_double, specific::none, 4, 3);

		case GL_SAMPLER_1D: return make(structure::sampler, datatype::_float, specific::none, 1, 0);
		case GL_SAMPLER_2D: return make(structure::sampler, datatype::_float, specific::none, 2, 0);
		case GL_SAMPLER_3D: return make(structure::sampler, datatype::_float, specific::none, 3, 0);
		case GL_SAMPLER_CUBE: return make(structure::sampler, datatype::_float, specific::cube, 0, 0);
		case GL_SAMPLER_1D_ARRAY: return make(structure::sampler, datatype::_float, specific::array, 1, 0);
		case GL_SAMPLER_2D_ARRAY: return make(structure::sampler, datatype::_float, specific::array, 2, 0);
		case GL_SAMPLER_2D_MULTISAMPLE: return make(structure::sampler, datatype::_float, specific::multisample, 2, 0);
		case GL_SAMPLER_2D_MULTISAMPLE_ARRAY: return make(structure::sampler, datatype::_float, specific::multisample_array, 2, 0);
		case GL_SAMPLER_BUFFER: return make(structure::sampler, datatype::_float, specific::buffer, 0, 0);
		case GL_SAMPLER_2D_RECT: return make(structure::sampler, datatype::_float, specific::rect, 2, 0);

		case GL_SAMPLER_1D_SHADOW: return make(structure::shadow_sampler, datatype::_float, specific::none, 1, 0);
		case GL_SAMPLER_2D_SHADOW: return make(structure::shadow_sampler, datatype::_float, specific::none, 2, 0);
		case GL_SAMPLER_1D_ARRAY_SHADOW: return make(structure::shadow_sampler, datatype::_float, specific::array, 1, 0);
		case GL_SAMPLER_2D_ARRAY_SHADOW: return make(structure::shadow_sampler, datatype::_float, specific::array, 2, 0);
		case GL_SAMPLER_CUBE_SHADOW: return make(structure::shadow_sampler, datatype::_float, specific::cube, 0, 0);
		case GL_SAMPLER_2D_RECT_SHADOW: return make(structure::shadow_sampler, datatype::_float, specific::rect, 2, 0);

		case GL_INT_SAMPLER_1D: return make(structure::sampler, datatype::_int, specific::none, 1, 0);
		case GL_INT_SAMPLER_2D: return make(structure::sampler, datatype::_int, specific::none, 2, 0);
		case GL_INT_SAMPLER_3D: return make(structure::sampler, datatype::_int, specific::none, 3, 0);
		case GL_INT_SAMPLER_CUBE: return make(structure::sampler, datatype::_int, specific::cube, 0, 0);
		case GL_INT_SAMPLER_1D_ARRAY: return make(structure::sampler, datatype::_int, specific::array, 1, 0);
		case GL_INT_SAMPLER_2D_ARRAY: return make(structure::sampler, datatype::_int, specific::array, 2, 0);
		case GL_INT_SAMPLER_2D_MULTISAMPLE: return make(structure::sampler, datatype::_int, specific::multisample, 2, 0);
		case GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return make(structure::sampler, datatype::_int, specific::multisample_array, 2, 0);
		case GL_INT_SAMPLER_BUFFER: return make(structure::sampler, datatype::_int, specific::buffer, 0, 0);
		case GL_INT_SAMPLER_2D_RECT: return make(structure::sampler, datatype::_int, specific::rect, 2, 0);

		case GL_UNSIGNED_INT_SAMPLER_1D: return make(structure::sampler, datatype::_unsigned, specific::none, 1, 0);
		case GL_UNSIGNED_INT_SAMPLER_2D: return make(structure::sampler, datatype::_unsigned, specific::none, 2, 0);
		case GL_UNSIGNED_INT_SAMPLER_3D: return make(structure::sampler, datatype::_unsigned, specific::none, 3, 0);
		case GL_UNSIGNED_INT_SAMPLER_CUBE: return make(structure::sampler, datatype::_unsigned, specific::cube, 0, 0);
		case GL_UNSIGNED_INT_SAMPLER_1D_ARRAY: return make(structure::sampler, datatype::_unsigned, specific::array, 1, 0);
		case GL_UNSIGNED_INT_SAMPLER_2D_ARRAY: return make(structure::sampler, datatype::_unsigned, specific::array, 2, 0);
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE: return make(structure::sampler, datatype::_unsigned, specific::multisample, 2, 0);
		case GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY: return make(structure::sampler, datatype::_unsigned, specific::multisample_array, 2, 0);
		case GL_UNSIGNED_INT_SAMPLER_BUFFER: return make(structure::sampler, datatype::_unsigned, specific::buffer, 0, 0);
		case GL_UNSIGNED_INT_SAMPLER_2D_RECT: return make(structure::sampler, datatype::_unsigned, specific::rect, 2, 0);

		case GL_IMAGE_1D: return make(structure::image, datatype::_float, specific::none, 1, 0);
		case GL_IMAGE_2D: return make(structure::image, datatype::_float, specific::none, 2, 0);
		case GL_IMAGE_3D: return make(structure::image, datatype::_float, specific::none, 3, 0);
		case GL_IMAGE_CUBE: return make(structure::image, datatype::_float, specific::cube, 0, 0);
		case GL_IMAGE_1D_ARRAY: return make(structure::image, datatype::_float, specific::array, 1, 0);
		case GL_IMAGE_2D_ARRAY: return make(structure::image, datatype::_float, specific::array, 2, 0);
		case GL_IMAGE_2D_MULTISAMPLE: return make(structure::image, datatype::_float, specific::multisample, 2, 0);
		case GL_IMAGE_2D_MULTISAMPLE_ARRAY: return make(structure::image, datatype::_float, specific::multisample_array, 2, 0);
		case GL_IMAGE_BUFFER: return make(structure::image, datatype::_float, specific::buffer, 0, 0);
		case GL_IMAGE_2D_RECT: return make(structure::image, datatype::_float, specific::rect, 2, 0);

		case GL_INT_IMAGE_1D: return make(structure::image, datatype::_int, specific::none, 1, 0);
		case GL_INT_IMAGE_2D: return make(structure::image, datatype::_int, specific::none, 2, 0);
		case GL_INT_IMAGE_3D: return make(structure::image, datatype::_int, specific::none, 3, 0);
		case GL_INT_IMAGE_CUBE: return make(structure::image, datatype::_int, specific::cube, 0, 0);
		case GL_INT_IMAGE_1D_ARRAY: return make(structure::image, datatype::_int, specific::array, 1, 0);
		case GL_INT_IMAGE_2D_ARRAY: return make(structure::image, datatype::_int, specific::array, 2, 0);
		case GL_INT_IMAGE_2D_MULTISAMPLE: return make(structure::image, datatype::_int, specific::multisample, 2, 0);
		case GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return make(structure::image, datatype::_int, specific::multisample_array, 2, 0);
		case GL_INT_IMAGE_BUFFER: return make(structure::image, datatype::_int, specific::buffer, 0, 0);
		case GL_INT_IMAGE_2D_RECT: return make(structure::image, datatype::_int, specific::rect, 2, 0);

		case GL_UNSIGNED_INT_IMAGE_1D: return make(structure::image, datatype::_unsigned, specific::none, 1, 0);
		case GL_UNSIGNED_INT_IMAGE_2D: return make(structure::image, datatype::_unsigned, specific::none, 2, 0);
		case GL_UNSIGNED_INT_IMAGE_3D: return make(structure::image, datatype::_unsigned, specific::none, 3, 0);
		case GL_UNSIGNED_INT_IMAGE_CUBE: return make(structure::image, datatype::_unsigned, specific::cube, 0, 0);
		case GL_UNSIGNED_INT_IMAGE_1D_ARRAY: return make(structure::image, datatype::_unsigned, specific::array, 1, 0);
		case GL_UNSIGNED_INT_IMAGE_2D_ARRAY: return make(structure::image, datatype::_unsigned, specific::array, 2, 0);
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE: return make(structure::image, datatype::_unsigned, specific::multisample, 2, 0);
		case GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY: return make(structure::image, datatype::_unsigned, specific::multisample_array, 2, 0);
		case GL_UNSIGNED_INT_IMAGE_BUFFER: return make(structure::image, datatype::_unsigned, specific::buffer, 0, 0);
		case GL_UNSIGNED_INT_IMAGE_2D_RECT: return make(structure::image, datatype::_unsigned, specific::rect, 2, 0);
	}
	log::error::critical << "Unsupported enum value.";
}

GLenum clap::gl::detail::convert::to_gl(shader::detail::variable_type_t::datatype datatype) {
	switch (datatype) {
		case shader::detail::variable_type_t::datatype::_float: return GL_FLOAT;
		case shader::detail::variable_type_t::datatype::_double: return GL_DOUBLE;
		case shader::detail::variable_type_t::datatype::_int: return GL_INT;
		case shader::detail::variable_type_t::datatype::_unsigned: return GL_UNSIGNED_INT;
		case shader::detail::variable_type_t::datatype::_bool: return GL_BOOL;
	}
	log::error::critical << "Unsupported enum value.";
}

size_t clap::gl::detail::convert::to_size(shader::detail::variable_type_t::datatype datatype) {
	switch (datatype) {
		case shader::detail::variable_type_t::datatype::_float: return sizeof(float);
		case shader::detail::variable_type_t::datatype::_double: return sizeof(double);
		case shader::detail::variable_type_t::datatype::_int: return sizeof(int);
		case shader::detail::variable_type_t::datatype::_unsigned: return sizeof(unsigned);
		case shader::detail::variable_type_t::datatype::_bool: return sizeof(bool);
	}
	log::error::critical << "Unsupported enum value.";
}

std::ostream &operator<<(std::ostream &stream, clap::gl::shader::type type) {
	switch (type) {
		case clap::gl::shader::type::fragment:				stream << "fragment"; break;
		case clap::gl::shader::type::vertex:					stream << "vertex"; break;
		case clap::gl::shader::type::geometry:				stream << "geometry"; break;
		case clap::gl::shader::type::compute:					stream << "compute"; break;
		case clap::gl::shader::type::tesselation_control:		stream << "tesselation control"; break;
		case clap::gl::shader::type::tesselation_evaluation:	stream << "tesselation evaluation"; break;
		default:
			clap::log::warning::major << "Unsupported enum value.";
	}
	return stream;
}


clap::gl::shader::detail::variable const &clap::gl::shader::variables::operator[](std::string name) {
	auto found = find(name);
	if (found != end())
		return found->second;
	else {
		log::error::major << "Attempting to access an non-existent variable.";
		log::info::major << "Requested name is '" << name << "'.";
	}
}
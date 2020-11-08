#include "gl/shader.hpp"
#include "gl/detail/context.hpp"

#include <fstream>

#include "essential/log.hpp"

#include "glad/glad.h"

clap::gl::shader::detail::object::object(type type) : id(0u) {
	if (auto context = access_context(); context)
		id = glCreateShader(gl::detail::convert::to_gl(type));
	if (id == 0)
		log::warning::critical << "Fail to create a shader object.";
	else
		log::message::minor << "Create a '" << type << "' " << *this << ".";
}
clap::gl::shader::detail::object::~object() {
	if (id)
		if (auto context = access_context(); context) {
			glDeleteShader(id);
			log::message::minor << "Destroy a " << *this << ".";
		}
}

std::optional<clap::gl::shader::detail::object> clap::gl::shader::from_source(type type, std::string_view source) {
	clap::gl::shader::detail::object out(type);

	if (source == "") {
		log::warning::critical << "Fail to compile a '" << type << "' " << out << ".";
		log::info::major << "Received source string is empty.";
		return std::nullopt;
	}

	if (auto context = out.access_context(); context) {
		auto const *glchar_source = static_cast<const GLchar *>(source.data());
		glShaderSource(out.id, 1, &glchar_source, NULL);
		glCompileShader(out.id);

		GLint compilation_successful;
		glGetShaderiv(out.id, GL_COMPILE_STATUS, &compilation_successful);
		if (compilation_successful) {
			log::message::minor << "Compile a '" << type << "' " << out << ".";
			return std::move(out);
		} else {
			GLsizei length;
			glGetShaderiv(out.id, GL_INFO_LOG_LENGTH, &length);

			GLchar *log = new GLchar[size_t(length) + 1];
			glGetShaderInfoLog(out.id, length, &length, log);
			log::warning::critical << "Fail to compile a '" << type << "' " << out << ".";
			log::info::critical << std::string(log);
			delete[] log;

			return std::nullopt;
		}
	}
}
std::optional<clap::gl::shader::detail::object> clap::gl::shader::from_file(type type, std::filesystem::path path) {
	std::ifstream filestream;
	filestream.open(path);
	if (!filestream) {
		log::warning::critical << "Fail to open '" << path << "' file. Make sure it exists.";
		log::info::critical << "While trying to compile a '" << type << "' shader object.";
		return std::nullopt;
	} else {
		std::string source{
			std::istreambuf_iterator<char>(filestream),
			std::istreambuf_iterator<char>()
		};

		return from_source(type, source);
	}
}

clap::gl::shader::detail::linker::linker() : id(0u) {
	if (auto context = access_context(); context)
		id = glCreateProgram();
	if (id == 0)
		log::warning::critical << "Fail to create a shader program.";
	else
		log::message::minor << "Create a " << *this << ".";
}

clap::gl::shader::detail::linker::~linker() {
	if (id)
		if (auto context = access_context(); context) {
			glDeleteProgram(id);
			log::warning::negligible << "Destroy a " << *this << ".";
			log::info::critical << "This linker object was never linked.";
		}
}

void clap::gl::shader::detail::linker::add(detail::object const &object) {
	if (id) {
		if (auto context = access_context(object); context) {
			glAttachShader(id, object.id);
			log::message::negligible << "Attach a " << object << " to a " << *this << ".";
		}
	} else {
		log::warning::major << "Fail to add a " << object << " to an invalid shader program linker object.";
		log::info::major << "Make sure it hasn't already been linked.";
	}
}

clap::gl::shader::detail::linker::operator clap::gl::shader::program() {
	if (id) {
		if (auto context = access_context(); context) {
			glLinkProgram(id);

			GLint linkage_successful;
			glGetProgramiv(id, GL_LINK_STATUS, &linkage_successful);
			if (linkage_successful) {
				log::message::minor << "Link a " << *this << ".";

				auto id_copy = id;
				id = 0;

				return program(id_copy);
			} else {
				GLsizei length;
				glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length);

				GLchar *log = new GLchar[size_t(length) + 1];
				glGetProgramInfoLog(id, length, &length, log);
				log::warning::critical << "Fail to link a " << *this << ".";
				log::info::critical << std::string(log);
				delete[] log;

				return program(0);
			}
		}
	} else {
		log::warning::major << "Fail to link an invalid linker object.";
		log::info::major << "Make sure it hasn't already been linked.";
		return program(0);
	}
}

template<typename gl_get_lambda_t, typename gl_get_location_lambda_t, typename create_variable_lambda_t>
void gl_variable_request_template(uint32_t program_id, GLenum active_type, GLenum active_type_length,
								  gl_get_lambda_t gl_get_lambda, gl_get_location_lambda_t gl_get_location_lambda,
								  create_variable_lambda_t create_variable_lambda) {
	GLint number, max_length, name_length, size;
	GLenum type;
	glGetProgramiv(program_id, active_type, &number);
	glGetProgramiv(program_id, active_type_length, &max_length);
	for (int i = 0; i < number; i++) {
		std::vector<GLchar> name(max_length);
		gl_get_lambda(program_id, i, max_length, &name_length, &size, &type, name.data());
		create_variable_lambda(name.data(), gl_get_location_lambda(program_id, name.data()), type, size);
	}
}
clap::gl::shader::program::program(unsigned id) : id(id) {
	if (id) {
		if (auto context = access_context(); context) {
			gl_variable_request_template(id, GL_ACTIVE_ATTRIBUTES, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, glGetActiveAttrib,
										 glGetAttribLocation, [this](auto name, auto location, auto datatype, auto size) {
											 attribute.emplace(
												 name,
												 variable::attribute(gl::detail::convert::to_attribute(datatype), size, location, name, *this)
											 );
										 });
			gl_variable_request_template(id, GL_ACTIVE_UNIFORMS, GL_ACTIVE_UNIFORM_MAX_LENGTH, glGetActiveUniform,
										 glGetUniformLocation, [this](auto name, auto location, auto datatype, auto size) {
											 uniform.emplace(
												 name,
												 variable::uniform(gl::detail::convert::to_uniform(datatype), size, location, name, *this)
											 );
										 });
			log::message::minor << "Request variable data for a " << *this << ".";
			log::info::major << "Uniform count: " << uniform.size() << ".";
			for (auto &entry : uniform)
				log::info::minor << '\t' << entry.first;
			log::info::major << "Attribute count: " << attribute.size() << ".";
			for (auto &entry : attribute)
				log::info::minor << '\t' << entry.first;
		}
	} else
		log::warning::major << "Create a shader program object with id = 0. It's considered invalid.";
}
clap::gl::shader::program::~program() {
	if (id)
		if (auto context = access_context(); context) {
			auto iterator = context->shader_program_stack.begin();
			while ((iterator = std::find(iterator, context->shader_program_stack.end(), this)) != context->shader_program_stack.end()) {
				log::warning::critical << "Destroy a currently used " << *this << ".";
				log::info::critical << "This could lead to some hard to trace errors.";
				*iterator = nullptr;
			}

			glDeleteProgram(id);
			log::message::minor << "Destroy a " << *this << ".";
		}
}

clap::essential::stack<clap::gl::shader::program const *>::iterator clap::gl::shader::detail::lock_program_callable::operator()() {
	if (auto context = program_ref.access_context(); context) {
		auto out = context->shader_program_stack.push(&program_ref);

		log::message::negligible << "Use a " << program_ref << ".";
		glUseProgram(program_ref.id);

		return out;
	}
}
void clap::gl::shader::detail::unlock_program_callable::operator()(clap::essential::stack<clap::gl::shader::program const *>::iterator iterator) {
	if (auto context = program_ref.access_context(); context) {
		if (context->shader_program_stack.is_front(iterator)) {
			if (auto active = context->shader_program_stack.pop(); active)
				log::message::negligible << "Stop using a " << *active << ".";
			else
				log::warning::minor << "Stop using a shader program object only after it was already destroyed.";

			std::remove_cvref<decltype(context->shader_program_stack.peek())>::type reactivated = nullptr;
			while (!context->shader_program_stack.empty() && !(reactivated = context->shader_program_stack.peek()))
				if (!reactivated) auto temp = context->shader_program_stack.pop();
			if (reactivated) {
				log::info::major << "Reuse a previously used " << *reactivated << ", the next element on the stack.";
				glUseProgram(reactivated->id);
			} else
				glUseProgram(0);
		} else {
			log::message::negligible << "Remove a " << program_ref << " from used shader::program stack.";
			log::info::major << "This doesn't affect active program in any way.";
			context->shader_program_stack.erase(iterator);
		}
	}
}

#pragma warning(push)
#pragma warning(disable : 4716)
template <typename T>
T const &clap::gl::shader::detail::unknown_variable_error(std::string_view const &name) {
	log::error::major << "Attempt to access an non-existent variable.";
	log::info::critical << "Requested name is '" << name << "'.";
}
template clap::gl::shader::variable::attribute const &clap::gl::shader::detail::unknown_variable_error(std::string_view const &name);
template clap::gl::shader::variable::uniform const &clap::gl::shader::detail::unknown_variable_error(std::string_view const &name);
#pragma warning(pop)

std::ostream &operator<<(std::ostream &stream, clap::gl::shader::variable::attribute const &variable) {
	return stream << variable.program_ref << " attribute \"" << variable.name << "\"";
}
std::ostream &operator<<(std::ostream &stream, clap::gl::shader::variable::uniform const &variable) {
	return stream << variable.program_ref << " uniform \"" << variable.name << "\"";
}

template<> void clap::gl::shader::variable::detail::set_vectors<1>(int location, unsigned count, float const *value) { glUniform1fv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<2>(int location, unsigned count, float const *value) { glUniform2fv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<3>(int location, unsigned count, float const *value) { glUniform3fv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<4>(int location, unsigned count, float const *value) { glUniform4fv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<1>(int location, unsigned count, int const *value) { glUniform1iv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<2>(int location, unsigned count, int const *value) { glUniform2iv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<3>(int location, unsigned count, int const *value) { glUniform3iv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<4>(int location, unsigned count, int const *value) { glUniform4iv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<1>(int location, unsigned count, unsigned const *value) { glUniform1uiv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<2>(int location, unsigned count, unsigned const *value) { glUniform2uiv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<3>(int location, unsigned count, unsigned const *value) { glUniform3uiv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<4>(int location, unsigned count, unsigned const *value) { glUniform4uiv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<1>(int location, unsigned count, double const *value) { glUniform1dv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<2>(int location, unsigned count, double const *value) { glUniform2dv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<3>(int location, unsigned count, double const *value) { glUniform3dv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<4>(int location, unsigned count, double const *value) { glUniform4dv(location, count, value); }
template<> void clap::gl::shader::variable::detail::set_vectors<1>(int location, unsigned count, bool const *value) {
	auto *tmp = new int[count];
	std::transform(value, value + count * 1, tmp, [](bool const &input) -> int { return static_cast<int>(input); });
	glUniform1iv(location, count, tmp);
	delete[] tmp;
}
template<> void clap::gl::shader::variable::detail::set_vectors<2>(int location, unsigned count, bool const *value) {
	auto *tmp = new int[count];
	std::transform(value, value + size_t(count) * 2, tmp, [](bool const &input) -> int { return static_cast<int>(input); });
	glUniform2iv(location, count, tmp);
	delete[] tmp;
}
template<> void clap::gl::shader::variable::detail::set_vectors<3>(int location, unsigned count, bool const *value) {
	auto *tmp = new int[count];
	std::transform(value, value + size_t(count) * 3, tmp, [](bool const &input) -> int { return static_cast<int>(input); });
	glUniform3iv(location, count, tmp);
	delete[] tmp;
}
template<> void clap::gl::shader::variable::detail::set_vectors<4>(int location, unsigned count, bool const *value) {
	auto *tmp = new int[count];
	std::transform(value, value + size_t(count) * 4, tmp, [](bool const &input) -> int { return static_cast<int>(input); });
	glUniform4iv(location, count, tmp);
	delete[] tmp;
}

template<> void clap::gl::shader::variable::detail::set_matrices<2, 2>(int location, unsigned count, float const *value) { glUniformMatrix2fv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<2, 2>(int location, unsigned count, double const *value) { glUniformMatrix2dv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<3, 3>(int location, unsigned count, float const *value) { glUniformMatrix3fv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<3, 3>(int location, unsigned count, double const *value) { glUniformMatrix3dv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<4, 4>(int location, unsigned count, float const *value) { glUniformMatrix4fv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<4, 4>(int location, unsigned count, double const *value) { glUniformMatrix4dv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<2, 3>(int location, unsigned count, float const *value) { glUniformMatrix2x3fv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<2, 3>(int location, unsigned count, double const *value) { glUniformMatrix2x3dv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<2, 4>(int location, unsigned count, float const *value) { glUniformMatrix2x4fv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<2, 4>(int location, unsigned count, double const *value) { glUniformMatrix2x4dv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<3, 2>(int location, unsigned count, float const *value) { glUniformMatrix3x2fv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<3, 2>(int location, unsigned count, double const *value) { glUniformMatrix3x2dv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<3, 4>(int location, unsigned count, float const *value) { glUniformMatrix3x4fv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<3, 4>(int location, unsigned count, double const *value) { glUniformMatrix3x4dv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<4, 2>(int location, unsigned count, float const *value) { glUniformMatrix4x2fv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<4, 2>(int location, unsigned count, double const *value) { glUniformMatrix4x2dv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<4, 3>(int location, unsigned count, float const *value) { glUniformMatrix4x3fv(location, count, false, value); }
template<> void clap::gl::shader::variable::detail::set_matrices<4, 3>(int location, unsigned count, double const *value) { glUniformMatrix4x3dv(location, count, false, value); }

void clap::gl::shader::variable::detail::variable_set_without_convertion_message(uniform const &variable) {
	log::message::negligible << "Set a " << variable << ".";
}
void clap::gl::shader::variable::detail::variable_set_with_convertion_message(uniform const &variable) {
	log::message::negligible << "Set a " << variable << " with a conversion to the variable type.";
}
void clap::gl::shader::variable::detail::cannot_convert_variable_type_error(uniform const &variable) {
	log::warning::major << "Fail to set a " << variable << " because a conversion to the variable type is impossible.";
	log::info::major << "'std::is_convertible<variable_type, passed_value_type>::value' is 'false'.";
}
void clap::gl::shader::variable::detail::variable_size_error(uniform const &variable, size_t expected, size_t received) {
	log::warning::major << "Fail to set a " << variable << " because the number of passed values is not the same as the number of values expected by the variable.";
	log::info::major << "Expected value count: " << expected << ".";
	log::info::major << "Received value count: " << received << ".";
}

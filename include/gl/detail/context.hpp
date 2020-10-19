#pragma once
#include "gl/detail/window.hpp"

#include "essential/guard.hpp"
#include "essential/stack.hpp"

#include "gl/vertex.hpp"

#include <memory>

namespace clap::gl::shader {
	class program;
	namespace detail {
		struct lock_program_callable;
		struct unlock_program_callable;
	}
}
namespace clap::gl::vertex {
	class buffer;
	namespace detail {
		struct bind_buffer_callable;
		struct unbind_buffer_callable;
	}
	enum class buffer_target;
}
namespace clap::gl::detail {
	class context;
	namespace detail {
		struct lock_context_callable {
			context &context_ref;
			void operator()();
		};
		struct unlock_context_callable {
			context &context_ref;
			void operator()();
		};
	}
	class context_guard
		: public essential::guard<std::mutex, detail::lock_context_callable, detail::unlock_context_callable> {
	public:
		context_guard(context &context_ref);
	};

	class context {
		friend context_guard;

		friend detail::lock_context_callable;
		friend detail::unlock_context_callable;

		friend clap::gl::shader::detail::lock_program_callable;
		friend clap::gl::shader::detail::unlock_program_callable;
		friend clap::gl::vertex::detail::bind_buffer_callable;
		friend clap::gl::vertex::detail::unbind_buffer_callable;

	public:
		context(std::u8string name, size_t width, size_t height);

		[[nodiscard]] static context* current();
		[[nodiscard]] context_guard make_current();

	protected:
		void activate();
		void deactivate();

	public:
		window::object window;

		inline clap::gl::shader::program const *active_shader_program() const {
			return !shader_program_stack.empty() 
				? shader_program_stack.peek() 
				: nullptr;
		}
		inline clap::gl::vertex::buffer const *bound_vertex_buffer(clap::gl::vertex::buffer_target target) const {
			return !vertex_buffer_stack[size_t(target)].empty() 
				? vertex_buffer_stack[size_t(target)].peek() 
				: nullptr;
		}

	protected:
		essential::stack<clap::gl::shader::program const*> shader_program_stack;
		essential::stack<clap::gl::vertex::buffer const*> 
			vertex_buffer_stack[size_t(gl::vertex::buffer_target::LAST)];

	private:
		std::mutex mutex;
		context *previous;
		size_t guard_counter;
	};
}
#pragma once
#include "gl/detail/window.hpp"

#include "essential/guard.hpp"
#include "essential/stack.hpp"

#include "gl/texture.hpp"
#include "gl/vertex.hpp"

#include <memory>
#include <vector>

namespace clap::gl::shader {
	class program;
	namespace detail {
		struct lock_program_callable;
		struct unlock_program_callable;
	}
}

namespace clap::gl::detail {
	namespace detail {
		class shader_program_stack_t {
			friend clap::gl::shader::detail::lock_program_callable;
			friend clap::gl::shader::detail::unlock_program_callable;

		public:
			inline clap::gl::shader::program const *active_shader_program() const {
				return !shader_program_stack.empty()
					? shader_program_stack.peek()
					: nullptr;
			}

		protected:
			essential::stack<clap::gl::shader::program const *> shader_program_stack;
		};

		class vertex_buffer_stack_t {
			friend clap::gl::vertex::detail::bind_buffer_callable;
			friend clap::gl::vertex::detail::unbind_buffer_callable;

		public:
			inline clap::gl::vertex::buffer const *bound_vertex_buffer(clap::gl::vertex::buffer_target target) const {
				return !vertex_buffer_stack[size_t(target)].empty()
					? vertex_buffer_stack[size_t(target)].peek()
					: nullptr;
			}

		protected:
			essential::stack<clap::gl::vertex::buffer const *>
				vertex_buffer_stack[size_t(gl::vertex::buffer_target::LAST)];
		};

		class vertex_array_stack_t {
			friend clap::gl::vertex::detail::bind_array_callable;
			friend clap::gl::vertex::detail::unbind_array_callable;

		public:
			inline clap::gl::vertex::array const *bound_vertex_array() const {
				return !vertex_array_stack.empty()
					? vertex_array_stack.peek()
					: nullptr;
			}

		protected:
			essential::stack<clap::gl::vertex::array const *> vertex_array_stack;
		};

		class texture_stack_t {
			template <texture::detail::target texture_type>
			friend struct texture::detail::bind_texture_callable;
			template <texture::detail::target texture_type>
			friend struct texture::detail::unbind_texture_callable;

		public:
			inline clap::gl::texture::detail::generic_interface bound_texture(size_t unit) const {
				if (unit >= texture_stack.size())
					return (clap::gl::texture::_1d *) nullptr;
				else {
					return !texture_stack.at(unit).empty()
						? texture_stack.at(unit).peek()
						: (clap::gl::texture::_1d *) nullptr;
				}
			}

		protected:
			std::vector<essential::stack<clap::gl::texture::detail::generic_interface>>
				texture_stack;
		};
	}

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

	class context 
		: public detail::shader_program_stack_t
		, public detail::vertex_buffer_stack_t
		, public detail::vertex_array_stack_t
		, public detail::texture_stack_t {

		friend context_guard;

		friend detail::lock_context_callable;
		friend detail::unlock_context_callable;

	public:
		context(std::u8string name, size_t width, size_t height);
		inline ~context() { 
			//std::scoped_lock(mutex);
		}

		[[nodiscard]] static context *current();
		[[nodiscard]] context_guard make_current();

	protected:
		void activate();
		void deactivate();

	public:
		window::object window;

	private:
		std::mutex mutex;
		context *previous;
		size_t guard_counter;
	};
}
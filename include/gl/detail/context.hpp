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
			friend shader::program;

		public:
			[[nodiscard]] inline clap::gl::shader::program const *active_shader_program() const {
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
			friend clap::gl::vertex::buffer;

		public:
			[[nodiscard]] inline clap::gl::vertex::buffer const *bound_vertex_buffer(clap::gl::vertex::buffer_target target) const {
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
			friend clap::gl::vertex::array;

		public:
			[[nodiscard]] inline clap::gl::vertex::array const *bound_vertex_array() const {
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

			template <texture::detail::target texture_type>
			friend class texture::detail::interface;

		public:
			[[nodiscard]] inline clap::gl::texture::detail::generic_interface bound_texture(size_t unit) const {
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
		struct activate_context_callable {
			context &context_ref;
			inline void operator()();
		};
		struct deactivate_context_callable {
			context &context_ref;
			inline void operator()();
		};

		class activation_guard
			: public essential::guard<std::mutex, detail::activate_context_callable, detail::deactivate_context_callable> {
		public:
			activation_guard(context &context_ref);
		};
		class access_guard {
		public:
			access_guard(context *context_ptr);
			~access_guard();

			access_guard(access_guard const &) = delete;
			access_guard &operator=(access_guard const &) = delete;
			access_guard(access_guard &&another) : context_ptr(another.context_ptr) { another.context_ptr = nullptr; }
			access_guard &operator=(access_guard &&) = delete;

			context *operator*() { return context_ptr; }
			context const *operator*() const { return context_ptr; }
			context *operator->() { return context_ptr; }
			context const *operator->() const { return context_ptr; }
			operator bool() const { return context_ptr; }
			auto operator<=>(access_guard const &another) const { return context_ptr <=> another.context_ptr; }
			bool operator!() const { return !bool(*this); }

		protected:
			context *context_ptr;
		};
	}

	class context
		: public detail::shader_program_stack_t
		, public detail::vertex_buffer_stack_t
		, public detail::vertex_array_stack_t
		, public detail::texture_stack_t {

		friend detail::activate_context_callable;
		friend detail::deactivate_context_callable;
		friend detail::activation_guard;
		friend detail::access_guard;

	public:
		using activation_guard = detail::activation_guard;
		using access_guard = detail::access_guard;

		context(std::u8string name, size_t width, size_t height);
		inline ~context() { std::lock_guard _(access_mutex); }

		[[nodiscard]] static access_guard current();
		[[nodiscard]] activation_guard make_current();

		friend inline std::ostream &operator<<(std::ostream &stream, context const &another) {
			return stream << "OpenGL context for a " << another.window;
		}

	protected:
		void activate();
		void deactivate();

	public:
		window::object window;

	private:
		std::mutex activation_mutex;
		std::shared_mutex access_mutex;
	};

	inline void detail::activate_context_callable::operator()() { context_ref.activate(); }
	inline void detail::deactivate_context_callable::operator()() { context_ref.deactivate(); }
}
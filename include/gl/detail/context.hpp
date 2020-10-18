#pragma once
#include "gl/detail/window.hpp"

#include "essential/guard.hpp"
#include "essential/stack.hpp"

#include <memory>

namespace clap::gl::shader {
	class program;
	namespace detail {
		struct lock_program_callable;
		struct unlock_program_callable;
	}
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

	public:
		context(std::u8string name, size_t width, size_t height);

		[[nodiscard]] static context* current();
		[[nodiscard]] context_guard make_current();

	protected:
		void activate();
		void deactivate();

	public:
		window::object window;

	protected:
		essential::stack<clap::gl::shader::program const*> shader_program_stack;

	private:
		std::mutex mutex;
		context *previous;
		size_t guard_counter;
	};
}
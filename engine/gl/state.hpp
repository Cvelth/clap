#pragma once
namespace engine::gl {
	namespace buffer {
		enum class target;
		namespace detail {
			class indexed;
		}
	}
	namespace shader {
		class program;
	}

	class state {
	public:
		static bool load();
		static void ensure_loaded();

		static void bind(buffer::target const &target, buffer::detail::indexed *buffer);
		static void unbind(buffer::target const &target);
		static buffer::detail::indexed *bound(buffer::target const &target);

		static void use(shader::program *program);
		static shader::program *being_used();

		state() = delete;
		state(state const &other) = delete;
		state(state &&other) = delete;

	private:
		static buffer::detail::indexed *bound_buffers[14];
		static shader::program *program_used;
	};
}


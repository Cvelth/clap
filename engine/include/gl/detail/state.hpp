#pragma once
#include <optional>
#include "gl/buffer.hpp"
#include "gl/vertex_array.hpp"

namespace engine::gl {
	namespace buffer {
		enum class target;
		namespace detail {
			class indexed;
		}
	}
	namespace vertex_array {
		namespace detail {
			class indexed;
		}
	}
	namespace shader {
		class program;
	}
}

namespace engine::gl::detail {
	class state {
	public:
		static bool load();
		static void ensure_loaded();

		static void bind(buffer::target const &target, buffer::detail::indexed &&buffer);
		static void unbind(buffer::target const &target);
		static std::optional<buffer::detail::indexed> const &bound(buffer::target const &target);

		static void bind(vertex_array::detail::indexed &&vertex_array);
		static void unbind();
		static std::optional<vertex_array::detail::indexed> const &bound();

		static void use(shader::program *program);
		static shader::program *being_used();

		state() = delete;
		state(state const &other) = delete;
		state(state &&other) = delete;

	private:
		static std::optional<buffer::detail::indexed> bound_buffers[14];
		static std::optional<vertex_array::detail::indexed> bound_vertex_array;
		static shader::program *program_used;
	};
}


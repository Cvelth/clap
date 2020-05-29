#pragma once
#include "buffer.hpp"
namespace engine::gl {
	class state {
	public:
		static bool load();
		static void check_load();

		static void bind(buffer::target const &target, buffer::detail::indexed *buffer);
		static void unbind(buffer::target const &target);
		static buffer::detail::indexed *bound(buffer::target const &target);

		state() = delete;
		state(state const &other) = delete;
		state(state &&other) = delete;

	private:
		static buffer::detail::indexed *bound_buffers[14];
	};
}


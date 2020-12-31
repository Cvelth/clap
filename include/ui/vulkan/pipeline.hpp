#pragma once
#include "precompiled/ui.hpp"

#include "ui/resource.hpp"

namespace clap::ui::vulkan {
	namespace detail {
		struct shader_container_t {
			std::vector<clap::resource_manager::detail::shader::loaded_t> loaded;
			std::vector<vk::PipelineShaderStageCreateInfo> stage_info;
		public:
			std::vector<vk::PipelineShaderStageCreateInfo> const *operator->() const {
				return &stage_info;
			}
		};
		void add_shader(shader_container_t &, clap::resource_manager::detail::shader::loaded_t &&);
	}

	template <std::same_as<clap::resource_manager::detail::shader::loaded_t> ...Ts>
	inline detail::shader_container_t shaders(Ts &&...ts) {
		detail::shader_container_t output;
		[](...) {}((detail::add_shader(output, std::move(ts)), 0)...);
		return output;
	}
}
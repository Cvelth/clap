#include "precompiled/ui.hpp"

#include "ui/vulkan/pipeline.hpp"

void clap::ui::vulkan::detail::add_shader(shader_container_t &container,
										  clap::resource_manager::detail::shader::loaded_t &&shader) {
	if (shader) {
		container.loaded.emplace_back(std::move(shader));
		container.stage_info.emplace_back(vk::PipelineShaderStageCreateInfo{
			.stage = container.loaded.back().stage,
			.module = **container.loaded.back().module,
			.pName = "main"
		});
	}
}
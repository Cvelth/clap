#include "precompiled/ui.hpp"

#include "ui/vulkan/core.hpp"
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

clap::ui::vulkan::pipeline::constructor::operator vk::UniquePipeline() {
	if (auto &device = clap::ui::vulkan::device(); device) {
		vk::PipelineVertexInputStateCreateInfo vertex_input_info = {
			.vertexBindingDescriptionCount = static_cast<uint32_t>(binding_descriptions.size()),
			.pVertexBindingDescriptions = binding_descriptions.data(),
			.vertexAttributeDescriptionCount = static_cast<uint32_t>(attribute_descriptions.size()),
			.pVertexAttributeDescriptions = attribute_descriptions.data()
		};
		vk::PipelineViewportStateCreateInfo viewport_info = {
			.viewportCount = 1,
			.scissorCount = 1
		};

		color_blend_create_info.attachmentCount = static_cast<uint32_t>(color_blend_attachments.size());
		color_blend_create_info.pAttachments = color_blend_attachments.data();

		std::vector<vk::DynamicState> dynamic_states_copy = dynamic_states;
		dynamic_states_copy.emplace_back(vk::DynamicState::eViewport);
		dynamic_states_copy.emplace_back(vk::DynamicState::eScissor);
		vk::PipelineDynamicStateCreateInfo dynamic_info = {
			.dynamicStateCount = static_cast<uint32_t>(dynamic_states_copy.size()),
			.pDynamicStates = dynamic_states_copy.data()
		};

		vk::GraphicsPipelineCreateInfo pipeline_info = {
			.stageCount = static_cast<uint32_t>(shader_container->size()),
			.pStages = shader_container->data(),
			.pVertexInputState = &vertex_input_info,
			.pInputAssemblyState = &input_assembly_create_info,
			.pViewportState = &viewport_info,
			.pRasterizationState = &rasterization_create_info,
			.pMultisampleState = &multisample_create_info,
			.pDepthStencilState = depth_stencil_create_info ? &*depth_stencil_create_info : nullptr,
			.pColorBlendState = &color_blend_create_info,
			.pDynamicState = &dynamic_info,

			.layout = (layout_ptr ? *layout_ptr : nullptr),
			.renderPass = (render_pass_ptr ? *render_pass_ptr : nullptr),
			.subpass = subpass_index,
			.basePipelineHandle = (base_ptr ? *base_ptr : nullptr),
			.basePipelineIndex = base_index
		};
		return device.createGraphicsPipelineUnique(cache_ptr ? *cache_ptr : nullptr, pipeline_info).value;
	}
	return {};
}
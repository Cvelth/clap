#include "precompiled/triangle.hpp"

class my_zone : public clap::ui::zone {
public:
	my_zone() : clap::ui::zone("Triangle example", 1280, 720) {
		on_initialize = [this] { initialize(); };
		on_update = [this](auto const &) { update(); };
	}
protected:
	void initialize_pipeline() {
		if (auto &device = clap::ui::vulkan::device(); device)
			if (auto window = this->window(); window) {
				auto shader_stages = clap::ui::vulkan::shaders(
					clap::resource::load::shader::vertex["triangle"],
					clap::resource::load::shader::fragment["triangle"]
				);
				vk::PipelineVertexInputStateCreateInfo vertex_input_info = {
					.vertexBindingDescriptionCount = 0,
					.pVertexBindingDescriptions = nullptr,
					.vertexAttributeDescriptionCount = 0,
					.pVertexAttributeDescriptions = nullptr
				};
				vk::PipelineInputAssemblyStateCreateInfo input_assembly_info = {
					.topology = vk::PrimitiveTopology::eTriangleFan,
					.primitiveRestartEnable = false
				};
				vk::Viewport viewport = {
					.x = 0.f,
					.y = 0.f,
					.width = static_cast<float>(this->width()),
					.height = static_cast<float>(this->height()),
					.minDepth = 0.f,
					.maxDepth = 1.f
				};
				vk::Rect2D scissor = {
					.offset = {0u, 0u},
					.extent = window->swapchain_extent
				};
				vk::PipelineViewportStateCreateInfo viewport_info = {
					.viewportCount = 1,
					.pViewports = &viewport,
					.scissorCount = 1,
					.pScissors = &scissor
				};
				vk::PipelineRasterizationStateCreateInfo rasterization_info = {
					.depthClampEnable = false,
					.rasterizerDiscardEnable = false,
					.polygonMode = vk::PolygonMode::eFill,
					.cullMode = vk::CullModeFlagBits::eNone,
					.frontFace = vk::FrontFace::eClockwise,
					.depthBiasEnable = false,
					.lineWidth = 1.f,
				};
				vk::PipelineMultisampleStateCreateInfo multisample_info = {
					.rasterizationSamples = vk::SampleCountFlagBits::e1,
					.sampleShadingEnable = false
				};
				//vk::PipelineDepthStencilStateCreateInfo depth_stencil_info = {/* ... */}
				vk::PipelineColorBlendAttachmentState color_blend_attachment = {
					.blendEnable = false,
					.srcColorBlendFactor = vk::BlendFactor::eOneMinusDstAlpha,
					.dstColorBlendFactor = vk::BlendFactor::eDstAlpha,
					.colorBlendOp = vk::BlendOp::eAdd,
					.srcAlphaBlendFactor = vk::BlendFactor::eOne,
					.dstAlphaBlendFactor = vk::BlendFactor::eZero,
					.alphaBlendOp = vk::BlendOp::eAdd,
					.colorWriteMask = vk::ColorComponentFlagBits::eR
									| vk::ColorComponentFlagBits::eG
									| vk::ColorComponentFlagBits::eB
									| vk::ColorComponentFlagBits::eA
				};
				vk::PipelineColorBlendStateCreateInfo color_blend_info = {
					.logicOpEnable = true,
					.logicOp = vk::LogicOp::eCopy,
					.attachmentCount = 1,
					.pAttachments = &color_blend_attachment
				};
				std::array dynamic_states = {
					//vk::DynamicState::eViewport,
					vk::DynamicState::eLineWidth,
					//vk::DynamicState::eScissor
				};
				vk::PipelineDynamicStateCreateInfo dynamic_info = {
					.dynamicStateCount = static_cast<uint32_t>(dynamic_states.size()),
					.pDynamicStates = dynamic_states.data()
				};
				vk::PipelineLayoutCreateInfo layout_info = {
					.setLayoutCount = 0,
					.pSetLayouts = nullptr,
					.pushConstantRangeCount = 0,
					.pPushConstantRanges = nullptr
				};
				pipeline_layout = device.createPipelineLayoutUnique(layout_info);

				vk::GraphicsPipelineCreateInfo pipeline_info = {
					.stageCount = static_cast<uint32_t>(shader_stages->size()),
					.pStages = shader_stages->data(),
					.pVertexInputState = &vertex_input_info,
					.pInputAssemblyState = &input_assembly_info,
					.pViewportState = &viewport_info,
					.pRasterizationState = &rasterization_info,
					.pMultisampleState = &multisample_info,
					.pDepthStencilState = nullptr,
					.pColorBlendState = &color_blend_info,
					.pDynamicState = &dynamic_info,

					.layout = *pipeline_layout,
					.renderPass = window->render_pass,
					.subpass = 0,
					.basePipelineHandle = nullptr,
					.basePipelineIndex = -1
				};
				pipeline = device.createGraphicsPipelineUnique(nullptr, pipeline_info).value;
			}
	}
	void initialize_command_buffers() {
		if (auto &device = clap::ui::vulkan::device(); device)
			if (auto window = this->window(); window) {
				vk::CommandBufferAllocateInfo command_buffer_info = {
					.commandPool = clap::ui::vulkan::command_pool(),
					.level = vk::CommandBufferLevel::ePrimary,
					.commandBufferCount = static_cast<uint32_t>(window->framebuffers.size())
				};
				command_buffers = device.allocateCommandBuffersUnique(command_buffer_info);

				for (size_t counter = 0u; auto &buffer : command_buffers) {
					buffer->begin(vk::CommandBufferBeginInfo{});

					vk::ClearValue clear_color(std::array{ 0.8f, 0.4f, 0.9f, 1.f });
					vk::RenderPassBeginInfo render_pass_begin_info = {
						.renderPass = window->render_pass,
						.framebuffer = *window->framebuffers[counter++],
						.renderArea = vk::Rect2D {
							.offset = {0, 0},
							.extent = window->swapchain_extent
						},
						.clearValueCount = 1,
						.pClearValues = &clear_color
					};
					buffer->beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
					buffer->bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline);
					buffer->draw(3, 1, 0, 0);
					buffer->endRenderPass();
					buffer->end();
				}
			}
	}
	void initialize() {
		initialize_pipeline();
		initialize_command_buffers();
		if (auto &device = clap::ui::vulkan::device(); device) {
			render_begin_semaphore = device.createSemaphoreUnique({});
			render_end_semaphore = device.createSemaphoreUnique({});
		}
	}
	void update() {
		if (auto &device = clap::ui::vulkan::device(); device)
			if (auto window = this->window(); window)
				if (auto &queue = clap::ui::vulkan::queue(); queue) {
					uint32_t image_index = device.acquireNextImageKHR(window->swapchain,
																	  std::numeric_limits<uint64_t>::max(),
																	  *render_begin_semaphore, nullptr).value;
					std::array wait_stages = { vk::PipelineStageFlags {
						vk::PipelineStageFlagBits::eColorAttachmentOutput
					} };
					vk::SubmitInfo submit_info = {
						.waitSemaphoreCount = 1,
						.pWaitSemaphores = &*render_begin_semaphore,
						.pWaitDstStageMask = wait_stages.data(),
						.commandBufferCount = 1,
						.pCommandBuffers = &*command_buffers[image_index],
						.signalSemaphoreCount = 1,
						.pSignalSemaphores = &*render_end_semaphore
					};
					queue.submit(std::array{ submit_info }, nullptr);

					vk::PresentInfoKHR present_info = {
						.waitSemaphoreCount = 1,
						.pWaitSemaphores = &*render_end_semaphore,
						.swapchainCount = 1,
						.pSwapchains = &window->swapchain,
						.pImageIndices = &image_index
					};
					[[maybe_unused]] auto result = queue.presentKHR(present_info);
					queue.waitIdle();
				}
	}
protected:
	vk::UniquePipelineLayout pipeline_layout;
	vk::UniquePipeline pipeline;
	std::vector<vk::UniqueCommandBuffer> command_buffers;

	vk::UniqueSemaphore render_begin_semaphore;
	vk::UniqueSemaphore render_end_semaphore;
};

void initialize_logger() {
	clap::logger::add(std::cout,
					  clap::logger::mask::every_error |
					  clap::logger::mask::minor_warnings |
					  clap::logger::mask::major_messages |
					  clap::logger::mask::every_custom_entry);
	clap::logger::add("problems/",
					  clap::logger::mask::every_error |
					  clap::logger::mask::minor_warnings |
					  clap::logger::mask::no_messages |
					  clap::logger::mask::no_custom_entries);
	clap::logger::add("full/", clap::logger::mask::everything);
	clap::logger::add("tagged/", clap::logger::mask::everything).with_tags();
}

int main(int argc, char **argv) {
	initialize_logger();

	[[maybe_unused]] my_zone zone;
	return clap::ui::loop(argc, argv);
}
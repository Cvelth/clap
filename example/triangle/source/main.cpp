#include "precompiled/triangle.hpp"

class my_zone : public clap::ui::zone {
public:
	my_zone() : clap::ui::zone("Triangle example", 1280, 720) {
		on_initialize = [this] { initialize(); };
		on_update = [this](auto const &) { update(); };
		on_resize = [this](size_t width, size_t height) { return resize(width, height); };
	}
protected:
	inline void initialize_pipeline() {
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
				vk::PipelineViewportStateCreateInfo viewport_info = {
					.viewportCount = 1,
					.scissorCount = 1
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
					vk::DynamicState::eViewport,
					vk::DynamicState::eScissor
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
	inline void initialize_command_buffers() {
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
							.extent = window->extent
						},
						.clearValueCount = 1,
						.pClearValues = &clear_color
					};
					buffer->beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
					buffer->bindPipeline(vk::PipelineBindPoint::eGraphics, *pipeline);
					
					buffer->setViewport(0, std::array{vk::Viewport {
						.x = 0,
						.y = 0,
						.width = static_cast<float>(window->extent.width),
						.height = static_cast<float>(window->extent.height),
						.minDepth = 0,
						.maxDepth = 1
					}});
					buffer->setScissor(0, std::array{vk::Rect2D {
						.offset = {0u, 0u},
						.extent = window->extent
					}});
					buffer->draw(3, 1, 0, 0);
					buffer->endRenderPass();
					buffer->end();
				}
			}
	}
	inline void initialize_synchronization() {
		if (auto &device = clap::ui::vulkan::device(); device)
			if (auto window = this->window(); window) {
				synchronization.resize(window->framebuffers.size());
				for (auto &sync : synchronization) {
					sync.framebuffer_ready_semaphore = device.createSemaphoreUnique({});
					sync.queue_submitted_semaphore = device.createSemaphoreUnique({});
					sync.queue_submitted_fence = device.createFenceUnique(vk::FenceCreateInfo{
						.flags = vk::FenceCreateFlagBits::eSignaled 
					});
				}
			}
	}
	void initialize() {
		initialize_pipeline();
		initialize_command_buffers();
		initialize_synchronization();
	}
	void update() {
		if (auto &device = clap::ui::vulkan::device(); device)
			if (auto window = this->window(); window)
				if (auto &queue = clap::ui::vulkan::queue(); queue) {
					auto temporary_semaphore = device.createSemaphoreUnique({});
					auto [result, framebuffer_index] = device.acquireNextImageKHR(
						window->swapchain, std::numeric_limits<uint64_t>::max(),
						*temporary_semaphore, nullptr
					);
					if (result == vk::Result::eSuccess || result == vk::Result::eSuboptimalKHR) {
						auto &sync = synchronization[framebuffer_index];

						[[maybe_unused]] auto wait_result = device.waitForFences(
							std::array{ *sync.queue_submitted_fence },
							true, std::numeric_limits<uint64_t>::max()
						);
						device.resetFences(std::array{ *sync.queue_submitted_fence });
						sync.framebuffer_ready_semaphore = std::move(temporary_semaphore);

						vk::PipelineStageFlags wait_stage = 
							vk::PipelineStageFlagBits::eColorAttachmentOutput;
						vk::SubmitInfo submit_info = {
							.waitSemaphoreCount = 1,
							.pWaitSemaphores = &*sync.framebuffer_ready_semaphore,
							.pWaitDstStageMask = &wait_stage,
							.commandBufferCount = 1,
							.pCommandBuffers = &*command_buffers[framebuffer_index],
							.signalSemaphoreCount = 1,
							.pSignalSemaphores = &*sync.queue_submitted_semaphore
						};
						queue.submit(std::array{ submit_info }, *sync.queue_submitted_fence);

						vk::PresentInfoKHR present_info = {
							.waitSemaphoreCount = 1,
							.pWaitSemaphores = &*sync.queue_submitted_semaphore,
							.swapchainCount = 1,
							.pSwapchains = &window->swapchain,
							.pImageIndices = &framebuffer_index
						};
						[[maybe_unused]] auto present_result = vkQueuePresentKHR(
							queue, &present_info.operator const VkPresentInfoKHR &()
						);
					}
				}
	}
	bool resize(size_t new_width, size_t new_height) {
		initialize_command_buffers();
		clap::log << "Got resized to (" << new_width << ", " << new_height << ").";
		return true;
	}
protected:
	vk::UniquePipelineLayout pipeline_layout;
	vk::UniquePipeline pipeline;
	std::vector<vk::UniqueCommandBuffer> command_buffers;

	struct synchronization_t {
		vk::UniqueSemaphore framebuffer_ready_semaphore;
		vk::UniqueSemaphore queue_submitted_semaphore;
		vk::UniqueFence queue_submitted_fence;
	};
	std::vector<synchronization_t> synchronization;
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
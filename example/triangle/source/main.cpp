#include "precompiled/triangle.hpp"

class my_zone : public clap::ui::zone {
protected:
	vk::UniquePipelineLayout pipeline_layout;
	vk::UniqueRenderPass render_pass;
	vk::UniquePipeline pipeline;
public:
	void initialize_pipeline() {
		if (auto &device = clap::ui::vulkan::device(); device) {
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
					.extent = {0u, 0u}
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

				vk::AttachmentDescription color_attachment = {
					.format = window->swapchain_image_format,
					.samples = vk::SampleCountFlagBits::e1,
					.loadOp = vk::AttachmentLoadOp::eClear,
					.storeOp = vk::AttachmentStoreOp::eStore,
					.stencilLoadOp = vk::AttachmentLoadOp::eDontCare,
					.stencilStoreOp = vk::AttachmentStoreOp::eDontCare,
					.initialLayout = vk::ImageLayout::eUndefined,
					.finalLayout = vk::ImageLayout::ePresentSrcKHR
				};
				vk::AttachmentReference color_attachment_ref = {
					.attachment = 0,
					.layout = vk::ImageLayout::eColorAttachmentOptimal
				};
				vk::SubpassDescription subpass = {
					.pipelineBindPoint = vk::PipelineBindPoint::eGraphics,
					.colorAttachmentCount = 1,
					.pColorAttachments = &color_attachment_ref
				};

				vk::RenderPassCreateInfo render_pass_info = {
					.attachmentCount = 1,
					.pAttachments = &color_attachment,
					.subpassCount = 1,
					.pSubpasses = &subpass
				};
				render_pass = device.createRenderPassUnique(render_pass_info);

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
					.renderPass = *render_pass,
					.subpass = 0,
					.basePipelineHandle = nullptr,
					.basePipelineIndex = -1
				};
				pipeline = device.createGraphicsPipelineUnique(nullptr, pipeline_info).value;
			}
		}
	}

	my_zone() : clap::ui::zone("Triangle example", 1280, 720) {
		on_initialize = [this] { initialize_pipeline(); };
		on_update = [](auto const &) {
			std::this_thread::sleep_for(4ms);
			return false;
		};
	}
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
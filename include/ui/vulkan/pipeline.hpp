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
	namespace pipeline {
		class constructor {
		public:
			template <std::same_as<clap::resource_manager::detail::shader::loaded_t> ...Ts>
			inline constructor &shaders(Ts &&...ts) {
				[](...) {}((detail::add_shader(shader_container, std::move(ts)), 0)...);
				return *this;
			}
			inline constructor &binding_description(vk::VertexInputBindingDescription const &description) {
				binding_descriptions.emplace_back(description);
				return *this;
			}
			inline constructor &binding_description(vk::VertexInputBindingDescription &&description) {
				binding_descriptions.emplace_back(std::move(description));
				return *this;
			}
			inline constructor &binding_description(uint32_t binding, uint32_t stride,
													vk::VertexInputRate inputRate) {
				binding_descriptions.emplace_back(vk::VertexInputBindingDescription {
					.binding = binding,
					.stride = stride,
					.inputRate = inputRate
				});
				return *this;
			}

			inline constructor &attribute_description(vk::VertexInputAttributeDescription const &description) {
				attribute_descriptions.emplace_back(description);
				return *this;
			}
			inline constructor &attribute_description(vk::VertexInputAttributeDescription &&description) {
				attribute_descriptions.emplace_back(std::move(description));
				return *this;
			}
			inline constructor &attribute_description(uint32_t location, uint32_t binding,
													  vk::Format format, uint32_t offset) {
				attribute_descriptions.emplace_back(vk::VertexInputAttributeDescription {
					.location = location,
					.binding = binding,
					.format = format,
					.offset = offset
				});
				return *this;
			}

			inline constructor &input_assembly(vk::PrimitiveTopology topology, bool enable_primitive_restart) {
				input_assembly_create_info = vk::PipelineInputAssemblyStateCreateInfo {
					.topology = topology,
					.primitiveRestartEnable = enable_primitive_restart
				};
				return *this;
			}
			inline constructor &rasterization(vk::PipelineRasterizationStateCreateInfo const &create_info) {
				rasterization_create_info = create_info;
				return *this;
			}
			inline constructor &rasterization(vk::PipelineRasterizationStateCreateInfo &&create_info) {
				rasterization_create_info = std::move(create_info);
				return *this;
			}
			inline constructor &multisample(vk::PipelineMultisampleStateCreateInfo const &create_info) {
				multisample_create_info = create_info;
				return *this;
			}
			inline constructor &multisample(vk::PipelineMultisampleStateCreateInfo &&create_info) {
				multisample_create_info = std::move(create_info);
				return *this;
			}
			inline constructor &color_blend(vk::PipelineColorBlendAttachmentState const &attachment) {
				color_blend_attachments.emplace_back(attachment);
				return *this;
			}
			inline constructor &color_blend(vk::PipelineColorBlendAttachmentState &&attachment) {
				color_blend_attachments.emplace_back(std::move(attachment));
				return *this;
			}
			inline constructor &color_blend(bool enable_logic_operation, vk::LogicOp logic_operation,
											vk::ArrayWrapper1D<float, 4> blend_contants) {
				color_blend_create_info.logicOpEnable = enable_logic_operation;
				color_blend_create_info.logicOp = logic_operation;
				color_blend_create_info.blendConstants = blend_contants;
				return *this;
			}
			inline constructor &dynamic_state(vk::DynamicState state) {
				dynamic_states.emplace_back(state);
				return *this;
			}
			inline constructor &layout(vk::PipelineLayout &layout) {
				layout_ptr = &layout;
				return *this;
			}
			inline constructor &render_pass(vk::RenderPass &render_pass) {
				render_pass_ptr = &render_pass;
				return *this;
			}
			inline constructor &subpass(uint32_t subpass) {
				subpass_index = subpass;
				return *this;
			}
			inline constructor &base(vk::Pipeline &pipeline, int32_t index) {
				base_ptr = &pipeline;
				base_index = index;
				return *this;
			}
			inline constructor &cache(vk::PipelineCache &cache) {
				cache_ptr = &cache;
				return *this;
			}

			operator vk::UniquePipeline();
			auto operator *() { return operator vk::UniquePipeline(); }

		protected:
			detail::shader_container_t shader_container;
			std::vector<vk::VertexInputBindingDescription> binding_descriptions;
			std::vector<vk::VertexInputAttributeDescription> attribute_descriptions;
			vk::PipelineInputAssemblyStateCreateInfo input_assembly_create_info = {
				.topology = vk::PrimitiveTopology::eTriangleList,
				.primitiveRestartEnable = true
			};
			vk::PipelineRasterizationStateCreateInfo rasterization_create_info = {
				.depthClampEnable = false,
				.rasterizerDiscardEnable = false,
				.polygonMode = vk::PolygonMode::eFill,
				.cullMode = vk::CullModeFlagBits::eNone,
				.frontFace = vk::FrontFace::eClockwise,
				.depthBiasEnable = false,
				.lineWidth = 1.f
			};
			vk::PipelineMultisampleStateCreateInfo multisample_create_info{
				.rasterizationSamples = vk::SampleCountFlagBits::e1,
				.sampleShadingEnable = false
			};
			std::optional<vk::PipelineDepthStencilStateCreateInfo> depth_stencil_create_info = std::nullopt;
			std::vector<vk::PipelineColorBlendAttachmentState> color_blend_attachments;
			vk::PipelineColorBlendStateCreateInfo color_blend_create_info{
				.logicOpEnable = false,
				.logicOp = vk::LogicOp::eClear
			};
			std::vector<vk::DynamicState> dynamic_states;
			vk::PipelineLayout *layout_ptr = nullptr;
			vk::RenderPass *render_pass_ptr = nullptr;
			uint32_t subpass_index = 0;
			vk::Pipeline *base_ptr = nullptr;
			int32_t base_index = -1;
			vk::PipelineCache *cache_ptr = nullptr;
		};
	}
}
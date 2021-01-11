#include "precompiled/triangle.hpp"

class my_zone : public clap::ui::zone {
public:
	my_zone() : clap::ui::zone("Triangle example", 1280, 720) {
		on_initialize = [this] { return initialize(); };
		on_render = [this](vk::CommandBuffer &buffer) { render(buffer); };
		on_update = [this](auto const &) { return true; };
		on_resize = [this](size_t, size_t) { return true; };

		vk::PipelineLayoutCreateInfo layout_info = {
			.setLayoutCount = 0,
			.pSetLayouts = nullptr,
			.pushConstantRangeCount = 0,
			.pPushConstantRanges = nullptr
		};
		if (auto &device = clap::ui::vulkan::device(); device)
			pipeline_layout = device.createPipelineLayoutUnique(layout_info);

		clear_color = std::array{ 1.f, 1.f, 1.f, 1.f };
	}
protected:
	inline vk::UniquePipeline initialize() {
		clap::ui::vulkan::pipeline::constructor pipeline_parameters;
		pipeline_parameters.shaders(clap::resource::load::shader::vertex["hlsl_triangle"],
									clap::resource::load::shader::fragment["hlsl_triangle"]);
		pipeline_parameters.input_assembly(vk::PrimitiveTopology::eTriangleFan, false);
		pipeline_parameters.color_blend(vk::PipelineColorBlendAttachmentState {
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
		});

		if (pipeline_layout)
			pipeline_parameters.layout(*pipeline_layout);
		if (auto window = this->window(); window && window->render_pass)
			pipeline_parameters.render_pass(window->render_pass);

		return pipeline_parameters;
	}
	void render(vk::CommandBuffer &buffer) {
		buffer.draw(3, 1, 0, 0);
	}
protected:
	vk::UniquePipelineLayout pipeline_layout;
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
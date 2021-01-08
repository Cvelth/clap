#include "precompiled/ui.hpp"

#include "resource/resource.hpp"
#include "ui/detail/manager.hpp"
#include "ui/vulkan/core.hpp"
#include "ui/vulkan/window.hpp"
#include "ui/compound.hpp"
#include "ui/zone.hpp"

clap::ui::zone::zone(std::string_view title, size_t width, size_t height)
	: get_identifier([] { return "a free zone"; })
	, state(when_free{ std::string(title), std::make_shared<vulkan::window>(width, height, title) })
	, size{width, height} {
	std::visit(utility::overload{
		[this, &title, &width, &height](when_free const &state) {
			if (state.window) {
				clap::log << cL::message << cL::major << "clap"_tag << "ui"_tag << "zone"_tag
					<< "Create a new free zone: " << title 
					<< " (" << width << ", " << height << ")";
				detail::manager::add(*this);
			} else
				clap::log << cL::warning << cL::major << "clap"_tag << "ui"_tag << "zone"_tag
					<< "Fail to create a window for a new free zone: " << title
					<< " (" << width << ", " << height << ")";
		},
		[this](when_owned const &) {
			clap::log << cL::error << cL::major << "clap"_tag << "ui"_tag << "zone"_tag
				<< "Fail to process " << this->name() << ". "
					"It has suddenly changed type from free to owned.";
		}
	}, state);
}
clap::ui::zone::~zone() {
	if (auto &device = vulkan::device(); device)
		device.waitIdle();
}
std::string clap::ui::zone::name() const {
	// TODO: Optimize so that strings are not built from the ground up each time.
	return std::visit(utility::overload{
		[this](when_free const &state) -> std::string { 
			return get_identifier().data() + (" titled '" + state.title + "'");
		},
		[this](when_owned const &state) -> std::string {
			return get_identifier().data() + (" owned by " + state.owner->name());
		}
	}, state);
}

clap::ui::zone::zone(std::shared_ptr<compound::interface> owner)
	: get_identifier([] { return "a zone"; })
	, state(when_owned { 
		.owner = owner,
		.root = owner ? 
			std::visit(utility::overload {
				[&owner](when_free const &) -> zone * {
					return &*owner;
				},
				[](when_owned const &state) -> zone * {
					return state.root;
				}
			}, owner->state) : nullptr}) {	
	bool should_continue = std::visit(utility::overload {
		[this](when_free const &) -> bool {
			clap::log << cL::error << cL::major << "clap"_tag << "ui"_tag << "zone"_tag
				<< "Fail to process " << name() << ". "
					"It has suddenly changed type from owned to free.";
			return false;
		},
		[this](when_owned const &state) -> bool {
			if (!state.owner)
				clap::log << cL::error << cL::major << "clap"_tag << "ui"_tag << "zone"_tag
					<< "Fail to process " << name() << ". "
						"It is not free but the owner is 'nullptr'.";
			return static_cast<bool>(state.owner);
		}
	}, owner->state);
	if (!should_continue) return;

	assert(false); // TODO: Implement embeded zones.
}

std::optional<clap::ui::vulkan::window_view> clap::ui::zone::window() {
	return std::visit(utility::overload{
		[this](when_free &state) -> std::optional<vulkan::window_view> {
			if (state.window)
				return *state.window;
			else
				return std::nullopt;
		},
		[this](when_owned &state) -> std::optional<vulkan::window_view> {
			if (state.root)
				return state.root->window();
			else if (state.owner)
				return state.owner->window();
			else
				return std::nullopt;
		}
	}, state);
}
std::optional<clap::ui::vulkan::window_view const> clap::ui::zone::window() const {
	return std::visit(utility::overload{
		[this](when_free const &state) -> std::optional<vulkan::window_view const> {
			if (state.window)
				return *state.window;
			else
				return std::nullopt;
		},
		[this](when_owned const &state) -> std::optional<vulkan::window_view const> {
			if (state.root)
				return state.root->window();
			else if (state.owner)
				return state.owner->window();
			else
				return std::nullopt;
		}
	}, state);
}

void clap::ui::zone::do_add() {
	bool should_continue = std::visit(utility::overload{
		[this](when_free const &) -> bool { return true; },
		[this](when_owned const &) -> bool {
				clap::log << cL::error << cL::major << "clap"_tag << "ui"_tag << "zone"_tag
					<< "Fail to add a window for " << name() << ". It is not free.";
				return false;
		} 
	}, state);
	if (!should_continue) return;
	if (auto window = this->window(); window) {
		do_initialize();
		while (!window->vkfw.shouldClose())
			do_update();
	} else
		clap::log << cL::warning << cL::major << "clap"_tag << "ui"_tag << "zone"_tag
			<< "Fail to launch zone loop for " << name() << ". "
				"'static_cast<bool>({zone}->window())' is 'false'.";
}
void clap::ui::zone::do_remove() {
	std::visit(utility::overload{
		[this](when_free &state) { state.window = nullptr; },
		[this](when_owned const &) {
			clap::log << cL::error << cL::major << "clap"_tag << "ui"_tag << "zone"_tag
				<< "Fail to remove a window from " << name() << ". It is not free.";
		}
	}, state);
}
void clap::ui::zone::do_initialize() {
	if (on_initialize)
		pipeline = std::make_shared<vk::UniquePipeline>(std::move(on_initialize()));
	else 
		std::visit(utility::overload{
			[this](when_free &) { /* Do nothing */ },
			[this](when_owned const &state) {
				pipeline = state.owner->pipeline;
			}
		}, state);
	if (!pipeline)
		clap::log << cL::warning << cL::major << name() << " was initialized without a valid pipeline object.";
	else if (auto &device = clap::ui::vulkan::device(); device)
		if (auto window = this->window(); window) {
			synchronization.resize(window->framebuffers.size());
			for (auto &sync : synchronization) {
				sync.framebuffer_ready_semaphore = device.createSemaphoreUnique({});
				sync.queue_submitted_semaphore = device.createSemaphoreUnique({});
				sync.queue_submitted_fence = device.createFenceUnique(vk::FenceCreateInfo{
					.flags = vk::FenceCreateFlagBits::eSignaled
				});
			}
			do_render();
		}
}
inline void clap::ui::zone::do_render() {
	if (on_render && pipeline && *pipeline)
		if (auto window = this->window(); window) {
			vk::CommandBufferAllocateInfo command_buffer_info = {
				.commandPool = clap::ui::vulkan::command_pool(),
				.level = vk::CommandBufferLevel::ePrimary,
				.commandBufferCount = static_cast<uint32_t>(window->framebuffers.size())
			};
			if (auto device = vulkan::device(); device)
				command_buffers = device.allocateCommandBuffersUnique(command_buffer_info);
			else
				return;

			vk::Rect2D render_area = {
				.offset = { static_cast<int32_t>(size.x()), static_cast<int32_t>(size.y()) },
				.extent = { static_cast<uint32_t>(size.w()), static_cast<uint32_t>(size.h()) }
			};
			vk::ClearValue clear_color_value(clear_color);
			vk::RenderPassBeginInfo render_pass_begin_info = {
				.renderPass = window->render_pass,
				//.framebuffer = *window->framebuffers[counter++],
				.renderArea = render_area,
				.clearValueCount = 1,
				.pClearValues = &clear_color_value
			};
			vk::Viewport viewport = {
				.x = static_cast<float>(size.x()),
				.y = static_cast<float>(size.y()),
				.width = static_cast<float>(size.w()),
				.height = static_cast<float>(size.h()),
				.minDepth = 0.f,
				.maxDepth = 1.f
			};

			for (size_t counter = 0u; auto &command_buffer : command_buffers) {
				command_buffer->begin(vk::CommandBufferBeginInfo{});
				render_pass_begin_info.framebuffer = *window->framebuffers[counter++];
				command_buffer->beginRenderPass(render_pass_begin_info, vk::SubpassContents::eInline);
				command_buffer->bindPipeline(vk::PipelineBindPoint::eGraphics, **pipeline);
				command_buffer->setViewport(0, std::array{ viewport });
				command_buffer->setScissor(0, std::array{ render_area });

				on_render(*command_buffer);

				command_buffer->endRenderPass();
				command_buffer->end();
			}
		}
}
inline void clap::ui::zone::do_update() {
	auto current_frame = std::chrono::high_resolution_clock::now();
	static auto last_frame = current_frame;
	auto time_step = std::chrono::duration_cast<clap::utility::timestep>(current_frame - last_frame);

	if (on_update && on_update(time_step) && !command_buffers.empty())
		if (auto &device = clap::ui::vulkan::device(); device)
			if (auto window = this->window(); window)
				if (auto &queue = clap::ui::vulkan::queue(); queue) {
					auto temporary_semaphore = device.createSemaphoreUnique({});
					uint32_t framebuffer_index = ~0u;
					auto acquire_result = static_cast<vk::Result>(vkAcquireNextImageKHR(
						device, window->swapchain, std::numeric_limits<uint64_t>::max(),
						*temporary_semaphore, nullptr, &framebuffer_index
					));
					if (acquire_result == vk::Result::eSuccess 
					 || acquire_result == vk::Result::eSuboptimalKHR) {
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
						auto present_result = static_cast<vk::Result>(vkQueuePresentKHR(
							queue, &present_info.operator const VkPresentInfoKHR & ()
						));
						if (present_result == vk::Result::eErrorOutOfDateKHR)
							do_resize();
					} else if (acquire_result == vk::Result::eErrorOutOfDateKHR)
						do_resize();
				}
}

inline void clap::ui::zone::do_resize() {
	std::visit(utility::overload {
		[this](when_free &state) {
			if (auto &device = clap::ui::vulkan::device(); device)
				device.waitIdle();
			command_buffers.clear();
			state.window->do_resize();
			size.current_w = state.window->extent.width;
			size.current_h = state.window->extent.height;
			do_render();
		},
		[this](when_owned const &state) {
			state.owner->do_resize();
		}
	}, state);
}

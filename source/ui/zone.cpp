#include "precompiled/ui.hpp"

#include "resource/resource.hpp"

clap::ui::zone::zone(std::string_view title, size_t width, size_t height)
	: get_identifier([] { return "a free zone"; })
	, state(when_free{ std::string(title), std::make_shared<detail::window>(width, height, title) })
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

std::optional<clap::ui::detail::window_view> clap::ui::zone::window() {
	return std::visit(utility::overload{
		[this](when_free &state) -> std::optional<detail::window_view> {
			if (state.window)
				return *state.window;
			else
				return std::nullopt;
		},
		[this](when_owned &state) -> std::optional<detail::window_view> {
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

		auto last_frame = std::chrono::high_resolution_clock::now();
		while (!window->vkfw.shouldClose()) {
			using namespace std::literals;
			constexpr static auto frame_limit = 1'000'000us / 60;
			constexpr static float numeric_frame_limit =
				std::chrono::duration_cast<std::chrono::duration<float>>(frame_limit).count();

			auto start_time = std::chrono::high_resolution_clock::now();
			auto timestep =
				std::chrono::duration_cast<clap::utility::timestep>(start_time - last_frame);
			last_frame = start_time;

			if (do_update(timestep))
				window->vkfw.swapBuffers();

			auto end_time = std::chrono::high_resolution_clock::now();
			auto frame_duration = end_time - start_time;
			float numeric_frame_duration =
				std::chrono::duration_cast<std::chrono::duration<float>>(frame_duration).count();

			clap::log << cL::message << cL::negligible << "clap"_tag << "ui"_tag << "zone"_tag
				<< "Draw a frame." << cL::extra
				<< "It took " << numeric_frame_duration << "/" << numeric_frame_limit
				<< "s (" << (100.f * numeric_frame_duration / numeric_frame_limit) << "%)";

			std::this_thread::sleep_until(start_time + frame_limit);
		}
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
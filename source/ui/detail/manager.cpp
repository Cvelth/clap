#include "precompiled/ui.hpp"

#include <unordered_map>

std::function<void(clap::ui::zone *, clap::ui::detail::window_handle)> clap::ui::detail::manager::zone_loop;
std::shared_mutex clap::ui::detail::manager::mutex;

struct owning_window_handle {
	vkfw::UniqueWindow window;
	vk::UniqueSurfaceKHR surface;

	operator bool() const { return window && surface; }
	operator clap::ui::detail::window_handle() {
		return clap::ui::detail::window_handle{ *window, *surface };
	}
	bool check_swapchain() {
		auto &instance = clap::ui::vulkan::instance();
		
		
		return instance;
	}
};

static std::unordered_map<clap::ui::zone *, owning_window_handle> zone_registry;
static std::vector<clap::ui::zone *> added_queue, removed_queue;
static std::mutex local_mutex;

std::optional<clap::ui::detail::window_handle> clap::ui::detail::manager::get(ui::zone &zone_ref) {
	std::shared_lock guard(manager::mutex);
	if (auto iterator = zone_registry.find(&zone_ref); iterator != zone_registry.end())
		if (iterator->second)
			return iterator->second;
		else {
			clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
				<< "Fail to obtain a 'vkfw::Window' object for " << zone_ref.name()
				<< cL::extra << "'vkfw::WindowUnique->operator bool()' has returned 'false'.";
			manager::remove(zone_ref);
		}
	else
		clap::log << cL::warning << cL::major << "clap"_tag << "ui"_tag << "manager"_tag
			<< "Fail to obtain a 'vkfw::Window' object for " << zone_ref.name()
			<< cL::extra << "It's possible the zone wasn't registered or was already destroyed.";
	return std::nullopt;
}

std::optional<clap::ui::detail::window_handle> clap::ui::detail::manager::add(ui::zone &zone_ref) {
	return std::visit(
		utility::overload{
			[&zone_ref](zone::when_free &state) -> std::optional<window_handle> {
				std::unique_lock guard(manager::mutex);
				auto [iterator, success] = zone_registry.try_emplace(&zone_ref);
				if (success) {
					if (vulkan::vkfw())
						iterator->second.window = vkfw::createWindowUnique(
							zone_ref.width(), zone_ref.height(), (char const *) state.name.data(),
							vkfw::WindowHints{}, nullptr, nullptr, false
						);
					if (auto &instance = vulkan::instance(); instance)
						iterator->second.surface = vkfw::createWindowSurfaceUnique(
							instance, *iterator->second.window
						);
					if (iterator->second && iterator->second.check_swapchain()) {
						std::lock_guard local_guard(local_mutex);
						added_queue.emplace_back(&zone_ref);
						vkfw::postEmptyEvent();
						return iterator->second;
					} else {
						clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
							<< "Fail to create a 'vkfw::Window' object for " << zone_ref.name()
							<< cL::extra << "'vkfw::WindowUnique->operator bool()' has returned 'false'.";
						manager::remove(zone_ref);
					}
				} else
					clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
						<< "Fail to add " << zone_ref.name() << " to the registry. "
							"'std::unordered_map::emplace()' has returned false.";
				return std::nullopt;
			},
			[&zone_ref](zone::when_owned &) -> std::optional<window_handle> {
				clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
					<< "Fail to add " << zone_ref.name() << " to the registry. "
						"Only free zones can be added.";
				return std::nullopt;
			}
		}, zone_ref.state
	);
}

void clap::ui::detail::manager::remove(ui::zone &zone_ref) {
	std::lock_guard guard(local_mutex);
	removed_queue.emplace_back(&zone_ref);
	vkfw::postEmptyEvent();
}

bool clap::ui::detail::manager::empty() {
	return zone_registry.empty();
}

void clap::ui::detail::manager::update() {
	std::lock_guard local_guard(local_mutex);
	while (!added_queue.empty()) {
		std::unique_lock guard(manager::mutex);
		if (auto iterator = zone_registry.find(added_queue.back()); iterator != zone_registry.end())
			if (manager::zone_loop) {
				if (iterator->second) {
					manager::zone_loop(iterator->first, iterator->second);
					clap::log << cL::message << cL::minor << "clap"_tag << "ui"_tag << "manager"_tag
						<< "Add " << added_queue.back()->name() << " to the registry.";
				} else
					clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
						<< "Fail to update a 'vkfw::Window' object."
						<< cL::extra << "'vkfw::WindowUnique->operator bool()' has returned 'false'.";
			} else
				clap::log << cL::warning << cL::major << "clap"_tag << "ui"_tag << "manager"_tag
					<< "Add a zone without running 'manager::zone_loop(...)'." 
					<< cL::extra << "The function is empty.";
		else
			clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
				<< "Fail to add " << added_queue.back()->name() << " to the registry.";
		added_queue.pop_back();
	}
	while (!removed_queue.empty()) {
		std::unique_lock guard(manager::mutex);
		if (auto iterator = zone_registry.find(removed_queue.back()); iterator != zone_registry.end()) {
			zone_registry.erase(iterator);
			clap::log << cL::message << cL::minor << "clap"_tag << "ui"_tag << "manager"_tag
				<< "Remove " << removed_queue.back()->name() << " from the registry.";
		} else
			clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
				<< "Fail to remove " << removed_queue.back()->name() << " from the registry. "
					"It was either never added or already removed.";
		removed_queue.pop_back();
	}
}

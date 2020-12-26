#include "precompiled/ui.hpp"

#include <unordered_map>

std::function<void(clap::ui::zone *, vkfw::Window *)> clap::ui::detail::manager::zone_loop;
std::shared_mutex clap::ui::detail::manager::mutex;

static std::unordered_map<clap::ui::zone *, vkfw::UniqueWindow> zone_registry;
static std::vector<clap::ui::zone *> added_queue, removed_queue;
static std::mutex local_mutex;

vkfw::Window *clap::ui::detail::manager::get(ui::zone &zone_ref) {
	std::shared_lock guard(manager::mutex);
	if (auto iterator = zone_registry.find(&zone_ref); iterator != zone_registry.end())
		if (iterator->second)
			return &iterator->second.get();
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
	return nullptr;
}

vkfw::Window *clap::ui::detail::manager::add(ui::zone &zone_ref) {
	[[maybe_unused]] auto &instance = vk::instance();
	return std::visit(
		utility::overload{
			[&zone_ref](zone::when_free &state) -> vkfw::Window * {
				std::unique_lock guard(manager::mutex);
				auto [iterator, success] = zone_registry.try_emplace(&zone_ref);
				if (success) {
					iterator->second = vkfw::createWindowUnique(
						zone_ref.width(), zone_ref.height(), (char const *) state.name.data(),
						vkfw::WindowHints{}, nullptr, nullptr, false
					);
					if (iterator->second) {
						std::lock_guard local_guard(local_mutex);
						added_queue.emplace_back(&zone_ref);
						return &iterator->second.get();
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
				return nullptr;
			},
			[&zone_ref](zone::when_owned &) -> vkfw::Window * {
				clap::log << cL::warning << cL::critical << "clap"_tag << "ui"_tag << "manager"_tag
					<< "Fail to add " << zone_ref.name() << " to the registry. "
						"Only free zones can be added.";
				return nullptr;
			}
		}, zone_ref.state
	);
}

void clap::ui::detail::manager::remove(ui::zone &zone_ref) {
	std::lock_guard guard(local_mutex);
	removed_queue.emplace_back(&zone_ref);
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
					manager::zone_loop(iterator->first, &iterator->second.get());
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

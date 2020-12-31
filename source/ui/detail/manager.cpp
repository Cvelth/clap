#include "precompiled/ui.hpp"

#include <mutex>

#include "ui/detail/manager.hpp"
#include "ui/zone.hpp"

static std::vector<clap::ui::zone *> added_queue, removed_queue;
static std::mutex adding_mutex, removing_mutex;
static size_t zone_counter = 0;
static std::vector<std::thread> threads;

void clap::ui::detail::manager::add(ui::zone &zone_ref) {
	std::lock_guard local_guard(adding_mutex);
	added_queue.emplace_back(&zone_ref);
	++zone_counter;
	vkfw::postEmptyEvent();
}
void clap::ui::detail::manager::remove(ui::zone &zone_ref) {
	std::lock_guard guard(removing_mutex);
	removed_queue.emplace_back(&zone_ref);
	--zone_counter;
	vkfw::postEmptyEvent();
}
bool clap::ui::detail::manager::empty() {
	return !zone_counter;
}
void clap::ui::detail::manager::wait() {
	for (auto &thread : threads)
		thread.join();
	threads.clear();
}

inline std::optional<clap::ui::zone *> next_addition() {
	std::lock_guard local_guard(adding_mutex);
	if (added_queue.empty())
		return std::nullopt;
	else {
		clap::ui::zone *output = added_queue.back();
		added_queue.pop_back();
		return output;
	}
}
inline std::optional<clap::ui::zone *> next_removal() {
	std::lock_guard local_guard(removing_mutex);
	if (removed_queue.empty())
		return std::nullopt;
	else {
		clap::ui::zone *output = removed_queue.back();
		removed_queue.pop_back();
		return output;
	}
}
void clap::ui::detail::manager::update_additions() {
	for (decltype(next_addition()) added; added = next_addition();) {
		if (*added)
			threads.emplace_back (
				[added_zone_ptr = std::move(*added)] {
					added_zone_ptr->do_add();
					clap::ui::detail::manager::remove(*added_zone_ptr);
				});
		else
			clap::log << cL::warning << cL::major << "clap"_tag << "ui"_tag << "manager"_tag << "update"_tag
				<< "Fail to start a zone loop. The zone is 'nullptr'.";
	}
}
void clap::ui::detail::manager::update_removals() {
	for (decltype(next_removal()) removed; removed = next_removal();) {
		if (*removed)
			(*removed)->do_remove();
		else
			clap::log << cL::warning << cL::major << "clap"_tag << "ui"_tag << "manager"_tag << "update"_tag
				<< "Fail to start a zone loop. The zone is 'nullptr'.";
	}
}
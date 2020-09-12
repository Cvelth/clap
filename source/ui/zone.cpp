#include "ui/zone.hpp"

#include "ui/detail/state.hpp"

clap::ui::zone::zone(std::u8string name, size_t width, size_t height) noexcept 
	: name(name), owner(nullptr), size(width, height) {
	detail::state::add(this);
}
clap::ui::zone::zone(clap::ui::compound::interface *owner) noexcept
	: owner(owner) {}

clap::ui::detail::size_constraits::size_constraits() noexcept : size_constraits(0u, 0u) {}

clap::ui::detail::size_constraits::size_constraits(size_t width, size_t height) noexcept :
	width(resize_preference::prefer_maximum), height(resize_preference::prefer_maximum),
	maximum_w(std::numeric_limits<size_t>::max()), maximum_h(std::numeric_limits<size_t>::max()),
	minimum_w(std::numeric_limits<size_t>::min()), minimum_h(std::numeric_limits<size_t>::min()),
	current_x(0u), current_y(0u), current_w(width), current_h(height) {}

#include "precompiled/ui.hpp"

#include "resource/resource.hpp"

clap::ui::zone::zone(std::string_view name, size_t width, size_t height)
	: get_identifier([] { return "a free zone"; })
	, state(when_free{std::string(name)})
	, size{width, height} {

	resource::identify();

	clap::log << cL::message << cL::minor
		<< "Create new free zone: " << name << " (" << width << ", " << height << ")";
	detail::manager::add(*this);
}
std::string clap::ui::zone::name() const {
	return std::visit(
		utility::overload{
			[this](when_free const &state) -> std::string { 
				return get_identifier().data() + (" named '" + state.name + "'");
			},
			[this](when_owned const &state) -> std::string{
				return get_identifier().data() + (" owned by " + state.owner->name());
			}
		}, state
	);
}
clap::ui::zone::zone(compound::interface &owner)
	: get_identifier([] { return "an embeded zone"; })
	, state(when_owned{ &owner }) {

	assert(false);
}
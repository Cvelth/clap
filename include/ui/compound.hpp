#pragma once
#include "ui/zone.hpp"

#include <list>
#include <memory>
#include <set>
#include <vector>

namespace clap::ui::compound {
	class interface : public zone {
	public:
		using zone::zone;
	};

	namespace detail {
		template <typename size_preference_t>
		struct stored_item {
			std::shared_ptr<zone> item;
			size_preference_t size;

			template <typename ...Ts>
			stored_item(compound::interface *ptr, Ts const &...ts)
				: item(std::make_shared<zone>(ptr, std::forward<Ts>(ts)...)),
				size() {}
		};
	}

	template <typename size_preference_t>
	class list : public interface, protected std::list<detail::stored_item<size_preference_t>> {
		using underlying_t = std::list<detail::stored_item<size_preference_t>>;
	public:
		using interface::interface;

		template <typename type, typename ...Ts>
		size_preference_t& emplace_back(Ts const &...ts) {
			underlying_t::emplace_back(static_cast<interface *>(this), std::forward<Ts>(ts)...);
			return underlying_t::back().size;
		}
	};

	template <typename size_preference_t>
	class set : public interface, protected std::set<detail::stored_item<size_preference_t>> {
	public:
		using interface::interface;
	};

	template <typename size_preference_t>
	class vector : public interface, protected std::vector<detail::stored_item<size_preference_t>> {
	public:
		using interface::interface;
	};
}
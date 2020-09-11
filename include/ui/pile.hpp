#pragma once
#include "ui/compound.hpp"

#include <variant>

namespace clap::ui {
	class pile;
	namespace detail {
		struct absolute_pile_constraints {
			size_t x, y, w, h;
		};
		struct relative_pile_constraints {
			float x, y, w, h;
		};
		void verify_relative_constraints(float x, float y, float w, float h);

		class pile_constraints {
			friend pile;
		public:
			void absolute(size_t x, size_t y, size_t w, size_t h) noexcept {
				value = absolute_pile_constraints{ x, y, w, h };
			}
			void relative(float x, float y, float w, float h) noexcept {
				verify_relative_constraints(x, y, w, h);
				value = relative_pile_constraints{ x, y, w, h };
			}
		protected:
			std::variant<
				absolute_pile_constraints,
				relative_pile_constraints
			> value;
		};
	}

	class pile : public compound::list<detail::pile_constraints> {
	public:
		using compound::list<detail::pile_constraints>::list;
	};
}
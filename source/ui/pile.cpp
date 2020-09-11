#include "ui/pile.hpp"

#include "essential/log.hpp"

void verify_relative_constraint(float v) {
	if (v <= 0.f || v >= 1.f) {
		clap::log::warning::critical << "Relative constraint in unacceptable. Value must be in range (0.f, 1.f).";
		clap::log::info::major << "Value: " << v << ".";
	}
}

void clap::ui::detail::verify_relative_constraints(float x, float y, float w, float h) {
	verify_relative_constraint(x);
	verify_relative_constraint(y);
	verify_relative_constraint(w);
	verify_relative_constraint(h);
}

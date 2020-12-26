#pragma once
#include <chrono>

namespace clap::utility {
	template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
	template<class... Ts> overload(Ts...)->overload<Ts...>;

	using timestep = std::chrono::duration<long double>;
}
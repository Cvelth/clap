#pragma once
#include <chrono>
#include <concepts>
#include <iterator>

namespace clap::utility {
	template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
	template<class... Ts> overload(Ts...)->overload<Ts...>;

	using timestep = std::chrono::duration<long double>;

	template <typename T>
	concept iteratable = requires(T a) {
		{ a.begin() } -> std::input_iterator<>;
		{ a.end() } -> std::input_iterator<>;
		{ a.size() } -> std::convertible_to<size_t>;
		{ a.empty() } -> std::convertible_to<bool>;
	};
}
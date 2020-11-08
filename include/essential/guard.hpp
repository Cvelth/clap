#pragma once
#include <mutex>
#include <shared_mutex>

namespace clap::essential {
	namespace detail {
		template <typename value_t, typename Enable = void>
		class guard_state;

		template <typename value_t>
		class guard_state<value_t, typename std::enable_if<std::is_same<value_t, void>::value>::type> {};
		template <typename value_t>
		class guard_state<value_t, typename std::enable_if<!std::is_same<value_t, void>::value>::type> {
		protected:
			value_t value;
		};
	}

	template <typename lock_lambda_t, typename unlock_lambda_t, typename state_t = decltype(std::declval<lock_lambda_t>()())>
	class simple_guard : protected detail::guard_state<state_t> {
	public:
		simple_guard(lock_lambda_t lock_lambda, unlock_lambda_t unlock_lambda) : unlock_lambda(unlock_lambda) {
			if constexpr (std::is_same<state_t, void>::value)
				lock_lambda();
			else
				detail::guard_state<state_t>::value = lock_lambda();
		}

		~simple_guard() {
			if constexpr (std::is_same<state_t, void>::value || !std::is_invocable<unlock_lambda_t, state_t>::value)
				unlock_lambda();
			else
				unlock_lambda(detail::guard_state<state_t>::value);
		}

		simple_guard(simple_guard const &) = delete;
		simple_guard(simple_guard &&) = delete;
		simple_guard &operator=(simple_guard const &) = delete;
		simple_guard &operator=(simple_guard &&) = delete;
	private:
		unlock_lambda_t unlock_lambda;
	};

	template <typename mutex_t, typename lock_lambda_t, typename unlock_lambda_t>
	class guard : public simple_guard<lock_lambda_t, unlock_lambda_t>, protected std::lock_guard<mutex_t> {
	public:
		guard(mutex_t &mutex, lock_lambda_t lock_lambda, unlock_lambda_t unlock_lambda)
			: simple_guard<lock_lambda_t, unlock_lambda_t>(std::forward<lock_lambda_t>(lock_lambda),
														   std::forward<unlock_lambda_t>(unlock_lambda)),
			std::lock_guard<mutex_t>(mutex) {}
		~guard() {}
	};

	template <typename mutex_t, typename lock_lambda_t, typename unlock_lambda_t>
	class shared_guard : public simple_guard<lock_lambda_t, unlock_lambda_t>, protected std::shared_lock<mutex_t> {
	public:
		shared_guard(mutex_t &mutex, lock_lambda_t lock_lambda, unlock_lambda_t unlock_lambda)
			: simple_guard<lock_lambda_t, unlock_lambda_t>(std::forward<lock_lambda_t>(lock_lambda),
														   std::forward<unlock_lambda_t>(unlock_lambda)),
			std::shared_lock<mutex_t>(mutex) {}
		~shared_guard() {}
	};
}
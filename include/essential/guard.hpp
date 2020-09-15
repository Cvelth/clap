#pragma once
#include <mutex>

namespace clap::essential {
	template <typename lock_lambda_t, typename unlock_lambda_t>
	class simple_guard {
	public:
		simple_guard(lock_lambda_t lock_lambda, unlock_lambda_t unlock_lambda) : unlock_lambda(unlock_lambda) {
			lock_lambda();
		}
		~simple_guard() {
			unlock_lambda();
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
}
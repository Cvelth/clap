#pragma once
#include <mutex>

namespace clap::essential {
	template <typename mutex_t, typename lock_lambda_t, typename unlock_lambda_t>
	class guard : std::lock_guard<mutex_t> {
	public:
		guard(mutex_t &mutex,
			  lock_lambda_t lock_lambda,
			  unlock_lambda_t unlock_lambda) :
			std::lock_guard<mutex_t>(mutex), unlock_lambda(unlock_lambda) {
			lock_lambda();
		}
		~guard() {
			unlock_lambda();
		}

		guard(guard const &) = delete;
		guard(guard &&) = delete;
		guard &operator=(guard const &) = delete;
		guard &operator=(guard &&) = delete;
	private:
		unlock_lambda_t unlock_lambda;
	};
}
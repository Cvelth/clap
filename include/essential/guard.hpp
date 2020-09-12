#pragma once

namespace clap::essential {
	template <typename contained_t, typename lock_lambda_t, typename unlock_lambda_t>
	class guard {
	public:
		guard(contained_t const &contained,
			  lock_lambda_t const &lock_lambda,
			  unlock_lambda_t const &unlock_lambda) :
			contained(contained), lock_lambda(lock_lambda), unlock_lambda(unlock_lambda) {
			lock_lambda(contained);
		}
		~guard() {
			unlock_lambda(contained);
		}

		guard(guard const &) = delete;
		guard(guard &&) = delete;
		guard& operator=(guard const &) = delete;
		guard& operator=(guard &&) = delete;
	private:
		contained_t const &contained;
		lock_lambda_t const &lock_lambda;
		unlock_lambda_t const &unlock_lambda;
	};
}
#pragma once
#include <forward_list>
#include <shared_mutex>
#include <utility>

namespace clap::essential {
	template <typename element_t>
	class stack : protected std::forward_list<element_t> {
	public:
		using iterator = typename std::forward_list<element_t>::iterator;
		using std::forward_list<element_t>::forward_list;

		[[nodiscard]] iterator begin() { return std::forward_list<element_t>::begin(); }
		[[nodiscard]] iterator begin() const { return std::forward_list<element_t>::begin(); }
		[[nodiscard]] iterator cbegin() const { return std::forward_list<element_t>::cbegin(); }

		[[nodiscard]] iterator end() { return std::forward_list<element_t>::end(); }
		[[nodiscard]] iterator end() const { return std::forward_list<element_t>::end(); }
		[[nodiscard]] iterator cend() const { return std::forward_list<element_t>::cend(); }

		iterator push(element_t const &element) {
			std::forward_list<element_t>::push_front(element);
			return std::forward_list<element_t>::before_begin();
		}
		iterator push(element_t &&element) {
			std::forward_list<element_t>::push_front(std::move(element));
			return std::forward_list<element_t>::before_begin();
		}

		[[nodiscard]] element_t &peek() {
			return std::forward_list<element_t>::front();
		}
		[[nodiscard]] element_t const &peek() const {
			return std::forward_list<element_t>::front();
		}

		[[nodiscard]] element_t pop() {
			auto ret = peek();
			std::forward_list<element_t>::pop_front();
			return ret;
		}

		[[nodiscard]] bool is_front(iterator iterator) const {
			return std::forward_list<element_t>::begin() == ++iterator;
		}
		[[nodiscard]] bool empty() const { return std::forward_list<element_t>::empty(); }

		void erase(iterator const &iterator) { std::forward_list<element_t>::erase_after(iterator); }

	private:
	};

	template <typename element_t>
	class protected_stack : protected stack<element_t> {
	private:
		using parent = stack<element_t>;
	public:
		using iterator = typename parent::iterator;
		using stack<element_t>::stack;

		iterator push(element_t const &element) {
			std::scoped_lock guard(mutex);
			return parent::push(std::forward(element));
		}
		iterator push(element_t &&element) {
			std::scoped_lock guard(mutex);
			return parent::push(std::move(element));
		}

		[[nodiscard]] element_t &peek() {
			std::shared_lock guard(mutex);
			return std::forward(parent::peek());
		}
		[[nodiscard]] element_t const &peek() const {
			std::shared_lock guard(mutex);
			return std::forward(parent::peek());
		}

		[[nodiscard]] element_t pop() {
			std::scoped_lock guard(mutex);
			return parent::pop();
		}

		[[nodiscard]] bool is_front(iterator iterator) const {
			std::shared_lock guard(mutex);
			return parent::is_front(std::forward(iterator));
		}
		[[nodiscard]] bool empty() const {
			std::shared_lock guard(mutex);
			return parent::empty();
		}

		void erase(iterator const &iterator) { 
			parent::erase_after(std::forward(iterator));
		}

	private:
		mutable std::shared_mutex mutex;
	};
}
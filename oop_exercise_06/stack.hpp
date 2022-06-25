#pragma once

#include <memory>
#include <stdexcept>

template <class T, class Allocator = std::allocator<T>>
class Stack {
private:
	using allocator_type = Allocator;
	//using allocator_type = typename Allocator::template rebind<T>::other;

	static allocator_type get_allocator() {
		static allocator_type allocator;
		return allocator;
	}

	struct stack_node {
		T value = {};
		std::unique_ptr<stack_node> next = nullptr;

		stack_node() = default;

		stack_node(T val)
			: value(std::move(val)) {
		}

		void* operator new(size_t size) {
			return get_allocator().allocate(sizeof(T));
		}

		void operator delete(void* p) {
			get_allocator().destroy((T*)p);
			get_allocator().deallocate((T*)p, sizeof(T));
		}
	};

public:
	Stack() = default;
	Stack(size_t size) : size_(size) {
		if (size == 0) {
			return;
		}

		head_ = std::make_unique<stack_node>();;
		stack_node* node = head_.get();
		while (size-- > 1) {
			node->next = std::make_unique<stack_node>();
			node = node->next.get();
		}
	}

	const T& operator[](std::size_t index) const {
		if (index >= size_) {
			throw std::out_of_range("index is out of range");
		}
		stack_node* node = head_.get();
		while (index-- > 0) {
			node = node->next.get();
		}
		return node->value;
	}

	T& operator[](std::size_t index) {
		return const_cast<T&>(const_cast<const Stack*>(this)->operator[](index));
	}

	std::size_t size() const {
		return size_;
	}

	struct forward_iterator {
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = int;
		using pointer = T*;
		using reference = T&;

		stack_node* ptr;

		forward_iterator(stack_node* ptr)
			:ptr(ptr) {
		}

		T& operator*() const {
			return ptr->value;
		}

		T* operator->() const {
			return &ptr->value;
		}

		forward_iterator& operator++() {
			if (!ptr) {
				throw std::runtime_error("trying to increment null pointer");
			}
			if (ptr->next) {
				ptr = ptr->next.get();
			}
			else {
				ptr = nullptr;
			}
			return *this;
		}

		forward_iterator operator++(int) {
			auto res = *this;
			operator++();
			return res;
		}

		bool operator==(const forward_iterator& rhs) const {
			return ptr == rhs.ptr;
		}

		bool operator!=(const forward_iterator& rhs) const {
			return ptr != rhs.ptr;
		}

		operator bool() {
			return ptr;
		}
	};

	forward_iterator begin() const {
		return forward_iterator(head_.get());
	}

	forward_iterator end() const {
		return forward_iterator{ nullptr };
	}

	forward_iterator insert_before(forward_iterator it, T value) {
		if (it == begin()) {
			auto ptr = std::make_unique<stack_node>(value);
			ptr->next = std::move(head_);
			head_ = std::move(ptr);
			++size_;
			return begin();
		}
		forward_iterator prev = find_prev(it);
		auto ptr = std::make_unique<stack_node>(value);
		ptr->next = std::move(prev.ptr->next);
		prev.ptr->next = std::move(ptr);
		++size_;
		return ++prev;
	}

	forward_iterator erase(forward_iterator it) {
		if (size_ == 0) {
			throw std::runtime_error("stack empty before erase");
		}
		if (it == begin()) {
			head_ = std::move(head_->next);
			--size_;
			return begin();
		}
		forward_iterator prev = find_prev(it);
		if (it.ptr) {
			prev.ptr->next = std::move(it.ptr->next);
		}
		else {
			throw std::runtime_error("trying to remove element using invalid iterator");
		}
		forward_iterator res{ prev.ptr };
		--size_;
		return  ++res;
	}

	T pop() {
		if (size_ == 0) {
			throw std::runtime_error("stack empty before pop");
		}
		T res = top();
		erase(begin());
		return res;
	}

	void push(T value) {
		insert_before(begin(), std::move(value));
	}

	const T& top() const {
		return *begin();
	}

	T& top() {
		return *begin();
	}

private:
	size_t size_ = 0;
	std::unique_ptr<stack_node> head_ = nullptr;

	forward_iterator find_prev(forward_iterator target) const {
		if (target == begin()) {
			throw std::runtime_error("there is no element before the first");
		}
		forward_iterator prev = begin();
        
		for (forward_iterator it = ++begin(); it != end(); ++it) {
			if (it == target) {
				return prev;
			}
			prev = it;
		}
		if (target == end()) {
			return prev;
		}
		throw std::invalid_argument("it's not iterator if this stack");
	}
};
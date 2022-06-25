#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <deque>
#include <algorithm>

template <class T>
class custom_allocator {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using size_type = std::size_t;

	custom_allocator() noexcept {}

	template <class U>
	struct rebind
	{
		using other = custom_allocator<U>;
	};

	T* allocate(size_t n) {
		//std::cout << "allocate" << std::endl;
		data.push_back(new T);
		return *prev(data.end());
	}

	void deallocate(T* pointer, size_t) {
		//std::cout << "deallocate:" << pointer << std::endl;
		auto it = std::find_if(data.begin(), data.end(), [pointer](const auto ptr) {return ptr == pointer; });
		if (it != data.end()) {
			delete (*it);
			data.erase(it);
		}
	}

	template <typename U, typename... Args>
	void construct(U* p, Args &&... args) {
		new (p) U(std::forward<Args>(args)...);
	}

	void destroy(pointer p) {
		//std::cout << "destroy" << std::endl;
		p->~T();
	}

private:
	std::deque<T*> data;
};
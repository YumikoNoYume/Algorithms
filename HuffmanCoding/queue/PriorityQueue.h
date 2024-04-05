#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>

template<typename type>
class PriorityQueue {
public:

	class Exception : public std::exception {
	public:
		Exception() = default;
		const char* what() const override;
	};

	PriorityQueue() : compare_func(std::less<type>()) {}
	PriorityQueue(std::function<bool(const type&, const type&)> compare) : compare_func(compare) {}

	void Insert(type el);
	void ExtracTop();
	type GetTop() const;
	std::size_t GetSize() const;

private:

	void ShiftUp(std::size_t index);
	std::size_t ShiftDown(std::size_t index);

	std::vector<type> queue;
	std::function<bool(const type&, const type&)> compare_func;
};

#include "PriorityQueue.hpp"

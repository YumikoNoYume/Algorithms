#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <string>

template<typename type>
const char* PriorityQueue<type>::Exception::what() const {
	return "Empty container!";
}

template <typename type>
void PriorityQueue<type>::Insert(type el) {
	queue.push_back(el);
	ShiftUp(queue.size() - 1);
}

template <typename type>
void PriorityQueue<type>::ExtracTop() {
	if (queue.empty()) {
		throw Exception();
	}
	std::swap(queue[0], queue[queue.size() - 1]);
	queue.pop_back();
	std::size_t index = 1;
	ShiftDown(index);
}

template <typename type>
type PriorityQueue<type>::GetTop() const {
	if (queue.empty()) {
		throw Exception();
	}
	return queue[0];
}

template <typename type>
std::size_t PriorityQueue<type>::GetSize() const{
	return queue.size();
}

template <typename type>
void PriorityQueue<type>::ShiftUp(std::size_t index) {
	while (true) {
		if (index == 0) {
			return;
		}
		if (compare_func(queue[index], queue[(index - 1) / 2])) {
			std::swap(queue[index], queue[(index - 1) / 2]);
			index = (index - 1) / 2;
			continue;
		}
		break;
	}
}

template <typename type>
std::size_t PriorityQueue<type>::ShiftDown(std::size_t index)
{
	while (true) {
		if (index * 2 - 1 >= queue.size() && index * 2 >= queue.size()) {
			return index - 1;
		}
		if (index * 2 >= queue.size()) {
			if (compare_func(queue[index - 1], queue[index * 2 - 1])) {
				return index - 1;
			}
			std::swap(queue[index - 1], queue[index * 2 - 1]);
			index = index * 2;
			continue;
		}
		else {
			if (compare_func(queue[index - 1], queue[index * 2]) && compare_func(queue[index - 1], queue[index * 2 - 1])) {
				return index - 1;
			}
			if (compare_func(queue[index * 2 - 1], queue[index * 2])) {
				std::swap(queue[index - 1], queue[index * 2 - 1]);
				index = index * 2;
				continue;
			}
			std::swap(queue[index - 1], queue[index * 2]);
			index = index * 2 + 1;
			continue;
		}
	}
}

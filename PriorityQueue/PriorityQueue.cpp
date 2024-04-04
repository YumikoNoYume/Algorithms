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
		const char* what() const override {
			return "Empty container!";
		}
	};
	
	PriorityQueue() : compare_func(std::less<type>()) {}
	PriorityQueue(std::function<bool(const type&, const type&)> compare) : compare_func(compare) {}
	
	void Insert(type el) {
		queue.push_back(el);
		ShiftUp(queue.size() - 1);
	}

	void ExtracTop() {
		if (queue.empty()) {
			throw Exception();
		}
		std::swap(queue[0], queue[queue.size() - 1]);
		queue.pop_back();
		std::size_t index = 1;
		ShiftDown(index);
	}

	type GetTop() const {
		if (queue.empty()) {
			throw Exception();
		}
		return queue[0];
	}

	std::size_t GetSize() const {
		return queue.size();
	}

private:
	void ShiftUp(std::size_t index) {
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

	std::size_t ShiftDown(std::size_t index) {
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

	std::vector<type> queue;
	std::function<bool(const type&, const type&)> compare_func;
};

namespace test {
	bool IsIntBigger(const int& val1, const int& val2) {
		return val1 > val2;
	}

	struct Data {
		int priority;
		int data;
	};

	bool IsDataBigger(const Data& val1, const Data& val2) {
		return val1.priority > val2.priority;
	}

	bool IsPairBigger(const std::pair<int, int>& val1, const std::pair<int, int>& val2) {
		return val1 > val2;
	}

}

int main() {
	try { // int test
		PriorityQueue<int> int_queue{test::IsIntBigger};
		//std::cout << int_queue.GetTop(); // test 1 - GetTop() on empty queue
		//int_queue.ExtracTop(); // test 2 - ExtractTop() on empty queue
		std:: cout << int_queue.GetSize() << "\n"; // test 4 - GetSize() on empty queue
		for (int val = 1; val <= 200; val++) {
			int_queue.Insert(val);
		}
		while (int_queue.GetSize() > 0) {
			std::cout << int_queue.GetTop() << " ";
			int_queue.ExtracTop();
		}
		std::cout << "\n";
	}
	catch (PriorityQueue<int>::Exception e) {
		std::cout << "Error! " << e.what() << "\n";
	}
	try { // custom data test
		PriorityQueue<test::Data> int_queue{test::IsDataBigger};
		test::Data data;
		for (int val = 1; val <= 200; val++) {
			data.priority = val;
			data.data = val - 1;
			int_queue.Insert(data);
		}
		while (int_queue.GetSize() > 0) {
			std::cout << int_queue.GetTop().data << " ";
			int_queue.ExtracTop();
		}
		std::cout << "\n";
	}
	catch (PriorityQueue<test::Data>::Exception e) {
		std::cout << "Error! " << e.what() << "\n";
	}
	try { // std::pair test
		PriorityQueue<std::pair<int, int>> int_queue{test::IsPairBigger};
		std::pair<int, int> data;
		for (int val = 1; val <= 200; val++) {
			data.first = val;
			data.second = val - 1;
			int_queue.Insert(data);
		}
		while (int_queue.GetSize() > 0) {
			std::cout << int_queue.GetTop().second << " ";
			int_queue.ExtracTop();
		}
	}
	catch (PriorityQueue<std::pair<int, int>>::Exception e) {
		std::cout << "Error! " << e.what() << "\n";
	}
}

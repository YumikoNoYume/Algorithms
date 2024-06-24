#include <iostream>
#include <vector>
#include <utility>
#include <memory>
#include <stdexcept>

class SplayTree {
public:
	struct Node {
		Node* parent = nullptr;
		Node* left_child = nullptr;
		Node* right_child = nullptr;
		long long data = 0;
	};
	
	SplayTree() = default;
	SplayTree(long long data) {
		Insert(data);
	}
	SplayTree(Node* root) : root(root) {}
	SplayTree(const SplayTree& other) : SplayTree() {
		if (other.root) {
			root = new Node;
			Copy(other.root, root, nullptr);
		}
	}
	SplayTree(SplayTree&& other) noexcept : SplayTree() {
		Swap(*this, other);
	}
	~SplayTree() {
		if (root) {
			Clear(root);
		}
	}

	SplayTree& operator = (const SplayTree& other) {
		SplayTree new_tree{ other };
		Swap(*this, new_tree);
		return *this;
	}
	SplayTree& operator = (SplayTree&& other) noexcept {
		Swap(*this, other);
		return *this;
	}

	// Positive numbers only
	void Insert(long long data) {
		long long data_to_insert = ((data + curr_sum) % func_number + func_number) % func_number;
		Node* to_insert = new Node;
		Node* in_tree_node = nullptr;

		if (data_to_insert < 0) {
			throw std::logic_error("Error! Positive numbers insertion only");
		}

		to_insert->data = data_to_insert;

		if (!root) { 
			root = to_insert;
			return;
		} 
		
		in_tree_node = root;

		while (true) {
			if (data_to_insert == in_tree_node->data) return;

			if (data_to_insert < in_tree_node->data) {
				if (!in_tree_node->left_child) {
					in_tree_node->left_child = to_insert;
					to_insert->parent = in_tree_node;
					break;
				}
				in_tree_node = in_tree_node->left_child;
			}
			else {
				if (!in_tree_node->right_child) {
					in_tree_node->right_child = to_insert;
					to_insert->parent = in_tree_node;
					break;
				}
				in_tree_node = in_tree_node->right_child;
			}
		}

		Splay(to_insert);
	}

	void Delete(long long data) {
		long long data_to_delete = ((data + curr_sum) % func_number + func_number) % func_number;
		SplayTree temp;
		long long sum = curr_sum;

		Search(data);

		if (!root || data_to_delete != root->data) return;
		if (!root->left_child && !root->right_child) {
			root = nullptr;
			return;
		}

		std::pair<SplayTree, SplayTree> free_nodes = Split(*this, data);
		root = nullptr;

		if (free_nodes.second.root->data != data_to_delete) {
			throw std::runtime_error("Node deletion error!");
		}

		if (free_nodes.second.root->right_child && !free_nodes.second.root->left_child) {
			temp = free_nodes.second.root->right_child;
			temp.root->parent = nullptr;
			free_nodes.second = temp;
			temp.root = nullptr;
			*this = Merge(free_nodes.first, free_nodes.second);
			curr_sum = sum;
		}
		else if (free_nodes.second.root->left_child && !free_nodes.second.root->right_child) {
			temp = free_nodes.second.root->left_child;
			temp.root->parent = nullptr;
			free_nodes.second = temp;
			temp.root = nullptr;
			*this = Merge(free_nodes.first, free_nodes.second);
			curr_sum = sum;
		}
		else if (free_nodes.second.root->left_child && free_nodes.second.root->right_child) {
			free_nodes.second.Delete(data);
		}
		else {
			*this = free_nodes.first;
			curr_sum = sum;
		}
	}

	bool Search(long long data) {
		long long data_to_search = ((data + curr_sum) % func_number + func_number) % func_number;
		Node* in_tree_node = nullptr;

		if (!root) {
			return false;
		}

		in_tree_node = root;

		while (true) {
			if (data_to_search == in_tree_node->data) {
				Splay(in_tree_node);
				return true;
			}
			else if (data_to_search < in_tree_node->data && in_tree_node->left_child) {
				in_tree_node = in_tree_node->left_child;
			}
			else if (data_to_search > in_tree_node->data && in_tree_node->right_child) {
				in_tree_node = in_tree_node->right_child;
			}
			else {
				Splay(in_tree_node);
				return false;
			}
		}
	}

	long long CountSum(long long data1, long long data2) {
		long long lower_bound = ((data1 + curr_sum) % func_number + func_number) % func_number,
				  upper_bound = ((data2 + curr_sum) % func_number + func_number) % func_number,
				  sum = 0;

		Search(data1);
		Search(data2);

		if (!root || lower_bound > root->data) {
			curr_sum = sum;
			return sum;
		}

		sum = upper_bound >= root->data ? root->data : 0;
		if (root->left_child) {
			sum += CountSum(root->left_child, lower_bound, upper_bound);
		}
		curr_sum = sum;
		return sum;
	}

private:

	static void Clear(Node* to_erase) {
		if (to_erase->left_child) Clear(to_erase->left_child);
		if (to_erase->right_child) Clear(to_erase->right_child);
		delete to_erase;
	}

	static void Swap(SplayTree& tree1, SplayTree& tree2) {
		std::swap(tree1.root, tree2.root);
		std::swap(tree1.curr_sum, tree2.curr_sum);
	}

	static void Copy(Node* to_copy, Node* node_child, Node* node_parent) {
		node_child->data = to_copy->data;
		if (to_copy->left_child) {
			Node* new_node = new Node;
			Copy(to_copy->left_child, new_node, node_child);
			node_child->left_child = new_node;
		}
		if (to_copy->right_child) {
			Node* new_node = new Node;
			Copy(to_copy->right_child, new_node, node_child);
			node_child->right_child = new_node;
		}
		node_child->parent = node_parent;
	}

	void Splay(Node* to_move_up) {
		while (true) {
			if (!to_move_up) return;
			if (!to_move_up->parent || to_move_up->data == root->data) {
				root = to_move_up;
				return;
			}
			if (to_move_up->parent->data == root->data) {
				if (to_move_up->parent->left_child && to_move_up->parent->left_child->data == to_move_up->data) {
					Zig(to_move_up);
				}
				else {
					Zag(to_move_up);
				}
			}
			else if (to_move_up->parent->parent) {
				if (to_move_up->parent->left_child && to_move_up->parent->parent->left_child && 
					to_move_up->parent->left_child->data == to_move_up->data && to_move_up->parent->parent->left_child->data == to_move_up->parent->data) {
					Zig(to_move_up->parent);
					Zig(to_move_up);
				}
				else if (to_move_up->parent->right_child && to_move_up->parent->parent->right_child && 
					to_move_up->parent->right_child->data == to_move_up->data && to_move_up->parent->parent->right_child->data == to_move_up->parent->data) {
					Zag(to_move_up->parent);
					Zag(to_move_up);
				}
				else if (to_move_up->parent->left_child && to_move_up->parent->parent->right_child && 
					to_move_up->parent->left_child->data == to_move_up->data && to_move_up->parent->parent->right_child->data == to_move_up->parent->data) {
					Zig(to_move_up);
					Zag(to_move_up);
				}
				else {
					Zag(to_move_up);
					Zig(to_move_up);
				}
			}
			else {
				throw std::runtime_error("Splay error!");
			}
		}
	}

	// from left to right
	void Zig(Node* to_move_up) { 
		Node* temp = to_move_up->parent;

		to_move_up->parent = temp->parent;
		if (temp->parent) {
			if (temp->parent->left_child && temp->parent->left_child->data == temp->data) {
				temp->parent->left_child = to_move_up;
			}
			else if (temp->parent->right_child && temp->parent->right_child->data == temp->data) {
				temp->parent->right_child = to_move_up;
			}
		}
		if (to_move_up->right_child) {
			to_move_up->right_child->parent = temp;
			temp->left_child = to_move_up->right_child;
		}
		else {
			temp->left_child = nullptr;
		}
		to_move_up->right_child = temp;
		temp->parent = to_move_up;
	}

	// from right to left
	void Zag(Node* to_move_up) { 
		Node* temp = to_move_up->parent;

		to_move_up->parent = temp->parent;
		if (temp->parent) {
			if (temp->parent->left_child && temp->parent->left_child->data == temp->data) {
				temp->parent->left_child = to_move_up;
			}
			else if (temp->parent->right_child && temp->parent->right_child->data == temp->data) {
				temp->parent->right_child = to_move_up;
			}
		}
		if (to_move_up->left_child) {
			to_move_up->left_child->parent = temp;
			temp->right_child = to_move_up->left_child;
		}
		else {
			temp->right_child = nullptr;
		}
		to_move_up->left_child = temp;
		temp->parent = to_move_up;
	}

	long long CountSum(Node* node, long long lower_bound, long long upper_bound) {
		long long sum = node->data >= lower_bound ? node->data : 0;

		if (sum != 0 && node->left_child) {
			sum += CountSum(node->left_child, lower_bound, upper_bound);
		}
		if (node->right_child) {
			sum += CountSum(node->right_child, lower_bound, upper_bound);
		}
		return sum;
	}

	long long GetMin() const {
		Node* in_tree_node = root;

		while (true) {
			if (in_tree_node->left_child) {
				in_tree_node = in_tree_node->left_child;
				continue;
			}
			break;
		}
		return in_tree_node->data;
	}

	long long GetMaxAndSplay() {
		Node* in_tree_node = root;

		while (true) {
			if (in_tree_node->right_child) {
				in_tree_node = in_tree_node->right_child;
				continue;
			}
			break;
		}
		Splay(in_tree_node);
		return in_tree_node->data;
	}

	static SplayTree Merge(SplayTree& splay_tree1, SplayTree& splay_tree2) {
		SplayTree new_tree;

		if (!splay_tree1.root && !splay_tree2.root) return new_tree;
		if (!splay_tree1.root) return splay_tree2;
		if (!splay_tree2.root) return splay_tree1;

		if (splay_tree1.GetMaxAndSplay() > splay_tree2.GetMin()) {
			throw std::runtime_error("Error! Bad tree!");
		}

		new_tree.root = splay_tree1.root;
		new_tree.root->right_child = splay_tree2.root;
		new_tree.root->right_child->parent = new_tree.root;
		return new_tree;
	}

	static std::pair<SplayTree, SplayTree> Split(SplayTree& tree_to_split, long long data) {
		long long data_to_devide = ((data + tree_to_split.curr_sum) % func_number + func_number) % func_number;
		Node* new_root = nullptr;

		if (data_to_devide > tree_to_split.root->data) {
			if (tree_to_split.root->right_child) {
				new_root = tree_to_split.root->right_child;
				new_root->parent = nullptr;
				tree_to_split.root->right_child = nullptr;
			}
			SplayTree splay_tree1{ tree_to_split.root };
			SplayTree splay_tree2{ new_root };
			return std::make_pair(splay_tree1, splay_tree2);
		}
		else {
			if (tree_to_split.root->left_child) {
				new_root = tree_to_split.root->left_child;
				new_root->parent = nullptr;
				tree_to_split.root->left_child = nullptr;
			}
			SplayTree splay_tree1{ new_root };
			SplayTree splay_tree2{ tree_to_split.root };
			return std::make_pair(splay_tree1, splay_tree2);
		}
	}

	// Data
	Node* root = nullptr;
	long long curr_sum = 0;
	static const int func_number = 1000000001;
};

int main() {
	std::size_t count = 0;
	char command = 0;
	long long data, additional_data = 0;
	SplayTree splay_tree;

	std::cin >> count;
	while (count > 0) {
		std::cin >> command;
		switch (command) {
		case '+': 
			std::cin >> data;
			splay_tree.Insert(data);
			break;
		case '-':
			std::cin >> data;
			splay_tree.Delete(data);
			break;
		case '?':
			std::cin >> data;
			if (splay_tree.Search(data)) {
				std::cout << "Found\n";
			}
			else {
				std::cout << "Not found\n";
			}
			break;
		case 's':
			std::cin >> data >> additional_data;
			std::cout << splay_tree.CountSum(data, additional_data) << "\n";
		}
		count--;
	}
	SplayTree to_paste{ splay_tree };
	std::cout << "Tree is copied!";
	SplayTree to_move{ (SplayTree&&)splay_tree };
	std::cout << "Tree is moved!";
	return 0;
}
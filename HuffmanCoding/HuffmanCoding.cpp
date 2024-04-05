#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <deque>
#include <functional>
#include <string_view>

#include "PriorityQueue.h"

class HuffmanCode {
public:

	class TreeNode {
	public:

		TreeNode() = default;
		TreeNode(TreeNode* n0, TreeNode* n1, const char c, int p) : node_zero(n0), node_one(n1), ch(c), priority(p) {}
		~TreeNode() {
			Clear(node_zero);
			Clear(node_one);
		}

		int GetPriority() const {
			return priority;
		}
		TreeNode* GetNodeZero() const {
			return node_zero;
		}
		TreeNode* GetNodeOne() const {
			return node_one;
		}
		char GetCharacter() const {
			return ch;
		}
		static bool IsPriorityHigher(const TreeNode* val1, const TreeNode* val2) {
			return val1->GetPriority() < val2->GetPriority();
		}

	private:

		void Clear(TreeNode* node) {
			if (!node) return;
			Clear(node->node_zero);
			Clear(node->node_one);
			delete node;
		}

		TreeNode* node_zero = nullptr;
		TreeNode* node_one = nullptr;
		char ch = '\0';
		int priority = 0;
	};

	HuffmanCode() = default;
	HuffmanCode(const std::string_view str, std::unordered_map<char, std::string> k) : string_to_decode(str), key(std::move(k)) {};
	HuffmanCode(const std::string_view str) : string_to_encode(str) {};

	void Encode() {
		string_to_decode.clear();
		CreateKey();
		for (auto ch : string_to_encode) {
			string_to_decode.append(key.at(ch));
		}
	}
	void Decode() {
		string_to_encode.clear();
		ReadString();
	}
	void PrintEncodedInfo() const {
		std::cout << key.size() << "\n";
		for (auto ch : key) {
			std::cout << ch.first << ": " << ch.second << "\n";
		}
		std::cout << string_to_decode << "\n";
	}
	void PrintDecodedInfo() const {
		std::cout << string_to_encode << "\n";
	}

private:

	void CreateKey() {
		std::string to_encode = string_to_encode;
		std::sort(to_encode.begin(), to_encode.end());
		PriorityQueue<TreeNode*> parsed_str{TreeNode::IsPriorityHigher};
		int count = 0;
		std::string code;
		for (std::size_t i = 0; i < to_encode.size(); i++) {
			count++;
			if (to_encode[i] != to_encode[i + 1]) {
				char ch = to_encode[i];
				code.push_back(count);
				TreeNode* node = new TreeNode{ nullptr, nullptr, ch, count };
				parsed_str.Insert(node);
				key.insert({ ch, code });
				count = 0;
				code.clear();
			}
		}
		TreeNode* tree = BuildTree(parsed_str);
		if (key.size() > 1) {
			ReadTree(tree, code);
			return;
		}
		key.at(tree->GetCharacter()) = "0";
	}
	void ReadTree(const TreeNode* tree, std::string code) {
		if (!tree->GetNodeZero() && !tree->GetNodeOne()) {
			key.at(tree->GetCharacter()) = code;
			return;
		}
		if (tree->GetNodeZero()) {
			code.push_back('0');
			ReadTree(tree->GetNodeZero(), code);
			code.pop_back();
		}
		if (tree->GetNodeOne()) {
			code.push_back('1');
			ReadTree(tree->GetNodeOne(), code);
			code.pop_back();
		}
		return;
	}
	static TreeNode* BuildTree(PriorityQueue<TreeNode*>& d) {
		std::size_t index = 0;
		if (d.GetSize() == 1) {
			return d.GetTop();
		}
		while (d.GetSize() > 1) {
			TreeNode* n1 = d.GetTop();
			d.ExtracTop();
			TreeNode* n2 = d.GetTop();
			d.ExtracTop();
			TreeNode* node = new TreeNode { n1, n2, ' ', n1->GetPriority() + n2->GetPriority()};
			d.Insert(node);
		}
		return d.GetTop();
	}
	void ReadString() {
		std::unordered_map<std::string, char> reversed_key;
		for (auto k : key) {
			reversed_key.insert({ k.second, k.first });
		}
		std::string code;
		for (auto ch : string_to_decode) {
			code.push_back(ch);
			if (reversed_key.contains(code)) {
				string_to_encode.push_back(reversed_key.at(code));
				code.clear();
			}
		}
	}

	std::string string_to_encode;
	std::string string_to_decode;
	std::unordered_map<char, std::string> key;
};


int main() {
	std::string str = "ajhdfygaksdfbkhjaahdgfhhasdhgfygv";
	HuffmanCode code{ str };
	code.Encode();
	code.PrintEncodedInfo();
	code.Decode();
	code.PrintDecodedInfo();
	return 0;
}
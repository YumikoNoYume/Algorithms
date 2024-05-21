#include <iostream>
#include <vector> 
#include <list>
#include <array>
#include <algorithm>
#include <utility>

namespace hash {
	namespace details {
		long long CalculatePowerMod(std::size_t final_index, int x, int prime_number) {
			long long power_mod = 281134874;
			for (std::size_t index = 19; index < final_index; index++) {
				power_mod = power_mod * x % prime_number;
			}
			return power_mod;
		}

		long long CalculateHash(const char* data_to_process, std::size_t size_of_data) {
			static std::array<long long, 20> power_mods_of_263 {{1, 263, 69169, 18191447, 784350533, 284188737, 741637313, 50611954, 310943811, 778221726,
				672312510, 818188898, 183678669, 307489611, 869767133, 748754383, 922401357, 591555197, 579015726, 281134874}};
			int prime_number = 1000000007, x = 263, byte = 0;
			long long sum = 0, power_mod = 0;
			for (std::size_t index = 0; index < size_of_data; index++) {
				if (index > 19) {
					power_mod = CalculatePowerMod(index, x, prime_number);
				}
				else {
					power_mod = power_mods_of_263[index];
				}
				byte = data_to_process[index] % prime_number;
				sum += byte * power_mod;
			}
			return sum % prime_number;
		}
	}

	template<class key>
	long long Hashify(const key& data_key) {
		return details::CalculateHash(reinterpret_cast<const char*>(&data_key), sizeof(data_key));
	}

	template<>
	long long Hashify<std::string>(const std::string& data_key) {
		return details::CalculateHash(data_key.c_str(), data_key.size());
	}
}

template <class key, class type>
class HashTable {
public:
	// Exceptions
	class NonExistentKeyAccess : public std::exception {
	public:
		NonExistentKeyAccess() {}
		const char* what() const override {
			return "Trying to get access to a non-existent key";
		}
	};

	// Constructors
	HashTable() {
		table.resize(1);
	}

	HashTable(const std::initializer_list<std::pair<key, type>>& values) : HashTable() {
		for (const auto& val : values) {
			Insert(val.first, val.second);
		}
	}

	HashTable(const std::vector<std::pair<key, type>>& values) : HashTable() {
		for (const auto& val : values) {
			Insert(val.first, val.second);
		}
	}

	~HashTable() = default;
	  
	// Modifiers
	void Insert(const key& data_key, const type& data) {
		long long data_key_hash = hash::Hashify(data_key);
		data_key_hash = data_key_hash % table.size();
		if (table[data_key_hash].size() > 0) {
			for (const auto& item : table[data_key_hash]) {
				if (item.first == data_key) {
					return;
				}
			}
		}
		table[data_key_hash].push_front(std::make_pair(data_key, data));
		size++;
		if (size == table.size()) {
			Resize();
		}
	}

	void Erase(const key& data_key) {
		long long data_key_hash = hash::Hashify(data_key);
		data_key_hash = data_key_hash % table.size();
		if (table[data_key_hash].size() > 0) {
			for (auto it = table[data_key_hash].begin(); it != table[data_key_hash].end(); it++) {
				if ((*it).first == data_key) {
					it = table[data_key_hash].erase(it);
					size--;
					break;
				}
			}
		}
		if (table.size() / size >= 2) {
			Resize();
		}
	}

	void Clear() {
		table.clear();
		table.resize(1);
		size = 0;
	}

	// Lookup
	type& GetAccessTo(const key& data_key) {
		long long data_key_hash = hash::Hashify(data_key);
		data_key_hash = data_key_hash % table.size();
		if (table[data_key_hash].size() > 0) {
			for (auto& item : table[data_key_hash]) {
				if (item.first == data_key) {
					return item.second;
				}
			}
		}
		throw NonExistentKeyAccess();
	}

	bool IsContaining(const key& data_key) const {
		long long data_key_hash = hash::Hashify(data_key);
		data_key_hash = data_key_hash % table.size();
		if (table[data_key_hash].size() > 0) {
			for (const auto& item : table[data_key_hash]) {
				if (item.first == data_key) {
					return true;
				}
			}
		}
		return false;
	}

	// Capacity
	std::size_t GetSize() const {
		return size;
	}

	bool IsEmpty() const {
		if (size == 0) {
			return true;
		}
		return false;
	}

private:
	void Resize() {
		static std::array<long long, 40> prime_numbers {{2, 5, 11, 23, 47, 97, 197, 397, 797, 1597, 
			3203, 6421, 12853, 25717, 51437, 102877, 205759, 411527, 823117, 1646237, 3292489, 
			6584983, 13169977, 26339969, 52679969, 105359939, 210719881, 421439783, 842879579,
			1685759167, 3371518343, 6743036717, 13486073473, 26972146961, 53944293929, 107888587883, 
			215777175787, 431554351609, 863108703229}};
		std::vector<std::list<std::pair<key, type>>> new_table;
		new_table.resize(*std::upper_bound(prime_numbers.begin(), prime_numbers.end(), table.size()));
		if (size > 0) {
			for (const auto& chain : table) {
				if (chain.size() > 0) {
					for (const auto& element : chain) {
						Insert(element, new_table);
					}
				}
			}
		}
		table = std::move(new_table);
	}

	static void Insert(const std::pair<key,type>& data, std::vector<std::list<std::pair<key, type>>>& new_table) {
		long long data_key_hash = hash::Hashify(data.first);
		data_key_hash = data_key_hash % new_table.size();
		new_table[data_key_hash].push_front(data);
	}

	// Data
	std::vector<std::list<std::pair<key, type>>> table;
	std::size_t size = 0;
};

int main() {
	// Test prep section
	std::vector<std::pair<int, std::string>> testing_stuff = { std::make_pair(98, "cat"), std::make_pair(65, "rabbit"), std::make_pair(29, "pug") };
	bool capacity_and_containing_checks = false;
	std::size_t size_check = 0;

	// Test section
	try {
		HashTable<int, std::string> test0; // Empty table
		HashTable<int, std::string> test1 {std::make_pair(10, "seal"), std::make_pair(20, "beaver"), std::make_pair(45, "squid")}; // Initializer list constructor + basic Insert() + basic Hashify() + ClaculateHash() + Resize() + hidden Insert()
		HashTable<int, std::string> test2 {testing_stuff}; // Vector constructor
		test2.GetAccessTo(98) = "bird"; // GetAccessTo()
		//test2.GetAccessTo(10) = "fish"; // EXCEPTION
		capacity_and_containing_checks = test2.IsContaining(65); // IsContaining() = true
		capacity_and_containing_checks = test2.IsEmpty(); // IsEmpty() = false
		size_check = test2.GetSize(); // GetSize() = 3
		test2.Clear(); // Clear()
		test1.Erase(10); // Erase()
	}
	catch (HashTable<int, std::string>::NonExistentKeyAccess exception) {
		std::cout << "Exception caught: " << exception.what() << '\n';
	}
	return 0;
}
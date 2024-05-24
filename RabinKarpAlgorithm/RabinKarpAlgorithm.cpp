#include <iostream>
#include <vector>

namespace utilits {
	const long long prime_number = 5552111;
	const int rand_number = 263;
	const std::size_t precounted_powers = 19;
	std::vector<long long> power_mods_of_rand_number {{1, 263, 69169, 1535114, 3982990, 3729502, 3687490, 3742556,
		1568581, 1680589, 3378138, 112534, 1835887, 5356735, 4137222, 5427741, 603356, 3223520, 3864888, 429231}};
	
	long long CalculatePowerMod(const std::size_t final_index) {
		if (final_index > power_mods_of_rand_number.size() - 1) {
			long long power_mod = power_mods_of_rand_number.back();

			for (std::size_t index = power_mods_of_rand_number.size() - 1; index < final_index; index++) {
				power_mod = power_mod * rand_number % prime_number;
				power_mods_of_rand_number.push_back(power_mod);
			}
		}
		return power_mods_of_rand_number[final_index];
	}
}

namespace hash {
	long long CalculateHash(const std::string& to_hashify, const int window_size) {
		long long hash = 0, 
			ascii = 0, 
			power = 0, 
			rand_number_power_mod = 0;

		for (std::size_t index = to_hashify.size() - window_size; index < to_hashify.size(); index++) {
			ascii = to_hashify[index];
			rand_number_power_mod = power > utilits::precounted_powers ? utilits::CalculatePowerMod(power) : utilits::power_mods_of_rand_number[power];
			hash = (hash + ascii * rand_number_power_mod) % utilits::prime_number;
			power++;
		}
		return hash;
	}

	long long CalculateHash(const long long& prev_window_hash, const int new_ascii, const int prev_last_ascii, const int window_size) {
		long long rand_number_power_mod = window_size > utilits::precounted_powers ? utilits::CalculatePowerMod(window_size - 1) : utilits::power_mods_of_rand_number[window_size - 1],
			hash_without_prev_ascii = (((prev_window_hash - (prev_last_ascii * rand_number_power_mod) % utilits::prime_number) % utilits::prime_number + utilits::prime_number) % utilits::prime_number * utilits::rand_number) % utilits::prime_number,
			new_hash = (hash_without_prev_ascii + new_ascii * utilits::power_mods_of_rand_number[0]) % utilits::prime_number;
		return new_hash;
	}
}

std::vector<std::size_t> FindPatternMatches(const std::string& pattern, const std::string& text) {
	std::vector<std::size_t> match_indexes;
	std::vector<long long> each_window_hash;
	std::size_t window_size = pattern.size(), 
		start_index = text.size() - window_size;
	long long prev_window_hash = hash::CalculateHash(text, window_size),
		standart_hash = hash::CalculateHash(pattern, window_size);

	each_window_hash.push_back(prev_window_hash);
	for (int index = text.size() - window_size - 1; index > -1; index--) {
		prev_window_hash = hash::CalculateHash(prev_window_hash, text[index], text[index + window_size], window_size);
		each_window_hash.push_back(prev_window_hash);
	}
	for (auto hash : each_window_hash) {
		if (hash == standart_hash) {
			std::size_t pattern_index = 0;

			for (std::size_t index = start_index; index < start_index + window_size; index++) {
				if (pattern[pattern_index] != text[index]) {
					break;
				}
				if (pattern_index == window_size - 1) {
					match_indexes.push_back(start_index);
				}
				pattern_index++;
			}
		}
		start_index--;
	}
	return match_indexes;
}


int main() {
	std::string pattern, text;
	std::cin >> pattern >> text;
	std::vector<std::size_t> match_indexes;
	match_indexes = std::move(FindPatternMatches(pattern, text));
	while (!match_indexes.empty()) {
		std::cout << match_indexes.back() << " ";
		match_indexes.pop_back();
	}
	return 0;
}
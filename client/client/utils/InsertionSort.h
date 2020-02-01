#pragma once
#include <vector>

namespace card {
	template<typename T>
	void insertionSortAscending(std::vector<T>& vec, T elemToAdd);

}

template<typename T>
void card::insertionSortAscending(std::vector<T>& vec, T elemToAdd) {
	bool wasInserted = false;
	for(auto it = vec.begin(); it != vec.end(); ++it) {
		if(elemToAdd < *it) {
			it = vec.insert(it, elemToAdd);
			wasInserted = true;
			break;
		}
	}

	if(!wasInserted) {
		vec.push_back(elemToAdd);
	}
}
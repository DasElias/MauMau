#pragma once
#include <memory>

namespace card {
	template<typename T>
	bool operator==(const T& ref, std::shared_ptr<T> ptr);
	
	template<typename T>
	bool operator==(std::shared_ptr<T> ptr, const T& ref);
}


template<typename T>
inline bool card::operator==(const T& ref, std::shared_ptr<T> ptr) {
	return &ref == ptr.get();
}

template<typename T>
inline bool card::operator==(std::shared_ptr<T> ptr, const T& ref) {
	return operator==(ref, ptr);
}

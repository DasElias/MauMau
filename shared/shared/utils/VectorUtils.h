#pragma once
#include <vector>
#include <initializer_list>
#include <algorithm>

namespace card {
    template <class T>
    std::vector<T>& operator+=(std::vector<T>& lhs, std::initializer_list<T> l) {
        lhs.insert(std::end(lhs), l);
        return lhs;
    }
}
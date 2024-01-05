#pragma once
#include <utility>

// first==width, second==height
typedef int width, height;
typedef std::pair<width, height> dims;       

template<typename T> using range = std::pair<T, T>;

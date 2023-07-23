#pragma once
#include <concepts>

//C++20 concept created so templates can require arithmetic properties of tempalted typenames
template<typename T>
concept arithmetic = std::integral<T> or std::floating_point<T>;

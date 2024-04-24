#pragma once
#include <concepts>

//C++20 concept created so templates can require arithmetic properties of tempalted typenames
template<typename T>
concept arithmetic = std::is_arithmetic_v<T> or std::is_unsigned_v<T>;

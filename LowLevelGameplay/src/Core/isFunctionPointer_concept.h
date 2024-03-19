#pragma once
#include <concepts>

template<typename T>
concept isFunctionPointer = std::is_function_v<T>;

#pragma once

#include <concepts>
#include "Core\Component.h"

template<class T>
concept isComponent = std::derived_from<T, LLGP::Core::Component> == true;

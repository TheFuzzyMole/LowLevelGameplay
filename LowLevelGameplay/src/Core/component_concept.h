#pragma once

#include <concepts>
#include <Core/Components/Component.h>

template<class T>
concept isComponent = std::derived_from<T, LLGP::Component> == true;


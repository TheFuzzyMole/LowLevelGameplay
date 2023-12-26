#pragma once
#include "UUID.h"

namespace LLGP::Core
{
	class Object
	{
	public:
		Object();
		Object(const Object&) = default;

		UUID uuid;

		inline bool operator==(const Object& rhs) { return (uint64_t)this->uuid == (uint64_t)rhs.uuid; }
		inline bool operator!=(const Object& rhs) { return !(*this == rhs); }
	};
}

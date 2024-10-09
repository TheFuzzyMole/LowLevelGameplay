#pragma once
#include <Core/Assets/Asset.h>
#include <string>

namespace LLGP
{
	class Prefab : public Asset
	{
	public:
		Prefab(std::string path);

		std::string _data;
	};
}

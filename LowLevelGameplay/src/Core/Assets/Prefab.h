#pragma once
#include <Core/Assets/Asset.h>
#include <string>

namespace LLGP
{
	class Prefab : public Asset
	{
	public:
		Prefab(std::filesystem::path path);

		std::string _data;
	};
}

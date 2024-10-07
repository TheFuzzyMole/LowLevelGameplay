#pragma once
#include <Core/Assets/Asset.h>
#include <yaml-cpp/yaml.h>

namespace LLGP
{
	class Prefab : public Asset
	{
	public:
		Prefab(std::string path);

		YAML::Node _data;
	};
}

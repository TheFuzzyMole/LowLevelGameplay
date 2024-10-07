#include <Core/Assets/Prefab.h>
#include <fstream>
#include <iostream>

namespace LLGP
{
	Prefab::Prefab(std::string path) : Asset(path)
	{
		std::ifstream stream(path);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		_data = YAML::Load(strStream.str());
		if (!_data["Prefab"]) { std::cout << "ERROR: Deserializing non-prefab file as prefab at: " << path << std::endl; }
	}
}

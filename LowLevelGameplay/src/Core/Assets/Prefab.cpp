#include <Core/Assets/Prefab.h>
#include <fstream>
#include <sstream>
#include <filesystem>


namespace LLGP
{
	Prefab::Prefab(std::filesystem::path path) : Asset(path)
	{
		std::ifstream stream(path);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		_data = strStream.str();
	}
}

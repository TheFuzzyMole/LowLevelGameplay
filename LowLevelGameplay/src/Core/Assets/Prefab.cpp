#include <Core/Assets/Prefab.h>
#include <fstream>
#include <sstream>


namespace LLGP
{
	Prefab::Prefab(std::string path) : Asset(path)
	{
		std::ifstream stream(path);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		_data = strStream.str();
	}
}

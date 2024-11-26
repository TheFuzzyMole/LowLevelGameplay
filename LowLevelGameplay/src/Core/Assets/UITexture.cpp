#include "UITexture.h"
#include <Utils/Debug.h>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <sstream>
#include <filesystem>

namespace LLGP
{
	UITexture::UITexture(std::filesystem::path path) : Asset(path)
	{
		std::ifstream UITextureFile;
		UITextureFile.open(path);
		if (UITextureFile.is_open())
		{
			_tex = std::make_unique<sf::Texture>();
			std::string line;
			std::getline(UITextureFile, line);
			_IsNineSlice = line == "true";
			std::getline(UITextureFile, line);
			std::stringstream lineStream(line);
			lineStream >> _InsetMin.x >> _InsetMin.y;
			std::getline(UITextureFile, line);
			lineStream.str(line);
			lineStream >> _InsetMax.x >> _InsetMax.y;
			std::getline(UITextureFile, line);
			Debug::LogWarning("LoadingTexture: " + line);
			_tex->loadFromFile(line);
			UITextureFile.close();
		}
	}
}

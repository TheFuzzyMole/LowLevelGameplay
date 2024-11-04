#include <Core/Assets/Texture.h>
#include <SFML/Graphics.hpp>
#include <filesystem>

namespace LLGP
{
	Texture::Texture(std::filesystem::path path) : Asset(path)
	{
		_tex = std::make_unique<sf::Texture>();
		_tex->loadFromFile(m_AssetPath.string());
	}
}

#include <Core/Assets/Texture.h>
#include <SFML/Graphics.hpp>
#include <Utils/Debug.h>
#include <filesystem>

namespace LLGP
{
	Texture::Texture(std::filesystem::path path) : Asset(path)
	{
		_tex = std::make_unique<sf::Texture>();
		Debug::LogWarning("LoadingTexture: " + m_AssetPath.string());
		_tex->loadFromFile(m_AssetPath.string());
	}
}

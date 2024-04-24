#include <Core/Texture.h>
#include <SFML/Graphics.hpp>

namespace LLGP
{
	Texture::Texture(std::string path) : Asset(path)
	{
		_tex = std::make_unique<sf::Texture>();
		_tex->loadFromFile(path);
	}
}

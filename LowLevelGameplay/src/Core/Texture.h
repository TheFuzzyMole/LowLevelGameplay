#pragma once
#include <Core/Asset.h>
#include <memory>
#include <Core/Vector2.h>

namespace sf
{
	class Texture;
}

namespace LLGP
{
	class Texture : public Asset
	{
	public:
		Texture(std::string path);

		operator sf::Texture*() { return _tex.get(); }

		std::unique_ptr<sf::Texture> _tex;
	};
}

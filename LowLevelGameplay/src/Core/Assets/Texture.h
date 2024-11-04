#pragma once
#include <Core/Assets/Asset.h>
#include <memory>
#include <Core/Maths/Vector2.h>

namespace sf
{
	class Texture;
}

namespace LLGP
{
	class Texture : public Asset
	{
	public:
		Texture(std::filesystem::path path);

		operator sf::Texture*() { return _tex.get(); }

		std::unique_ptr<sf::Texture> _tex;
	};
}

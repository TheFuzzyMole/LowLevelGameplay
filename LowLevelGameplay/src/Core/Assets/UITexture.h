#pragma once
#include <Core/Assets/Asset.h>
#include <Core/Maths/Vector2.h>
#include <memory>

namespace sf
{
	class Texture;
}
namespace LLGP
{
	class UITexture : public Asset
	{
	public:
		UITexture(std::filesystem::path path);

		operator sf::Texture* () { return _tex.get(); }

		std::unique_ptr<sf::Texture> _tex;

		bool _IsNineSlice;
		/// <summary>
		/// distance of slices in pixels from the (left, top)
		/// </summary>
		LLGP::Vector2u _InsetMin;
		/// <summary>
		/// distance of slices in pixels from the (right, bottom)
		/// </summary>
		LLGP::Vector2u _InsetMax;
	};
}

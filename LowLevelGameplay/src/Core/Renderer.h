#pragma once
#include <Core/Component.h>
#include <Core/Event.h>
#include <Core/Vector2.h>
#include <SFML/Graphics.hpp>
#include <memory>

namespace sf
{
	class Texture;
}

namespace LLGP
{
	class Texture;

	enum RenderLayers
	{
		DEFAULT,
		BACKGROUND,
		DEBUG
	};

	class Renderer : public Component
	{
	public:
		Renderer(GameObject* owner);
		~Renderer();

		static Event<sf::RenderWindow&, RenderLayers> OnRenderLayer;

		void SetupTexture(std::string filePath, LLGP::Vector2u spritesInTex);
		void SetupQuad(LLGP::Vector2f inSize);
		void SetupSpriteUV(LLGP::Vector2u inIndex);
		void SetupRenderLayer(LLGP::RenderLayers layer);

		void Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override;

	private:
		sf::RectangleShape m_Quad;
		LLGP::Vector2u m_SpritesInTex;
		LLGP::Vector2u m_SpriteIndex;
		std::shared_ptr<LLGP::Texture> m_Texture;
		sf::IntRect m_TexUV;

		RenderLayers m_RenderLayer;

		void Handle_Render(sf::RenderWindow& window, RenderLayers layerToRender);
	};
}

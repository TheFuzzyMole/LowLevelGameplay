#include <Core/Renderer.h>
#include <Core/AssetManager.h>
#include <Core/Texture.h>
#include <Core/GameObject.h>
#include <algorithm>


namespace LLGP
{
	Event<sf::RenderWindow&, RenderLayers> Renderer::OnRenderLayer;

	Renderer::Renderer(GameObject* owner) : Component(owner)
	{
		m_RenderLayer = RenderLayers::DEFAULT;

		OnRenderLayer += std::bind(&Renderer::Handle_Render, this, std::placeholders::_1, std::placeholders::_2);
	}

	Renderer::~Renderer()
	{
		OnRenderLayer -= std::bind(&Renderer::Handle_Render, this, std::placeholders::_1, std::placeholders::_2);
	}

	void Renderer::SetupTexture(std::string filePath, LLGP::Vector2u spritesInTex)
	{
		m_Texture = AssetManager::GetAsset<LLGP::Texture>(filePath);
		m_SpritesInTex = spritesInTex;
		m_Quad.setTexture(m_Texture->_tex.get());
	}

	void Renderer::SetupQuad(LLGP::Vector2f inSize)
	{
		m_Quad.setSize(inSize);
		m_Quad.setOrigin(inSize / 2.f);
	}

	void Renderer::SetupSpriteUV(LLGP::Vector2u inIndex)
	{
		inIndex = LLGP::Vector2u(std::min(inIndex.x, m_SpritesInTex.x - 1), std::min(inIndex.y, m_SpritesInTex.y - 1));
		LLGP::Vector2u spriteSize = (LLGP::Vector2u)m_Texture->_tex->getSize();
		spriteSize = LLGP::Vector2u(spriteSize.x / m_SpritesInTex.x, spriteSize.y / m_SpritesInTex.y);
		m_TexUV = sf::IntRect(spriteSize.x * inIndex.x, spriteSize.y * inIndex.y, spriteSize.x, spriteSize.y);
		m_Quad.setTextureRect(m_TexUV);
	}

	void Renderer::SetupRenderLayer(LLGP::RenderLayers layer)
	{
		m_RenderLayer = layer;
	}

	void Renderer::Handle_Render(sf::RenderWindow& window, RenderLayers layerToRender)
	{
		if (m_RenderLayer != layerToRender) return;

		m_Quad.setPosition(_GameObject->transform->GetPosition());
		window.draw(m_Quad);
	}
}

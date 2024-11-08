#include <Core/Components/Renderer.h>
#include <Core/Assets/AssetManager.h>
#include <Core/Assets/Texture.h>
#include <Core/GameObject.h>
#include <algorithm>


namespace LLGP
{
	Event<sf::RenderWindow&, RenderLayers> Renderer::OnRenderLayer;

	Renderer::Renderer(GameObject* owner) : Component(owner)
	{
		m_RenderLayer = RenderLayers::DEFAULT;
		using namespace std::placeholders;
		OnRenderLayer.AddListener(this, std::bind(&Renderer::Handle_Render, this, _1, _2));
	}

	Renderer::~Renderer()
	{
		using namespace std::placeholders;
		OnRenderLayer.RemoveListener(this, std::bind(&Renderer::Handle_Render, this, _1, _2));
	}

	void Renderer::SetupTexture(const std::string& filePath, LLGP::Vector2u spritesInTex)
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
		m_SpriteIndex = inIndex;
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

    void Renderer::Serialize(YAML::Emitter& out)
    {
		out << YAML::Key << "Renderer" << YAML::Value << YAML::BeginMap; //Renderer
		out << YAML::Key << "UUID" << YAML::Value << uuid;

		out << YAML::Key << "TextureFilePath" << YAML::Value << m_Texture->GetAssetPath().string();
		out << YAML::Key << "QuadSize" << YAML::Value << (LLGP::Vector2f)m_Quad.getSize();
		out << YAML::Key << "SpritesInTex" << YAML::Value << m_SpritesInTex;
		out << YAML::Key << "CurrentSpriteIndex" << YAML::Value << m_SpriteIndex;
		out << YAML::Key << "RenderLayer" << YAML::Value << (int)m_RenderLayer;
		out << YAML::EndMap; //Renderer
    }

    bool Renderer::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
    {
		if (!node["TextureFilePath"] || !node["QuadSize"] || !node["SpritesInTex"] || !node["CurrentSpriteIndex"] || !node["RenderLayer"]) { return false; }
		SetupQuad(node["QuadSize"].as<LLGP::Vector2f>());
		SetupTexture(node["TextureFilePath"].as<std::string>(), node["SpritesInTex"].as<LLGP::Vector2u>());
		SetupSpriteUV(node["CurrentSpriteIndex"].as<LLGP::Vector2u>());
		m_RenderLayer = (LLGP::RenderLayers)node["RenderLayer"].as<int>();
		return true;
    }
}

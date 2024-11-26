#include <Core/Components/UI/Image.h>
#include <Core/Components/UI/RectTransform.h>
#include <Core/Components/UI/Canvas.h>
#include <Core/GameObject.h>
#include <Core/Assets/AssetManager.h>
#include <Core/Assets/UITexture.h>
#include <Utils/Debug.h>

namespace LLGP
{
	Image::Image(LLGP::GameObject* owner) : Component(owner)
	{
		m_Transform = dynamic_cast<LLGP::RectTransform*>(_GameObject->transform);
		Debug::Assert(m_Transform != nullptr, "UI Image component attached to a GameObject that is missing a RectTransform");
		if (m_Transform != nullptr) //this will never be null as if it is the line above will assert and crash but this is required to stop an MSVC C6011 warning
		{
			m_Transform->OnTransformSizeChanged.AddListener(this, std::bind(&Image::Handle_TransformSizeChanged, this, std::placeholders::_1));
		}
	}

	void Image::Start()
	{
		m_Transform->GetCanvasRef().OnRenderChildren.AddListener(this, std::bind(&Image::Render, this, std::placeholders::_1));
	}

	void Image::Render(sf::RenderWindow& window)
	{
		LLGP::Vector2f pos = m_Transform->GetPosition();
		if (m_Texture->_IsNineSlice)
		{
			for (int quadIdx = 0; quadIdx < 9; quadIdx++)
			{
				m_Quads[quadIdx].setPosition(pos);
				window.draw(m_Quads[quadIdx]);
			}
		}
		else
		{
			m_Quads[4].setPosition(pos);
			window.draw(m_Quads[4]);
		}
	}

	void Image::SetupTexture(std::filesystem::path filePath)
	{
		m_Texture = AssetManager::GetAsset<LLGP::UITexture>(filePath);
		LLGP::Vector2u size(m_Texture->_tex->getSize());
		if (m_Texture->_IsNineSlice)
		{
			for (int quadIdx = 0; quadIdx < 9; quadIdx++)
			{
				m_Quads[quadIdx].setTexture(m_Texture->_tex.get());
			}
			unsigned int centerX = size.x - (m_Texture->_InsetMin.x + m_Texture->_InsetMax.x);
			unsigned int middleY = size.y - (m_Texture->_InsetMin.y + m_Texture->_InsetMax.y);
			unsigned int rightX = size.x - m_Texture->_InsetMax.x;
			unsigned int bottomY = size.y - m_Texture->_InsetMax.y;
			//top left
			m_Quads[0].setTextureRect(sf::IntRect(
					0, 0,
					m_Texture->_InsetMin.x, m_Texture->_InsetMin.y
				));
			//top center
			m_Quads[1].setTextureRect(sf::IntRect(
					m_Texture->_InsetMin.x, 0,
					centerX, m_Texture->_InsetMin.y
				));
			//top right
			m_Quads[2].setTextureRect(sf::IntRect(
					rightX, 0,
					m_Texture->_InsetMax.x, m_Texture->_InsetMin.y
				));
			//middle left
			m_Quads[3].setTextureRect(sf::IntRect(
					0, m_Texture->_InsetMin.y,
					m_Texture->_InsetMin.x, middleY
				));
			//middle center
			m_Quads[4].setTextureRect(sf::IntRect(
					m_Texture->_InsetMin.x, m_Texture->_InsetMin.y,
					centerX, middleY
				));
			//middle right
			m_Quads[5].setTextureRect(sf::IntRect(
					rightX, m_Texture->_InsetMin.y,
					m_Texture->_InsetMax.x, middleY
				));
			//bottom left
			m_Quads[6].setTextureRect(sf::IntRect(
					0, bottomY,
					m_Texture->_InsetMin.x, m_Texture->_InsetMax.y
				));
			//bottom center
			m_Quads[7].setTextureRect(sf::IntRect(
					m_Texture->_InsetMin.x, bottomY,
					centerX, m_Texture->_InsetMax.y
				));
			//bottom right
			m_Quads[8].setTextureRect(sf::IntRect(
					rightX, bottomY,
					m_Texture->_InsetMax.x, m_Texture->_InsetMax.y
				));
		}
		else
		{
			m_Quads[4].setTexture(m_Texture->_tex.get());
			m_Quads[4].setTextureRect(sf::IntRect(0, 0, size.x, size.y));
		}
	}

	void Image::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "Image" << YAML::Value << YAML::BeginMap;

		out << YAML::Key << "TextureFilePath" << YAML::Value << m_Texture->GetAssetPath().string();

		out << YAML::EndMap;
	}

	bool Image::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		if (!node["TextureFilePath"]) { return false; }

		SetupTexture(node["TextureFilePath"].as<std::string>());
		return true;
	}

	void Image::Handle_TransformSizeChanged(LLGP::Vector2f newSize)
	{
		LLGP::Vector2f defaultOrigin = newSize / 2.f;
		if (m_Texture->_IsNineSlice)
		{
			float centerX = newSize.x - (m_Texture->_InsetMin.x + m_Texture->_InsetMax.x);
			float middleY = newSize.y - (m_Texture->_InsetMin.y + m_Texture->_InsetMax.y);
			//top left
			m_Quads[0].setSize(m_Texture->_InsetMin);
			m_Quads[0].setOrigin(defaultOrigin);
			//top center
			m_Quads[1].setSize(LLGP::Vector2u(centerX, m_Texture->_InsetMin.y));
			m_Quads[1].setOrigin(LLGP::Vector2u(defaultOrigin.x - m_Texture->_InsetMin.x, defaultOrigin.y ));
			//top right
			m_Quads[2].setSize(LLGP::Vector2u(m_Texture->_InsetMax.x, m_Texture->_InsetMin.y ));
			m_Quads[2].setOrigin(LLGP::Vector2u(defaultOrigin.x - (m_Texture->_InsetMin.x + centerX), defaultOrigin.y ));
			//middle left
			m_Quads[3].setSize(LLGP::Vector2u(m_Texture->_InsetMin.x, middleY ));
			m_Quads[3].setOrigin(LLGP::Vector2u(defaultOrigin.x, defaultOrigin.y - m_Texture->_InsetMin.y ));
			//middle center
			m_Quads[4].setSize(LLGP::Vector2u(centerX, middleY ));
			m_Quads[4].setOrigin(LLGP::Vector2u(defaultOrigin.x - m_Texture->_InsetMin.x, defaultOrigin.y - m_Texture->_InsetMin.y ));
			//middle right
			m_Quads[5].setSize(LLGP::Vector2u(m_Texture->_InsetMax.x, middleY ));
			m_Quads[5].setOrigin(LLGP::Vector2u(defaultOrigin.x - (m_Texture->_InsetMin.x + centerX), defaultOrigin.y - m_Texture->_InsetMin.y ));
			//bottom left
			m_Quads[6].setSize(LLGP::Vector2u(m_Texture->_InsetMin.x, m_Texture->_InsetMax.y ));
			m_Quads[6].setOrigin(LLGP::Vector2u(defaultOrigin.x, defaultOrigin.y - (m_Texture->_InsetMin.y + middleY) ));
			//bottom center
			m_Quads[7].setSize(LLGP::Vector2u(centerX, m_Texture->_InsetMax.y ));
			m_Quads[7].setOrigin(LLGP::Vector2u(defaultOrigin.x - m_Texture->_InsetMin.x, defaultOrigin.y - (m_Texture->_InsetMin.y + middleY) ));
			//bottom right
			m_Quads[8].setSize(m_Texture->_InsetMax);
			m_Quads[8].setOrigin(LLGP::Vector2u(defaultOrigin.x - (m_Texture->_InsetMin.x + centerX), defaultOrigin.y - (m_Texture->_InsetMin.y + middleY) ));
		}
		else
		{
			m_Quads[4].setSize(newSize);
			m_Quads[4].setOrigin(defaultOrigin);
		}
	}
}

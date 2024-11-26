#pragma once
#include <Core/Components/Component.h>
#include <SFML/Graphics.hpp>
#include <memory>
#include <filesystem>

namespace LLGP
{
	class RectTransform;
	class UITexture;

	class Image : public Component
	{
	public:
		Image(LLGP::GameObject* owner);

		void Start() override;

		void Render(sf::RenderWindow& window);
		void SetupTexture(std::filesystem::path filePath);

		void Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override;

	private:

		sf::RectangleShape m_Quads[9];
		std::shared_ptr<LLGP::UITexture> m_Texture;
		LLGP::RectTransform* m_Transform;

		void Handle_TransformSizeChanged(LLGP::Vector2f newSize);
	};
}

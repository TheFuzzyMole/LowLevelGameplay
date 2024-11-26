#pragma once
#include <Core/Components/Component.h>
#include <Core/Event.h>
#include <SFML/Graphics.hpp>

namespace LLGP
{
	class Transform;

	class Canvas : public Component
	{
	public:
		Canvas(LLGP::GameObject* owner);

		static inline LLGP::Event<sf::RenderWindow&> OnRenderUI;
		LLGP::Event<sf::RenderWindow&> OnRenderChildren;

		virtual void Serialize(YAML::Emitter& out) override;
		virtual bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override;

	private:
		LLGP::Transform* m_Camera;
		LLGP::Vector2f m_ReferenceScreenSize;

		void Handle_RenderUI(sf::RenderWindow& window);
	};
}

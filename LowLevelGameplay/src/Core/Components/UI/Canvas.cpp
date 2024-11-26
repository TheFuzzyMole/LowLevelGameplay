#include "Canvas.h"

namespace LLGP
{
	Canvas::Canvas(LLGP::GameObject* owner) : Component(owner)
	{
		OnRenderUI.AddListener(this, std::bind(&Canvas::Handle_RenderUI, this, std::placeholders::_1));
	}
	void Canvas::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "Canvas" << YAML::Value << YAML::BeginMap;

		out << YAML::EndMap;
	}
	bool Canvas::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		return true;
	}
	void Canvas::Handle_RenderUI(sf::RenderWindow& window)
	{
		OnRenderChildren(window);
	}
}

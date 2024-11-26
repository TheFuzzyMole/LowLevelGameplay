#include "LayoutElement.h"

namespace LLGP
{
	LayoutElement::LayoutElement(LLGP::GameObject* owner) : Component(owner)
	{
	}

	float LayoutElement::GetMinWidth() { return m_MinWidth; }
	float LayoutElement::GetMinHeight() { return m_MinHeight; }
	float LayoutElement::GetPreferredWidth() { return m_PreferredWidth; }
	float LayoutElement::GetPreferredHeight() { return m_PreferredHeight; }
	float LayoutElement::GetFlexibleWidth() { return m_FlexibleWidth; }
	float LayoutElement::GetFlexibleHeight() { return m_FlexibleHeight; }

	void LayoutElement::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "LayoutElement" << YAML::Value << YAML::BeginMap;

		out << YAML::Key << "MinWidth" << YAML::Value << m_MinWidth;
		out << YAML::Key << "MinHeight" << YAML::Value << m_MinHeight;
		out << YAML::Key << "PreferredWidth" << YAML::Value << m_PreferredWidth;
		out << YAML::Key << "PreferredHeight" << YAML::Value << m_PreferredHeight;
		out << YAML::Key << "FlexibleWidth" << YAML::Value << m_FlexibleWidth;
		out << YAML::Key << "FlexibleHeight" << YAML::Value << m_FlexibleHeight;

		out << YAML::EndMap;
	}

	bool LayoutElement::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		if (!node["MinWidth"] || !node["MinHeight"] || !node["PreferredWidth"] || !node["PreferredHeight"] || !node["FlexibleWidth"] || !node["FlexibleHeight"]) { return false; }

		m_MinWidth = node["MinWidth"].as<float>();
		m_MinHeight = node["MinHeight"].as<float>();
		m_PreferredWidth = node["PreferredWidth"].as<float>();
		m_PreferredHeight = node["PreferredHeight"].as<float>();
		m_FlexibleWidth = node["FlexibleWidth"].as<float>();
		m_FlexibleHeight = node["FlexibleHeight"].as<float>();

		return true;
	}
}

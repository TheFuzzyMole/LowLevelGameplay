#include "LayoutElement.h"

namespace LLGP
{
	LayoutElement::LayoutElement(LLGP::GameObject* owner) : Component(owner), m_MinWidth(std::nullopt), m_MinHeight(std::nullopt), m_PreferredWidth(std::nullopt), m_PreferredHeight(std::nullopt), m_FlexibleWidth(std::nullopt), m_FlexibleHeight(std::nullopt)
	{}

	float LayoutElement::GetMinWidth() { return m_MinWidth.value_or(0.f); }
	float LayoutElement::GetMinHeight() { return m_MinHeight.value_or(0.f); }
	float LayoutElement::GetPreferredWidth() { return m_PreferredWidth.value_or(0.f); }
	float LayoutElement::GetPreferredHeight() { return m_PreferredHeight.value_or(0.f); }
	float LayoutElement::GetFlexibleWidth() { return m_FlexibleWidth.value_or(0.f); }
	float LayoutElement::GetFlexibleHeight() { return m_FlexibleHeight.value_or(0.f); }
	bool LayoutElement::HasMinWidth() { return m_MinWidth.has_value(); }
	bool LayoutElement::HasMinHeight() { return m_MinHeight.has_value(); }
	bool LayoutElement::HasPreferredWidth() { return m_PreferredWidth.has_value(); }
	bool LayoutElement::HasPreferredHeight() { return m_PreferredHeight.has_value(); }
	bool LayoutElement::HasFlexibleWidth() { return m_FlexibleWidth.has_value(); }
	bool LayoutElement::HasFlexibleHeight() { return m_FlexibleHeight.has_value(); }



	void LayoutElement::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "LayoutElement" << YAML::Value << YAML::BeginMap;

		out << YAML::Key << "MinWidth" << YAML::Value << m_MinWidth.value();
		out << YAML::Key << "MinHeight" << YAML::Value << m_MinHeight.value();
		out << YAML::Key << "PreferredWidth" << YAML::Value << m_PreferredWidth.value();
		out << YAML::Key << "PreferredHeight" << YAML::Value << m_PreferredHeight.value();
		out << YAML::Key << "FlexibleWidth" << YAML::Value << m_FlexibleWidth.value();
		out << YAML::Key << "FlexibleHeight" << YAML::Value << m_FlexibleHeight.value();

		out << YAML::EndMap;
	}

	bool LayoutElement::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		if (node["MinWidth"]) { m_MinWidth = node["MinWidth"].as<float>(); }
		if (node["MinHeight"]) { m_MinHeight = node["MinHeight"].as<float>(); }
		if (node["PreferredWidth"]) { m_PreferredWidth = node["PreferredWidth"].as<float>(); }
		if (node["PreferredHeight"]) { m_PreferredHeight = node["PreferredHeight"].as<float>(); }
		if (node["FlexibleWidth"]) { m_FlexibleWidth = node["FlexibleWidth"].as<float>(); }
		if (node["FlexibleHeight"]) { m_FlexibleHeight = node["FlexibleHeight"].as<float>(); }

		return true;
	}
}

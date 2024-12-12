#pragma once
#include <Core/Components/Component.h>
#include <optional>

namespace LLGP
{
	class LayoutElement : public Component
	{
	public:
		LayoutElement(LLGP::GameObject* owner);

		float GetMinWidth();
		float GetMinHeight();
		float GetPreferredWidth();
		float GetPreferredHeight();
		float GetFlexibleWidth();
		float GetFlexibleHeight();
		bool HasMinWidth();
		bool HasMinHeight();
		bool HasPreferredWidth();
		bool HasPreferredHeight();
		bool HasFlexibleWidth();
		bool HasFlexibleHeight();

		void Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override;

	private:
		std::optional<float> m_MinWidth;
		std::optional<float> m_MinHeight;
		std::optional<float> m_PreferredWidth;
		std::optional<float> m_PreferredHeight;
		std::optional<float> m_FlexibleWidth;
		std::optional<float> m_FlexibleHeight;
	};
}

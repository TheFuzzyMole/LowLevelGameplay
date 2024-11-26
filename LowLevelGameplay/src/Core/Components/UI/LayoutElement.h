#pragma once
#include <Core/Components/Component.h>

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

		void Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override;

	private:
		float m_MinWidth;
		float m_MinHeight;
		float m_PreferredWidth;
		float m_PreferredHeight;
		float m_FlexibleWidth;
		float m_FlexibleHeight;
	};
}

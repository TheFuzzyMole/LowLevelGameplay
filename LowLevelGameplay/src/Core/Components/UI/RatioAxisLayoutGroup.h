#pragma once
#include <Core/Components/UI/LayoutGroup.h>

namespace LLGP
{
	enum LayoutDirection
	{
		VERTICAL,
		HORIZONTAL
	};

	class RatioAxisLayoutGroup : public LayoutGroup
	{
	public:
		RatioAxisLayoutGroup(LLGP::GameObject* owner);

		virtual void UpdateChildSizeAndPosition() override;

		void Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override;

	private:
		bool m_StretchOtherAxis;
		LayoutDirection m_Direction;
		float m_Spacing;
	};
}

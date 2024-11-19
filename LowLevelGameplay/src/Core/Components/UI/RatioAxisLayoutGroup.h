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

	private:
		bool m_StretchOtherAxis;
		LayoutDirection m_Direction;
		float m_Spacing;
	};
}

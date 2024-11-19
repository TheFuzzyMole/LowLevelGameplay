#pragma once
#include <Core/Components/Component.h>

namespace LLGP
{
	class RectTransform;

	class LayoutGroup : public Component
	{
	public:
		LayoutGroup(GameObject* owner);

		virtual void UpdateChildSizeAndPosition();

	protected:
		LLGP::RectTransform* m_Transform;
	};
}

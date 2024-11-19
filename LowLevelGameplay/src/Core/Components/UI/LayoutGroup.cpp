#include "LayoutGroup.h"
#include <Core/GameObject.h>
#include <Core/Components/UI/RectTransform.h>
#include <Utils/Debug.h>

namespace LLGP
{

	LayoutGroup::LayoutGroup(GameObject* owner) : Component(owner)
	{
		m_Transform = dynamic_cast<LLGP::RectTransform*>(_GameObject->transform);
		Debug::Assert((m_Transform != nullptr), "LayoutGroup component added to GameObject which doesnt have a RectTransform");
	}

	void LayoutGroup::UpdateChildSizeAndPosition()
	{
	}
}

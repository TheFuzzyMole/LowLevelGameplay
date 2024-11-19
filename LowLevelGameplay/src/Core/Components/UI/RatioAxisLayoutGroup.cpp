#include "RatioAxisLayoutGroup.h"
#include <Core/GameObject.h>
#include <Core/Components/UI/RectTransform.h>
#include <Core/Components/UI/LayoutElement.h>
#include <Utils/Debug.h>

namespace LLGP
{
	RatioAxisLayoutGroup::RatioAxisLayoutGroup(LLGP::GameObject* owner) : LayoutGroup(owner)
	{
	}

	void RatioAxisLayoutGroup::UpdateChildSizeAndPosition()
	{
		/*
		* cache and loop through children
		* sum layout ratios from layout elements if they exist
		* create cell space for each child based on values agiant sum total (if 0 total then equal share)
		* call controlTransformProperties on each child
		*/

		std::vector<LLGP::RectTransform*> _childTransforms;
		_childTransforms.reserve(m_Transform->GetChildCount());

		float flexibleTotal = 0.f;

		for (LLGP::Transform* child : m_Transform->GetChildren())
		{
			if (LLGP::RectTransform* childRect = dynamic_cast<LLGP::RectTransform*>(child))
			{
				_childTransforms.push_back(childRect);
				LLGP::LayoutElement* le = childRect->GetGameObject()->GetComponent<LLGP::LayoutElement>();
				if (le == nullptr) { continue; }
				flexibleTotal += m_Direction == LayoutDirection::HORIZONTAL ? le->GetFlexibleWidth() : le->GetFlexibleHeight();
			}
			else
			{
				Debug::LogWarning("child " + child->GetGameObject()->GetName() + " of a UI object not set with a RectTransform somehow", _GameObject);
			}
		}

		float totalSize = m_Direction == LayoutDirection::HORIZONTAL ? m_Transform->GetRenderSize().x : m_Transform->GetRenderSize().y;
		float unitSize = totalSize - m_Spacing * (m_Transform->GetChildCount() - 1);
		unitSize /= flexibleTotal == 0 ? m_Transform->GetChildCount() : flexibleTotal;

		float pos = 0;
		for (int childIdx = 0; childIdx < _childTransforms.size(); childIdx++)
		{
			LLGP::LayoutElement* le = _childTransforms[childIdx]->GetGameObject()->GetComponent<LLGP::LayoutElement>();
			float flexibleSize = (le == nullptr ? 0.f : (m_Direction == LayoutDirection::HORIZONTAL ? le->GetFlexibleWidth() : le->GetFlexibleHeight()));
			if (flexibleSize == 0.f && flexibleTotal == 0.f) { flexibleSize = 1.f; }
			float actualSize = flexibleSize * unitSize;
			_childTransforms[childIdx]->SetTransformAlongAxis(m_Direction == LayoutDirection::VERTICAL, LLGP::Vector2f::up, { pos, totalSize - (pos + actualSize) });
			pos += actualSize + m_Spacing;
			if (m_StretchOtherAxis)
			{
				_childTransforms[childIdx]->SetTransformAlongAxis(m_Direction == LayoutDirection::HORIZONTAL, LLGP::Vector2f::up, LLGP::Vector2f::up);
			}
		}
	}
}

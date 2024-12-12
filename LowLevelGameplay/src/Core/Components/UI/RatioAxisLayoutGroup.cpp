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

	void RatioAxisLayoutGroup::UpdateChildSizeAndPosition(const LLGP::Vector2f& newSize)
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

		float totalSize = m_Direction == LayoutDirection::HORIZONTAL ? newSize.x : newSize.y;
		float unitSize = totalSize - m_Spacing * (m_Transform->GetChildCount() - 1);
		unitSize /= flexibleTotal == 0 ? m_Transform->GetChildCount() : flexibleTotal;

		float pos = 0;
		for (int childIdx = 0; childIdx < _childTransforms.size(); childIdx++)
		{
			LLGP::LayoutElement* le = _childTransforms[childIdx]->GetGameObject()->GetComponent<LLGP::LayoutElement>();
			float flexibleSize = (le == nullptr ? 0.f : (m_Direction == LayoutDirection::HORIZONTAL ? le->GetFlexibleWidth() : le->GetFlexibleHeight()));
			if (flexibleSize == 0.f && flexibleTotal == 0.f) { flexibleSize = 1.f; }
			float actualSize = flexibleSize * unitSize;
			LLGP::Vector2f mainAxisAnchors(AlignmentAnchors[m_ChildAlignment * 2 + (m_Direction == LayoutDirection::VERTICAL)]);
			_childTransforms[childIdx]->SetTransformAlongAxis(m_Direction == LayoutDirection::VERTICAL, mainAxisAnchors, {pos - (mainAxisAnchors.x * totalSize), (mainAxisAnchors.y * totalSize) - (pos + actualSize)}, newSize);
			pos += actualSize + m_Spacing;
			if (m_StretchOtherAxis)
			{
				totalSize = m_Direction == LayoutDirection::VERTICAL ? newSize.x : newSize.y;
				LLGP::Vector2f otherAxisAnchors(AlignmentAnchors[m_ChildAlignment * 2 + (m_Direction == LayoutDirection::HORIZONTAL)]);
				_childTransforms[childIdx]->SetTransformAlongAxis(m_Direction == LayoutDirection::HORIZONTAL, otherAxisAnchors, {-(otherAxisAnchors.x * totalSize), (otherAxisAnchors.y * totalSize) - totalSize}, newSize);
			}
		}
	}
	void RatioAxisLayoutGroup::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "RatioAxisLayoutGroup" << YAML::Value << YAML::BeginMap;

		out << YAML::Key << "StretchOtherAxis" << YAML::Value << m_StretchOtherAxis;
		out << YAML::Key << "LayoutDirection" << YAML::Value << (int)m_Direction;
		out << YAML::Key << "Spacing" << YAML::Value << m_Spacing;
		out << YAML::Key << "ChildAlignment" << YAML::Value << (int)m_ChildAlignment;

		out << YAML::EndMap;
	}
	bool RatioAxisLayoutGroup::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		if (!node["StretchOtherAxis"] || !node["LayoutDirection"] || !node["Spacing"] || !node["ChildAlignment"]) { return false; }

		m_StretchOtherAxis = node["StretchOtherAxis"].as<bool>();
		m_Direction = (LayoutDirection)node["LayoutDirection"].as<int>();
		m_Spacing = node["Spacing"].as<float>();
		m_ChildAlignment = (LayoutAlignment)node["ChildAlignment"].as<int>();

		return true;
	}
}

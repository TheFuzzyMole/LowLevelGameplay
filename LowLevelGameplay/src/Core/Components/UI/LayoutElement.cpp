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
}

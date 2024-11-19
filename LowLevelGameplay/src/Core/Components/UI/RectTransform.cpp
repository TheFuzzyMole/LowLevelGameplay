#include <Core/Components/UI/RectTransform.h>
#include <Core/Components/UI/Canvas.h>
#include <Core/GameObject.h>
#include <Core/Commons.h>
#include <Utils/Debug.h>

namespace LLGP
{
	RectTransform::RectTransform(GameObject* owner, Transform* parent, Vector2f inPos) : Transform(owner, parent, inPos)
	{
		if (parent)
		{
			if (LLGP::RectTransform* parentRect = dynamic_cast<LLGP::RectTransform*>(parent))
			{
				m_Canvas = const_cast<LLGP::Canvas*>(parentRect->GetCanvasRef());
			}
			else
			{
				m_Canvas = _GameObject->GetComponent<LLGP::Canvas>();
			}

			Debug::Assert(m_Canvas != nullptr, "No Canvas Component added to base RectTransform", _GameObject);
		}
	}
	const LLGP::Vector2f& RectTransform::GetRenderSize()
	{
		if (m_IsDirty) { CleanTransform(); }
		return m_LocalRenderSize;
	}

	void RectTransform::SetTransformAlongAxis(int axis, const LLGP::Vector2f& _anchors, const LLGP::Vector2f& _offsets)
	{
		switch (axis)
		{
		case 0:
			m_AnchorMin.x = _anchors.x;
			m_AnchorMax.x = _anchors.y;
			m_OffsetMin.x = _offsets.x;
			m_OffsetMax.x = _offsets.y;
			m_IsBeingControlled |= 0b10101010;
			break;
		case 1:
			m_AnchorMin.y = _anchors.x;
			m_AnchorMax.y = _anchors.y;
			m_OffsetMin.y = _offsets.x;
			m_OffsetMax.y = _offsets.y;
			m_IsBeingControlled |= 0b01010101;
			break;
		default:
			Debug::LogWarning("trying to set UI rect transform size along axis " + axis, _GameObject);
			break;
		}
	}
	void RectTransform::ReleaseControl(uint8_t releaseMask)
	{
		m_IsBeingControlled &= ~releaseMask; //reverse the release mask and use it as a mask
	}
	void RectTransform::SetIgnoreControl(uint8_t ignoreMask)
	{
		m_IsIgnoringControl = ignoreMask;
	}
	bool RectTransform::IsBeingControlled(uint8_t controlMask) const
	{
		return ((m_IsBeingControlled & ~m_IsIgnoringControl) & controlMask) == controlMask;
	}
	const LLGP::Canvas* RectTransform::GetCanvasRef() const
	{
		return m_Canvas;
	}
	void RectTransform::CleanTransform()
	{
		LLGP::Transform::CleanTransform();

		m_LocalRenderSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
		if (m_Parent)
		{
			if(LLGP::RectTransform* parentRect = dynamic_cast<LLGP::RectTransform*>(m_Parent))
			{
				m_LocalRenderSize = parentRect->GetRenderSize();
			}
		}

		m_LocalRenderSize =
			{fmaxf((m_LocalRenderSize.x * (m_AnchorMax.x - m_AnchorMin.x)) - (m_OffsetMin.x + m_OffsetMax.x), 0.f),
			 fmaxf((m_LocalRenderSize.y * (m_AnchorMax.y - m_AnchorMin.y)) - (m_OffsetMin.y + m_OffsetMax.y), 0.f) };

		OnTransformSizeChanged();
	}
}

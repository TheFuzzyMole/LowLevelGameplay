#pragma once
#include <Core/Components/Transform.h>
#include <Core/Event.h>

namespace LLGP
{
	class Canvas;

	class RectTransform : public Transform
	{
	public:
		RectTransform(GameObject* owner, Transform* parent = nullptr, Vector2f inPos = Vector2f::zero);
		
		LLGP::Event<> OnTransformSizeChanged;
		
		const LLGP::Vector2f& GetRenderSize();
		void SetTransformAlongAxis(int axis, const LLGP::Vector2f& _anchors, const LLGP::Vector2f& _offsets);
		/// <summary>
		/// release control of aspects of the transform, should be called from a layout group only
		/// </summary>
		/// <param name="releaseMask">packed boolean flags for (anchor(min|max)|offset(min|max))</param>
		void ReleaseControl(uint8_t releaseMask);
		/// <summary>
		/// Set the ignore control override mask
		/// </summary>
		/// <param name="ignoreMask">packed boolean flags for (anchor(min|max)|offset(min|max))</param>
		void SetIgnoreControl(uint8_t ignoreMask);
		/// <summary>
		/// Check if the aspects in the mask are being controlled by a layour group or not
		/// </summary>
		/// <param name="controlMask">packed boolean flags for (anchor(min|max)|offset(min|max))</param>
		bool IsBeingControlled(uint8_t controlMask) const;

		const LLGP::Canvas* GetCanvasRef() const;

		virtual void CleanTransform() override;

	private:
		uint8_t m_IsBeingControlled;
		uint8_t m_IsIgnoringControl;

		LLGP::Vector2f m_OffsetMin;
		LLGP::Vector2f m_OffsetMax;
		LLGP::Vector2f m_AnchorMin;
		LLGP::Vector2f m_AnchorMax;

		LLGP::Vector2f m_LocalRenderSize;

		LLGP::Canvas* m_Canvas;
	};
}

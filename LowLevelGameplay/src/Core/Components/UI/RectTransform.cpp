#include <Core/Components/UI/RectTransform.h>
#include <Core/Components/UI/Canvas.h>
#include <Core/Components/UI/LayoutElement.h>
#include <Core/Components/Camera.h>
#include <Core/GameObject.h>
#include <Core/Commons.h>
#include <Utils/Debug.h>

namespace LLGP
{
	RectTransform::RectTransform(GameObject* owner, Transform* parent, Vector2f inPos) : Transform(owner, parent, inPos), m_Canvas(nullptr), m_LayoutElement(nullptr), m_IsBeingControlled(0)
	{}

	const LLGP::Vector2f& RectTransform::GetRenderSize()
	{
		if (m_IsDirty) { CleanTransform(); }
		return m_LocalRenderSize;
	}

	void RectTransform::SetTransformAlongAxis(int axis, const LLGP::Vector2f& _anchors, const LLGP::Vector2f& _offsets, const LLGP::Vector2f& _parentSize)
	{
		float totalSpace = 0.f;
		float actualSpace = 0.f;

		switch (axis)
		{
		case 0:
			m_AnchorMin.x = _anchors.x;
			m_AnchorMax.x = _anchors.y;
			m_OffsetMin.x = _offsets.x;
			m_OffsetMax.x = _offsets.y;
			m_IsBeingControlled |= 0b10101010;
			totalSpace = _parentSize.x * (m_AnchorMax.x - m_AnchorMin.x);
			actualSpace = totalSpace - (m_OffsetMin.x + m_OffsetMax.x);
			break;
		case 1:
			m_AnchorMin.y = _anchors.x;
			m_AnchorMax.y = _anchors.y;
			m_OffsetMin.y = _offsets.x;
			m_OffsetMax.y = _offsets.y;
			m_IsBeingControlled |= 0b01010101;
			totalSpace = _parentSize.y * (m_AnchorMax.y - m_AnchorMin.y);
			actualSpace = totalSpace - (m_OffsetMin.y + m_OffsetMax.y);
			break;
		default:
			Debug::LogWarning("trying to set UI rect transform size along axis " + axis, _GameObject);
			break;
		}

		SetAsDirty();
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
	LLGP::Canvas& RectTransform::GetCanvasRef() const
	{
		return *m_Canvas;
	}
	void RectTransform::CleanTransform()
	{
		LLGP::Transform::CleanTransform();

		LLGP::Vector2f parentSize = { SCREEN_WIDTH, SCREEN_HEIGHT };
		LLGP::Vector2f baseOffset = -parentSize / 2.f;
		if (m_Parent)
		{
			if(LLGP::RectTransform* parentRect = dynamic_cast<LLGP::RectTransform*>(m_Parent))
			{
				parentSize = parentRect->GetRenderSize();
				baseOffset = LLGP::Vector2f::zero;
			}
		}

		LLGP::Vector2f overshoot(
			(m_LayoutElement && m_LayoutElement->HasMinWidth() ? fmaxf(m_LayoutElement->GetMinWidth() - ((parentSize.x * (m_AnchorMax.x - m_AnchorMin.x)) - (m_OffsetMax.x + m_OffsetMin.x)), 0.f) : 0.f),
			(m_LayoutElement && m_LayoutElement->HasMinHeight() ? fmaxf(m_LayoutElement->GetMinHeight() - ((parentSize.y * (m_AnchorMax.y - m_AnchorMin.y)) - (m_OffsetMax.y + m_OffsetMin.y)), 0.f) : 0.f)
		);

		LLGP::Vector2f ratioDenominators = m_AnchorMin + (LLGP::Vector2f::one - m_AnchorMax);

		m_OffsetMin.x -= overshoot.x * (ratioDenominators.x == 0 ? 0.5f : m_AnchorMin.x / ratioDenominators.x);
		m_OffsetMax.x -= overshoot.x * (ratioDenominators.x == 0 ? 0.5f : (1.f - m_AnchorMax.x) / ratioDenominators.x);
		m_OffsetMin.y -= overshoot.y * (ratioDenominators.y == 0 ? 0.5f : m_AnchorMin.y / ratioDenominators.y);
		m_OffsetMax.y -= overshoot.y * (ratioDenominators.y == 0 ? 0.5f : (1.f - m_AnchorMax.y) / ratioDenominators.y);

		m_LocalRenderSize =
			{fmaxf((parentSize.x * (m_AnchorMax.x - m_AnchorMin.x)) - (m_OffsetMin.x + m_OffsetMax.x), 0.f),
			 fmaxf((parentSize.y * (m_AnchorMax.y - m_AnchorMin.y)) - (m_OffsetMin.y + m_OffsetMax.y), 0.f) };

		SetLocalPosition(baseOffset + LLGP::Vector2f(
			(parentSize.x * ((m_AnchorMax.x + m_AnchorMin.x) / 2.f) - 0.5f) + ((m_OffsetMin.x - m_OffsetMax.x) / 2.f),
			(parentSize.y * ((m_AnchorMax.y + m_AnchorMin.y) / 2.f) - 0.5f) + ((m_OffsetMin.y - m_OffsetMax.y) / 2.f)
			));


		OnTransformSizeChanged(m_LocalRenderSize);
	}
	void RectTransform::Start()
	{
		if (m_Parent)
		{
			if (LLGP::RectTransform* parentRect = dynamic_cast<LLGP::RectTransform*>(m_Parent))
			{
				m_Canvas = &parentRect->GetCanvasRef();
			}
		}

		if (m_Canvas == nullptr)
		{
			m_Canvas = _GameObject->GetComponent<LLGP::Canvas>();
		}

		m_LayoutElement = _GameObject->GetComponent<LLGP::LayoutElement>();
		LLGP::RectTransform* rectParent = nullptr;
		if (m_Parent) { rectParent = dynamic_cast<LLGP::RectTransform*>(m_Parent); }

		SetAsDirty();

		Debug::Assert(m_Canvas != nullptr, "No Canvas Component added to base RectTransform", _GameObject);
	}
	void RectTransform::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "RectTransform" << YAML::Value << YAML::BeginMap;
		out << YAML::Key << "UUID" << YAML::Value << uuid;

		out << YAML::Key << "Position" << YAML::Value << GetLocalPosition();
		out << YAML::Key << "AnchorMin" << YAML::Value << m_AnchorMin;
		out << YAML::Key << "AnchorMax" << YAML::Value << m_AnchorMax;
		out << YAML::Key << "OffsetMin" << YAML::Value << m_OffsetMin;
		out << YAML::Key << "OffsetMax" << YAML::Value << m_OffsetMax;

		if (m_Parent)
		{
			out << YAML::Key << "Parent" << YAML::Value << m_Parent->uuid;
		}

		out << YAML::EndMap;
	}
	bool RectTransform::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		if (!node["Position"] || !node["AnchorMin"] || !node["AnchorMax"] || !node["OffsetMin"] || !node["OffsetMax"]) { return false; }
		m_LocalPosition = node["Position"].as<LLGP::Vector2f>();
		m_AnchorMin = node["AnchorMin"].as<LLGP::Vector2f>();
		m_AnchorMax = node["AnchorMax"].as<LLGP::Vector2f>();
		m_OffsetMin = node["OffsetMin"].as<LLGP::Vector2f>();
		m_OffsetMax = node["OffsetMax"].as<LLGP::Vector2f>();

		if (node["Parent"])
		{
			linkRequests.emplace_back(std::bind(
				[](LLGP::Transform* context, LLGP::Component* parent)
					{context->SetParent(dynamic_cast<LLGP::Transform*>(parent), false); },
				this, std::placeholders::_1),
				node["Parent"].as<uint64_t>()
			);
		}
		else
		{
			m_Parent = nullptr;
		}
		SetAsDirty();
		return true;
	}
}

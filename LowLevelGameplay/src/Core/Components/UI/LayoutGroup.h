#pragma once
#include <Core/Components/Component.h>

namespace LLGP
{
	class RectTransform;

	enum LayoutAlignment
	{
		TOPLEFT,
		TOPCENTER,
		TOPRIGHT,
		MIDDLELEFT,
		MIDDLECENTER,
		MIDDLERIGHT,
		BOTTOMLEFT,
		BOTTOMCENTER,
		BOTTOMRIGHT
	};

	class LayoutGroup : public Component
	{
	public:
		LayoutGroup(GameObject* owner);

		virtual void UpdateChildSizeAndPosition(const LLGP::Vector2f& newSize);

		virtual void Serialize(YAML::Emitter& out) override = 0;
		virtual bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override = 0;

	protected:
		LLGP::RectTransform* m_Transform;
		LLGP::LayoutAlignment m_ChildAlignment;
		
		const LLGP::Vector2f AlignmentAnchors[18] = {
		{0.0f, 0.0f}, {0.0f, 0.0f},
		{0.5f, 0.5f}, {0.0f, 0.0f},
		{1.0f, 1.0f}, {0.0f, 0.0f},
		{0.0f, 0.0f}, {0.5f, 0.5f},
		{0.5f, 0.5f}, {0.5f, 0.5f},
		{1.0f, 1.0f}, {0.5f, 0.5f},
		{0.0f, 0.0f}, {1.0f, 1.0f},
		{0.5f, 0.5f}, {1.0f, 1.0f},
		{1.0f, 1.0f}, {1.0f, 1.0f},
		};
	};
}

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

		virtual void Serialize(YAML::Emitter& out) override = 0;
		virtual bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override = 0;

	protected:
		LLGP::RectTransform* m_Transform;
	};
}

#pragma once
#include <Core/Components/Component.h>

namespace LLGP
{
	class Transform;

	class Canvas : public Component
	{
	public:
		Canvas(LLGP::GameObject* owner);

	private:
		LLGP::Transform* m_Camera;
		LLGP::Vector2f m_ReferenceScreenSize;
	};
}

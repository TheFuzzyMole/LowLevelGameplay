#include <Core/GameObject.h>

#include <Core/Component.h>
#include <Core/Transform.h>

namespace LLGP
{
	GameObject::GameObject() : Object()
	{
		transform = this->AddComponent<Transform>();
	}

	void GameObject::SetActive(bool newActive)
	{
		if (m_Active == newActive) return;

		m_Active = newActive;
		for (int i = 0; i < m_Components.size(); i++)
		{
			m_Components[i].get()->OwnerActiveChange(m_Active);
		}
	}
}

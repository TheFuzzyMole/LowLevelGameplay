#include <Core/GameObject.h>

#include <Core/Components/Component.h>
#include <Core/Components/Components.h>
#include <Core/Scene.h>

namespace LLGP
{
	GameObject::GameObject(LLGP::Scene& _owningScene, const std::string& name) : m_Name(name), OwningScene(&_owningScene)
	{
		transform = this->AddComponent<Transform>();
		_IsPendingKill = false;
		OnWorldUpdate.AddListener(this, [&]() { this->OnUpdate(); });
		OnWorldFixedUpdate.AddListener(this, [&]() { this->OnFixedUpdate(); });
	}

	GameObject::~GameObject()
	{
		OnWorldUpdate.RemoveListener(this, [&]() { this->OnUpdate(); });
		OnWorldFixedUpdate.RemoveListener(this, [&]() { this->OnFixedUpdate(); });
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

	void GameObject::Destroy()
	{
		_IsPendingKill = true;
		OnDestroy();
	}
}

#include <Core/GameObject.h>

#include <Core/Component.h>
#include <Core/Transform.h>

namespace LLGP
{
	std::vector<GameObject*> GameObject::s_PendingKillList;
	LLGP::Event<> GameObject::OnUpdate;
	LLGP::Event<> GameObject::OnFixedUpdate;

	GameObject::GameObject() : Object()
	{
		transform = this->AddComponent<Transform>();
		_IsPendingKill = false;
		OnUpdate.AddListener(this, std::bind(&GameObject::UpdateComponents, this));
		OnFixedUpdate.AddListener(this, std::bind(&GameObject::FixedUpdateComponents, this));
	}

	GameObject::~GameObject()
	{
		OnUpdate.RemoveListener(this, std::bind(&GameObject::UpdateComponents, this));
		OnFixedUpdate.RemoveListener(this, std::bind(&GameObject::FixedUpdateComponents, this));
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

	void GameObject::StartComponents()
	{
		for (std::unique_ptr<LLGP::Component>& c : m_Components)
		{
			c.get()->Start();
		}
	}

	void GameObject::UpdateComponents()
	{
		for (std::unique_ptr<LLGP::Component>& c : m_Components)
		{
			c.get()->Update();
		}
	}

	void GameObject::FixedUpdateComponents()
	{
		for (std::unique_ptr<LLGP::Component>& c : m_Components)
		{
			c.get()->FixedUpdate();
		}
	}
	void GameObject::Destroy()
	{
		_IsPendingKill = true;
		if (std::find_if(s_PendingKillList.begin(), s_PendingKillList.end(),
			[this](GameObject* go) {return go == this; }) == s_PendingKillList.end())
		{
			s_PendingKillList.push_back(this);
		}
	}
}

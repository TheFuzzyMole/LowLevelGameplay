#include "Core\GameObject.h"

#include "Core\Component.h"
#include "Core\Transform.h"

namespace LLGP::Core
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

	template<class T> requires isComponent<T>
	T* GameObject::GetComponent()
	{
		T* returnComp = nullptr;
		for (int i = 0; i < m_Components.size(); i++)
		{
			returnComp = dynamic_cast<T*>(m_Components[i].get());
			if (returnComp != nullptr)
			{
				break;
			}
		}

		return returnComp;
	}

	template<class T> requires isComponent<T>
	T* GameObject::AddComponent()
	{
		std::unique_ptr<Component> newComp = std::make_unique<T>(this);
		m_Components.push_back(std::move(newComp));
		return dynamic_cast<T*>(m_Components[m_Components.size()].get());
	}

	template<class T> requires isComponent<T>
	bool GameObject::RemoveComponent(T* comp)
	{
		std::vector<LLGP::Core::Component*>::iterator item = std::find(m_Components.begin(), m_Components.end(), comp);
		if (item == m_Components.end()) return false;
		int index = std::distance(m_Components.begin(), item);
		m_Components.erase(item);
		//TODO figure out if there is more that needs doing here
	}
}

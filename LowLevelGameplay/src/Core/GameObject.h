#pragma once
#include <Core/Object.h>
#include <vector>
#include <memory>
#include <string>
#include <Core/component_concept.h>
#include <Core/Physics.h>
#include <Core/Collider.h>
#include <Core/Event.h>

namespace LLGP
{
	class Transform;

	class GameObject : public Object
	{
	public:
		GameObject();
		GameObject(const GameObject&) = default;
		~GameObject();

		Transform* transform;
		LLGP::Event<Collision*> OnCollisionEnter;
		LLGP::Event<Collision*> OnCollisionStay;
		LLGP::Event<Collision*> OnCollisionExit;

		inline void SetName(std::string newName) { m_Name = newName; }
		inline std::string GetName() { return m_Name; }
		void SetActive(bool newActive);
		inline bool GetActive() { return m_Active; }
		inline void SetTag(std::string newTag) { m_Tag = newTag; }
		inline bool CompareTag(std::string comp) { return m_Tag == comp; }

		template<class T> requires isComponent<T>
		T* GetComponent()
		{
			T* returnComp = nullptr;
			for (int i = 0; i < m_Components.size(); i++)
			{
				if(returnComp = dynamic_cast<T*>(m_Components[i].get()))
				{
					return returnComp;
				}
			}

			return nullptr;
		}
		template<class T> requires isComponent<T>
		std::vector<T*> GetComponents()
		{
			std::vector<T*> toReturn;
			T* returnComp = nullptr;
			for (int i = 0; i < m_Components.size(); i++)
			{
				if (returnComp = dynamic_cast<T*>(m_Components[i].get()))
				{
					toReturn.push_back(returnComp);
				}
			}
			return toReturn;
		}
		template<class T> requires isComponent<T>
		T* AddComponent()
		{
			//std::unique_ptr<Component> newComp = std::make_unique<T>(this);
			m_Components.push_back(std::make_unique<T>(this));
			return dynamic_cast<T*>(m_Components[m_Components.size()-1].get());
		}
		template<class T> requires isComponent<T>
		bool RemoveComponent(T* comp)
		{
			T* castedType = nullptr;
			for (int i = 0; i < m_Components.size(); i++)
			{
				if (castedType = dynamic_cast<T*>(m_Components[i].get()))
				{
					if (*castedType == *comp)
					{
						m_Components[i].reset();
						m_Components.erase(std::next(m_Components.begin(), i));
						return true;
					}
				}
			}
			return false;
		}
	
	private:
		std::string m_Name;
		bool m_Active;
		std::string m_Tag;
		std::vector<std::unique_ptr<Component>> m_Components;
	};
}

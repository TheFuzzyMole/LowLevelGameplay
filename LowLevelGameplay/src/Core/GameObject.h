#pragma once
#include <Core/Object.h>
#include <Core/ISerializable.h>
#include <vector>
#include <memory>
#include <string>
#include <Core/component_concept.h>
#include <Core/Physics.h>
#include <Core/Components/Collider.h>
#include <Core/Event.h>

namespace LLGP
{
	class Transform;
	class Scene;

	class GameObject : public Object
	{
	public:
		GameObject(LLGP::Scene& _owningScene, const std::string& name = "");
		GameObject(const GameObject&) = default;
		~GameObject();

#pragma region static events
		static LLGP::Event<> OnWorldUpdate;
		static LLGP::Event<> OnWorldFixedUpdate;
#pragma endregion

#pragma region instanced events
		//these exist as functions would just be a passthrough anyway
		//and this lets the handlers in components be protected to block manual calling
		LLGP::Event<Collision*> OnCollisionEnter;
		LLGP::Event<Collision*> OnCollisionStay;
		LLGP::Event<Collision*> OnCollisionExit;

		LLGP::Event<> OnStart;
		LLGP::Event<> OnUpdate;
		LLGP::Event<> OnFixedUpdate;
		LLGP::Event<> OnDestroy;
#pragma endregion

		bool _IsPendingKill;
		LLGP::Transform* transform;
		LLGP::Scene* OwningScene;

#pragma region getters + setters
		inline void SetName(std::string newName) { m_Name = newName; }
		inline std::string GetName() { return m_Name; }
		void SetActive(bool newActive);
		inline bool GetActive() { return m_Active; }
		inline void SetTag(std::string newTag) { m_Tag = newTag; }
		inline bool CompareTag(std::string comp) { return m_Tag == comp; }
		inline std::string GetTag() { return m_Tag; }
#pragma endregion

		void Destroy();

#pragma region component utilities
		template<class T> requires isComponent<T>
		T* GetComponent()
		{
			T* returnComp = nullptr;
			for (int i = 0; i < m_Components.size(); i++)
			{
				if (returnComp = dynamic_cast<T*>(m_Components[i].get()))
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
			m_Components.push_back(std::make_unique<T>(this));
			return dynamic_cast<T*>(m_Components.back().get());
		}
		template<class T> requires isComponent<T>
		bool RemoveComponent(T* comp)
		{
			T* castedType = nullptr;
			for (int i = 0; i < m_Components.size(); i++)
			{
				if (castedType = dynamic_cast<T>(m_Components[i].get()))
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
		template<class T> requires isComponent<T>
		bool HasComponent()
		{
			return std::find_if(m_Components.begin(), m_Components.end(),
				[](std::unique_ptr<LLGP::Component>& comp)
				{
					return dynamic_cast<T*>(comp.get());
				})
				!= m_Components.end();
		}
#pragma endregion

	private:
		std::string m_Name;
		bool m_Active;
		std::string m_Tag;
		std::vector<std::unique_ptr<Component>> m_Components;

		friend class SceneSerializer;
	};
}

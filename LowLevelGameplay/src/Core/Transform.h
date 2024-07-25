#pragma once

#include <Core\Component.h>
#include <Core\Vector2.h>
#include <string>
#include <vector>

namespace LLGP
{
	class Transform : public Component
	{
	public:
		static std::vector<Transform*> s_DirtyList;

		static void CleanUpDirtyList();

		Transform(GameObject* owner, Transform* parent = nullptr, Vector2f inPos = Vector2f::zero);
		~Transform() = default;

		Transform* GetChild(int index);
		Transform* GetChild(std::string name);
		std::vector<Transform> GetChildren();
		int GetChildCount();
		int GetIndexOfChild(Transform* child);

		void SetNewChild(Transform* newChild);
		void RemoveChild(Transform* oldChild);

		Transform* GetRoot();

		Transform* GetParent();
		int GetSiblingIndex();

		void SetParent(Transform* parent, bool worldTransformStays = true);

		LLGP::Vector2f GetPosition();
		LLGP::Vector2f GetLocalPosition();

		void SetPosition(LLGP::Vector2f newPosition);
		void ChangePosition(LLGP::Vector2f changeInPosition);
		void SetLocalPosition(LLGP::Vector2f newLocalPosition);
		void ChangeLocalPosition(LLGP::Vector2f changeInLocalPosition);

		virtual void OwnerActiveChanged(bool newActive);

		void CleanTransform();
		void SetAsDirty();

		void Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override;

	private:
		bool m_IsDirty;

		Transform* m_Root;
		Transform* m_Parent;
		std::vector<Transform*> m_Children;

		LLGP::Vector2f m_Position;
		LLGP::Vector2f m_LocalPosition;
	};
}

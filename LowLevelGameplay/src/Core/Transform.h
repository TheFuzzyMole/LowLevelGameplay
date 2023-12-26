#pragma once

#include <Core\Component.h>
#include <Core\Vector3.h>
#include <string>
#include <vector>

namespace LLGP::Core
{
	class Transform : public Component
	{
	public:
		static std::vector<Transform*> s_DirtyList;

		static void CleanUpDirtyList();

		Transform(GameObject* owner, Transform* parent = nullptr,
			Vector3d inPos = Vector3d::zero, double inRot = 0.0, Vector2d inScale = Vector2d::zero);
		virtual ~Transform();

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

		LLGP::Vector3d GetPosition();
		LLGP::Vector3d GetLocalPosition();
		LLGP::Vector2d GetScale();
		LLGP::Vector2d GetLocalScale();
		double GetRotation();
		double GetLocalRotation();

		void SetPosition(LLGP::Vector3d newPosition);
		void ChangePosition(LLGP::Vector3d changeInPosition);
		void SetLocalPosition(LLGP::Vector3d newLocalPosition);
		void ChangeLocalPosition(LLGP::Vector3d changeInLocalPosition);
		void SetScale(LLGP::Vector2d newScale);
		void ChangeScale(LLGP::Vector2d changeInScale);
		void SetLocalScale(LLGP::Vector2d newLocalScale);
		void ChangeLocalScale(LLGP::Vector2d changeInLocalScale);
		void SetRotation(double newRotation);
		void ChangeRotation(double changeInRotation);
		void SetLocalRotation(double newLocalRotation);
		void ChangeLocalRotation(double changeInLocalRotation);

		virtual void OwnerActiveChanged(bool newActive);

		Vector3d GetUpVector();
		Vector3d GetRightVector();

		void CleanTransform();
		void SetAsDirty();

	private:
		bool m_IsDirty;

		Transform* m_Root;
		Transform* m_Parent;
		std::vector<Transform*> m_Children;

		LLGP::Vector3d m_Position;
		LLGP::Vector2d m_Scale;
		double m_Rotation;

		LLGP::Vector3d m_LocalPosition;
		LLGP::Vector2d m_LocalScale;
		double m_LocalRotation;
	};
}

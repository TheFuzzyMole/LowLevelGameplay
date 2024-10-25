#pragma once

#include <Core/Components/Component.h>
#include <Core/Maths/Vector2.h>
#include <Core/Maths/Matrix3x3.h>
#include <string>
#include <vector>

namespace LLGP
{
	class Transform : public Component
	{
	public:
		static inline std::vector<Transform*> s_DirtyList;

		static void CleanUpDirtyList();

		Transform(GameObject* owner, Transform* parent = nullptr, Vector2f inPos = Vector2f::zero);
		~Transform() = default;

		Transform* GetChild(int index);
		Transform* GetChild(std::string name);
		std::vector<LLGP::Transform*>& GetChildren();
		int GetChildCount();
		int GetIndexOfChild(Transform* child);

		void SetNewChild(Transform* newChild);
		void RemoveChild(Transform* oldChild);

		Transform* GetRoot();

		Transform* GetParent();
		int GetSiblingIndex();

		void SetParent(Transform* parent, bool worldTransformStays = true);

		LLGP::Vector2f GetPosition();
		const LLGP::Vector2f& GetLocalPosition();
		float GetRotation();
		const float& GetLocalRotation();
		LLGP::Vector2f GetScale();
		const LLGP::Vector2f& GetLocalScale();

		const LLGP::Mat3f& GetL2WMatrix();

		LLGP::Vector2f TransformDirection(const LLGP::Vector2f& in);
		LLGP::Vector2f TransformVector(const LLGP::Vector2f& in);
		LLGP::Vector2f TransformPoint(const LLGP::Vector2f& in);
		LLGP::Vector2f InverseTransformDirection(const LLGP::Vector2f& in);
		LLGP::Vector2f InverseTransformVector(const LLGP::Vector2f& in);
		LLGP::Vector2f InverseTransformPoint(const LLGP::Vector2f& in);

		void SetPosition(LLGP::Vector2f newPosition);
		void ChangePosition(LLGP::Vector2f changeInPosition);
		void SetLocalPosition(LLGP::Vector2f newLocalPosition);
		void ChangeLocalPosition(LLGP::Vector2f changeInLocalPosition);

		void SetRotation(float newRotation);
		void ChangeRotation(float changeInRotation);
		void SetLocalRotation(float newLocalRotation);
		void ChangeLocalRotation(float changeInLocalRotation);

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

		LLGP::Vector2f m_LocalPosition;
		float m_LocalRotation;
		LLGP::Vector2f m_LocalScale;

		LLGP::Mat3f m_L2WMatrix;
	};
}

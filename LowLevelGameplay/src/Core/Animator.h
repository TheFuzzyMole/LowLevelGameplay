#pragma once
#include <Core/Component.h>
#include <Core/Event.h>
#include <memory>
#include <string>

namespace LLGP
{
	class Animation;
	class Renderer;

	class Animator : public Component
	{
	public:
		Animator(GameObject* owner);
		Animator(GameObject* owner, YAML::Node inData);

		LLGP::Event<> OnAnimationFinished;

		void Start() override;
		void Update() override;

		void SetAnimation(std::string path);
		void SetAnimation(std::shared_ptr<LLGP::Animation> assetRef);

		void Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node node) override;
	private:
		float m_Progress;
		float m_NextTime;
		LLGP::Renderer* m_Renderer;
		std::shared_ptr<LLGP::Animation> m_CurAnimation;
	};
}

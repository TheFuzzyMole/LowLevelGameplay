#pragma once
#include <Core/Component.h>
#include <unordered_map>
#include <string>
#include <memory>
#include <functional>
#include <variant>

namespace LLGP
{
	class Animation;
	class Animator;

	class AnimatorController : public Component
	{
		struct Node
		{
			std::string _path;
			std::shared_ptr<LLGP::Animation> _Anim;
		};
	public:
		AnimatorController(GameObject* owner);

		void Update() override;

		void AddAnimation(std::string path);
		void AddTransition(std::string animationPath, std::function<bool(std::variant<bool, float>)> pred);
	private:
		std::unordered_map <std::string, std::shared_ptr<LLGP::Animation>> m_Animations;
		std::string m_CurAnimation;
		LLGP::Animator* m_Animator;
	};
}

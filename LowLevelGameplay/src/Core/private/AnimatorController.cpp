#include <Core/AnimatorController.h>

namespace LLGP
{
	AnimatorController::AnimatorController(GameObject* owner) : Component(owner)
	{

	}

	void AnimatorController::Update()
	{

	}

	void AnimatorController::AddAnimation(std::string path)
	{
	}

	void AnimatorController::AddTransition(std::string animationPath, std::function<bool(std::variant<bool, float>)> pred)
	{
	}
}

#pragma once
#include <Core/Asset.h>
#include <Core/Vector2.h>
#include <vector>

namespace LLGP
{
	struct KeyFrame
	{
		KeyFrame()
		{
			Time = 0.f;
			SpriteIndex = LLGP::Vector2u::zero;
		}
		KeyFrame(float _Time, LLGP::Vector2u _index)
		{
			Time = _Time;
			SpriteIndex = _index;
		}
		float Time;
		LLGP::Vector2u SpriteIndex;
	};

	class Animation : public Asset
	{
	public:
		Animation(std::string path);

		KeyFrame GetNextKeyFrame(float curTime);
		KeyFrame GetNextKeyFrame(KeyFrame& last);
		KeyFrame GetPreviousKeyFrame(float curTime);
		float GetAnimationLength();

	private:
		std::vector<KeyFrame> m_Data;
		int m_LastQueryIndex;
	};
}

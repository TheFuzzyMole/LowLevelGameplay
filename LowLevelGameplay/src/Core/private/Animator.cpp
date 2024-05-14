#include <Core/Animator.h>
#include <Core/GameObject.h>
#include <Core/Animation.h>
#include <Core/Renderer.h>
#include <Core/Time.h>
#include <Core/AssetManager.h>
#include <cassert>

namespace LLGP
{
	Animator::Animator(GameObject* owner) : Component(owner)
	{
		m_Renderer = _GameObject->GetComponent<LLGP::Renderer>();
	}

	void Animator::Start()
	{
		if (m_Renderer == nullptr) { m_Renderer = _GameObject->GetComponent<LLGP::Renderer>(); }
		assert(m_Renderer != nullptr);
	}

	void Animator::Update()
	{
		if (!m_CurAnimation) return;

		m_Progress += LLGP::Time::deltaTime;
		if (m_Progress >= m_NextTime)
		{
			if (m_Progress > m_CurAnimation->GetAnimationLength())
			{
				m_Progress -= m_CurAnimation->GetAnimationLength();
				OnAnimationFinished();
			}
			m_Renderer->SetupSpriteUV(m_CurAnimation->GetPreviousKeyFrame(m_Progress).SpriteIndex);
			m_NextTime = m_CurAnimation->GetNextKeyFrame(m_Progress).Time;
		}
	}

	void Animator::SetAnimation(std::string path)
	{
		m_CurAnimation = LLGP::AssetManager::GetAsset<LLGP::Animation>(path);
		assert(m_CurAnimation);
		m_Progress = 0.f;
		m_NextTime = m_CurAnimation->GetNextKeyFrame(m_Progress).Time;
		m_Renderer->SetupSpriteUV(m_CurAnimation->GetPreviousKeyFrame(m_Progress).SpriteIndex);
	}

	void Animator::SetAnimation(std::shared_ptr<LLGP::Animation> assetRef)
	{
		m_CurAnimation = assetRef;
		m_Progress = 0.f;
		m_NextTime = m_CurAnimation->GetNextKeyFrame(m_Progress).Time;
		m_Renderer->SetupSpriteUV(m_CurAnimation->GetPreviousKeyFrame(m_Progress).SpriteIndex);
	}
}

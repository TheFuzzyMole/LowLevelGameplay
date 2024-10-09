#pragma once
#include <Core/Components/Component.h>

namespace LLGP
{
	class Camera : public Component
	{
	public:
		Camera(LLGP::GameObject* owner);

		static inline LLGP::Camera* main;

		void Serialize(YAML::Emitter& out) override;
		bool Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests) override;
	};
}

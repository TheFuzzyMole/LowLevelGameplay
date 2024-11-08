#include "Camera.h"
#include <Utils/Debug.h>

namespace LLGP
{
	Camera::Camera(LLGP::GameObject* owner) : Component(owner)
	{
		if (main == nullptr) { main = this; }
		else { Debug::LogError("too many camera components in the scene"); }
	}

	void Camera::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap; //Camera
		out << YAML::Key << "UUID" << YAML::Value << uuid;

		out << YAML::EndMap; //Camera
	}

	bool Camera::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		return true;
	}
}

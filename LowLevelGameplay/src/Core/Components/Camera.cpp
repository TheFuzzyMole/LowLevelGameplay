#include "Camera.h"
#include <iostream>

namespace LLGP
{
	Camera::Camera(LLGP::GameObject* owner) : Component(owner)
	{
		if (main == nullptr) { main = this; }
		else { std::cout << "ERROR: too many camera components in the scene" << std::endl; }
	}

	void Camera::Serialize(YAML::Emitter& out)
	{
		out << YAML::Key << "Camera" << YAML::Value << YAML::BeginMap; //Camera
		out << YAML::Key << "UUID" << YAML::Value << uuid;

		out << YAML::EndMap; //Camera
	}

	bool Camera::Deserialize(YAML::Node node, std::vector<LinkRequest>& linkRequests)
	{
		return false;
	}
}

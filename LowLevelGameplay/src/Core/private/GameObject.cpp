#include <Core/GameObject.h>

#include <Core/Component.h>
#include <Core/Transform.h>

namespace LLGP
{
	LLGP::Event<> GameObject::OnWorldUpdate;
	LLGP::Event<> GameObject::OnWorldFixedUpdate;

	GameObject::GameObject(const std::string& name) : m_Name(name)
	{
		transform = this->AddComponent<Transform>();
		_IsPendingKill = false;
		OnWorldUpdate.AddListener(this, [&]() { OnUpdate(); });
		OnWorldFixedUpdate.AddListener(this, [&]() { OnFixedUpdate(); });
	}

	GameObject::GameObject(YAML::Node inData) : Object(inData["GameObject"].as<uint64_t>())
	{
		if (!Deserialize(inData)) { std::cout << "Error Deserialising GameObject: " << m_Name << std::endl; }

		_IsPendingKill = false;
		OnWorldUpdate.AddListener(this, [&]() { OnUpdate(); });
		OnWorldFixedUpdate.AddListener(this, [&]() { OnFixedUpdate(); });
	}

	GameObject::~GameObject()
	{
		OnWorldUpdate.RemoveListener(this, [&]() { OnUpdate(); });
		OnWorldFixedUpdate.RemoveListener(this, [&]() { OnFixedUpdate(); });
	}

	void GameObject::SetActive(bool newActive)
	{
		if (m_Active == newActive) return;

		m_Active = newActive;
		for (int i = 0; i < m_Components.size(); i++)
		{
			m_Components[i].get()->OwnerActiveChange(m_Active);
		}
	}

	void GameObject::Destroy()
	{
		_IsPendingKill = true;
		OnDestroy();
	}
	void GameObject::Serialize(YAML::Emitter& out)
	{
		out << YAML::BeginMap; //GameObject
		out << YAML::Key << "GameObject" << YAML::Value << uuid;
		out << YAML::Key << "Name" << YAML::Value << m_Name;
		out << YAML::Key << "Active" << YAML::Value << m_Active;
		out << YAML::Key << "Tag" << YAML::Value << m_Tag;

		out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq;

		for (std::unique_ptr<Component>& c : m_Components)
		{
			c->Serialize(out);
		}

		out << YAML::EndSeq; //Components

		out << YAML::EndMap; //GameObject
	}
	bool GameObject::Deserialize(YAML::Node node)
	{
		m_Name = node["Name"].as<std::string>();
		m_Active = node["Active"].as<bool>();
		m_Tag = node["Tag"].as<std::string>();

		if (YAML::Node components = node["Components"])
		{
			for (YAML::Node compNode : components)
			{
				if (YAML::Node tData = compNode["Transform"])
				{
					m_Components.push_back(std::make_unique<LLGP::Transform>(this, tData));
				}

				if (YAML::Node rbData = compNode["Rigidbody"])
				{
					m_Components.push_back(std::make_unique<LLGP::Rigidbody>(this, rbData));
				}

				if (YAML::Node ccData = compNode["CircleCollider"])
				{

				}

				if (YAML::Node bcData = compNode["BoxCollider"])
				{

				}

				if (YAML::Node aData = compNode["Animator"])
				{

				}

				if (YAML::Node rData = compNode["Renderer"])
				{

				}

				if (YAML::Node pmData = compNode["PlayerMovement"])
				{

				}
			}
		}
		return false;
	}
}

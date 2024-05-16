#include <Core/GameObject.h>

#include <Core/Component.h>
#include <Core/Components.h>
#include <Core/Scene.h>

namespace LLGP
{
	LLGP::Event<> GameObject::OnWorldUpdate;
	LLGP::Event<> GameObject::OnWorldFixedUpdate;

	GameObject::GameObject(LLGP::Scene& _owningScene, const std::string& name) : m_Name(name), OwningScene(&_owningScene)
	{
		transform = this->AddComponent<Transform>();
		_IsPendingKill = false;
		OnWorldUpdate.AddListener(this, [&]() { OnUpdate(); });
		OnWorldFixedUpdate.AddListener(this, [&]() { OnFixedUpdate(); });
	}

	GameObject::GameObject(LLGP::Scene& _owningScene, YAML::Node inData) : Object(inData["GameObject"].as<uint64_t>()), OwningScene(&_owningScene)
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
			out << YAML::BeginMap;
			c->Serialize(out);
			out << YAML::EndMap;
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
					transform = static_cast<LLGP::Transform*>(m_Components[m_Components.size() - 1].get());
				}

				if (YAML::Node rbData = compNode["Rigidbody"])
				{
					m_Components.push_back(std::make_unique<LLGP::Rigidbody>(this, rbData));
				}

				if (YAML::Node ccData = compNode["CircleCollider"])
				{
					m_Components.push_back(std::make_unique<LLGP::CircleCollider>(this, ccData));
				}

				if (YAML::Node bcData = compNode["BoxCollider"])
				{
					m_Components.push_back(std::make_unique<LLGP::BoxCollider>(this, bcData));
				}

				if (YAML::Node aData = compNode["Animator"])
				{
					m_Components.push_back(std::make_unique<LLGP::Animator>(this, aData));
				}

				if (YAML::Node rData = compNode["Renderer"])
				{
					m_Components.push_back(std::make_unique<LLGP::Renderer>(this, rData));
				}

				if (YAML::Node pmData = compNode["PlayerMovement"])
				{
					m_Components.push_back(std::make_unique<TEST::PlayerMovement>(this, pmData));
				}
			}

			OnStart();
			return true;
		}
		return false;
	}
}

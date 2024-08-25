#include <Core/GameObject.h>

#include <Core/Components/Component.h>
#include <Core/Components/Components.h>
#include <Core/Scene.h>

namespace LLGP
{
#define DeserializeComponent(__TYPE, __NAME) if (YAML::Node data = compNode[__NAME]) { m_Components.push_back(std::make_unique<__TYPE>(this, data)); }
#define DeserializeComponentType(__TYPE) { \
	std::string __name = #__TYPE; __name.substr(6, -1); \
	DeserializeComponent(__TYPE, __name)}

	LLGP::Event<> GameObject::OnWorldUpdate;
	LLGP::Event<> GameObject::OnWorldFixedUpdate;

	GameObject::GameObject(LLGP::Scene& _owningScene, const std::string& name) : m_Name(name), OwningScene(&_owningScene)
	{
		transform = this->AddComponent<Transform>();
		_IsPendingKill = false;
		OnWorldUpdate.AddListener(this, [&]() { this->OnUpdate(); });
		OnWorldFixedUpdate.AddListener(this, [&]() { this->OnFixedUpdate(); });
	}

	GameObject::~GameObject()
	{
		OnWorldUpdate.RemoveListener(this, [&]() { this->OnUpdate(); });
		OnWorldFixedUpdate.RemoveListener(this, [&]() { this->OnFixedUpdate(); });
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
	/*void GameObject::Serialize(YAML::Emitter& out)
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
				DeserializeComponent(LLGP::Transform, "Transform")
				DeserializeComponent(LLGP::Rigidbody, "Rigidbody")
				DeserializeComponent(LLGP::CircleCollider, "CircleCollider")
				DeserializeComponent(LLGP::BoxCollider, "BoxCollider")
				DeserializeComponent(LLGP::Animator, "Animator")
				DeserializeComponent(LLGP::Renderer, "Renderer")
				DeserializeComponent(TEST::PlayerMovement, "PlayerMovement")
			}
			transform = static_cast<LLGP::Transform*>(GetComponent<LLGP::Transform>());

			OnStart();
			return true;
		}
		return false;
	}*/
}

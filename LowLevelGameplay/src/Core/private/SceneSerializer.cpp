#include <Core/SceneSerializer.h>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <type_traits>

#include <Core/GameObject.h>
#include <Core/Components.h>
#include <Core/Component.h>
#include <Core/Animation.h>
#include <Core/Texture.h>
#include <Core/AssetManager.h>

namespace LLGP
{
	SceneSerializer::SceneSerializer(LLGP::Scene& const scene)
		: m_Scene(&scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filePath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << m_Scene->m_Name;

		out << YAML::Key << "Textures" << YAML::Value << YAML::BeginSeq; //Textures
		for (std::shared_ptr<LLGP::Asset>& a : m_Scene->m_SceneTextures)
		{
			out << a->GetAssetPath();
		}
		out << YAML::EndSeq; //Textures

		out << YAML::Key << "Animations" << YAML::Value << YAML::BeginSeq; //Animations
		for (std::shared_ptr<LLGP::Asset>& a : m_Scene->m_SceneAnimations)
		{
			out << a->GetAssetPath();
		}
		out << YAML::EndSeq; //Animations

		out << YAML::Key << "GameObjects" << YAML::Value << YAML::BeginSeq;
		for (std::pair<const uint64_t, std::unique_ptr<LLGP::GameObject>>& go : m_Scene->m_SceneObjects)
		{
			out << YAML::BeginMap; //GameObject
			out << YAML::Key << "GameObject" << YAML::Value << go.second->uuid;
			out << YAML::Key << "Name" << YAML::Value << go.second->m_Name;
			out << YAML::Key << "Active" << YAML::Value << go.second->m_Active;
			out << YAML::Key << "Tag" << YAML::Value << go.second->m_Tag;

			out << YAML::Key << "Components" << YAML::Value << YAML::BeginSeq; //Components

			for (std::unique_ptr<Component>& c : go.second->m_Components)
			{
				out << YAML::BeginMap;
				c->Serialize(out);
				out << YAML::EndMap;
			}

			out << YAML::EndSeq; //Components

			out << YAML::EndMap; //GameObject
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	bool SceneSerializer::DeserializeScene(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"]) { std::cout << "ERROR: Deserializing non-scene file as scene at: " << filePath << std::endl; return false; }

		m_Scene->m_Name = data["Scene"].as<std::string>();

		if (YAML::Node Textures = data["Textures"])
		{
			for (int texIdx = 0; texIdx < Textures.size(); texIdx++)
			{
				m_Scene->m_SceneTextures.push_back(LLGP::AssetManager::GetAsset<LLGP::Texture>(Textures[texIdx].as<std::string>()));
			}
		}
		if (YAML::Node Animations = data["Animations"])
		{
			for (int animIdx = 0; animIdx < Animations.size(); animIdx++)
			{
				m_Scene->m_SceneAnimations.push_back(LLGP::AssetManager::GetAsset<LLGP::Animation>(Animations[animIdx].as<std::string>()));
			}
		}

		if (YAML::Node GameObjects = data["GameObjects"])
		{
			if (!DeserializeGameObjects(GameObjects)) { std::cout << "ERROR: Deserializing scene : " << m_Scene->m_Name << std::endl; return false; }
		}
		return true;
	}
	bool SceneSerializer::DeserializePrefab(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Prefab"]) { std::cout << "ERROR: Deserializing non-prefab file as prefab at: " << filePath << std::endl; return false; }

		if (YAML::Node GameObjects = data["GameObjects"])
		{
			if (!DeserializeGameObjects(GameObjects)) { std::cout << "ERROR: Deserializing scene : " << m_Scene->m_Name << std::endl; return false; }
		}
		return true;
	}
	bool SceneSerializer::DeserializeGameObjects(YAML::Node GameObjects)
	{
		for (YAML::Node goData : GameObjects)
		{
			//m_Scene->m_SceneObjects.insert_or_assign(goData["GameObject"].as<uint64_t>(), std::make_unique<GameObject>(*m_Scene, goData));
			LLGP::GameObject* newGO = m_Scene->Instantiate(goData["Name"].as<std::string>());
			newGO->SetActive(goData["Active"].as<bool>());
			newGO->SetTag(goData["Tag"].as<std::string>());

			if (YAML::Node components = goData["Components"])
			{
				for (YAML::Node compNode : components)
				{
					LLGP::Component* compRef = nullptr;
					YAML::Node data;
					if (data = compNode["Transform"])
					{
						compRef = newGO->transform;
						if (!compRef->Deserialize(data, m_LinkRequests)) { std::cout << "ERROR: Deserializing component: Transform" << std::endl; }
					}
					else if (data = compNode["Rigidbody"])
					{
						compRef = newGO->AddComponent<LLGP::Rigidbody>();
						if (!compRef->Deserialize(data, m_LinkRequests)) { std::cout << "ERROR: Deserializing component: Rigidbody" << std::endl; }
					}
					else if (data = compNode["CircleCollider"])
					{
						compRef = newGO->AddComponent<LLGP::CircleCollider>();
						if (!compRef->Deserialize(data, m_LinkRequests)) { std::cout << "ERROR: Deserializing component: CircleCollider" << std::endl; }
					}
					else if (data = compNode["BoxCollider"])
					{
						compRef = newGO->AddComponent<LLGP::BoxCollider>();
						if (!compRef->Deserialize(data, m_LinkRequests)) { std::cout << "ERROR: Deserializing component: BoxCollider" << std::endl; }
					}
					else if (data = compNode["Animator"])
					{
						compRef = newGO->AddComponent<LLGP::Animator>();
						if (!compRef->Deserialize(data, m_LinkRequests)) { std::cout << "ERROR: Deserializing component: Animator" << std::endl; }
					}
					else if (data = compNode["Renderer"])
					{
						compRef = newGO->AddComponent<LLGP::Renderer>();
						if (!compRef->Deserialize(data, m_LinkRequests)) { std::cout << "ERROR: Deserializing component: Renderer" << std::endl; }
					}
					else if (data = compNode["PlayerMovement"])
					{
						compRef = newGO->AddComponent<TEST::PlayerMovement>();
						if (!compRef->Deserialize(data, m_LinkRequests)) { std::cout << "ERROR: Deserializing component: PlayerMovement" << std::endl; }
					}

					if (compRef)
					{
						m_Token2PtrLUT.insert({ data["UUID"].as<uint64_t>(), compRef });
					}
				}
				newGO->OnStart();
			}
			else
			{
				return false;
			}
		}

		for (LLGP::LinkRequest& linkRequest : m_LinkRequests)
		{
			if (!m_Token2PtrLUT.contains(linkRequest.linkToken)) { std::cout << "ERROR: deserializing scene: " << m_Scene->m_Name << ". Unable to find local token for " << linkRequest.linkToken << std::endl; return false; }
			linkRequest.linkDelegate(m_Token2PtrLUT[linkRequest.linkToken]);
		}
		return true;
	}
}

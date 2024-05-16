#include <Core/SceneSerializer.h>
#include <fstream>
#include <yaml-cpp/yaml.h>
#include <type_traits>

#include <Core/GameObject.h>

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
		out << YAML::Key << "GameObjects" << YAML::Value << YAML::BeginSeq;
		for (std::pair<const uint64_t, std::unique_ptr<LLGP::GameObject>>& go : m_Scene->m_SceneObjects)
		{
			go.second->Serialize(out);
		}
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filePath);
		fout << out.c_str();
	}

	bool SceneSerializer::Deserialize(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if(!data["Scene"])
			return false;

		std::string newName = data["Scene"].as<std::string>();
		if (m_Scene->m_Name != newName) { std::cout << "Error Deserializing Scene: " << newName << ". Already has different name: " << m_Scene->m_Name << std::endl; return false; }
		if (YAML::Node GameObjects = data["GameObjects"])
		{
			for (YAML::Node goData : GameObjects)
			{
				m_Scene->m_SceneObjects.insert_or_assign(goData["GameObject"].as<uint64_t>(), std::make_unique<GameObject>(*m_Scene, goData));
			}
		}
		return true;
	}
}

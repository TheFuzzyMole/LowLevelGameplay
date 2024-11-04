#pragma once
#include <memory>
#include <unordered_map>
#include <string>
#include <concepts>
#include <filesystem>
#include <iostream>
#include <Core/Assets/Asset.h>

namespace LLGP
{
	class AssetManager
	{
	public:
		AssetManager() = delete;

		/*template<class T> requires std::derived_from<T, Asset>
		static std::shared_ptr<T> GetAsset(std::string filePath)
		{
			std::shared_ptr<T> toReturn;
			if (!map.contains(filePath) || map[filePath].expired())
			{
				//load a new one
				toReturn = std::make_shared<T>(filePath);
				map.insert({ filePath, toReturn });
				return toReturn;
			}
			
			std::shared_ptr<T> check = dynamic_pointer_cast<T>(map[filePath].lock());
			if (check == nullptr)
			{
				//requested wrong type, error
			}
			return check;
		}*/

		template<class T> requires std::derived_from<T, Asset>
		static std::shared_ptr<T> GetAsset(std::filesystem::path filePath)
		{
			std::shared_ptr<T> toReturn;
			if (!map.contains(filePath) || map[filePath].expired())
			{
				//load a new one
				toReturn = std::make_shared<T>(filePath);
				map.insert({ filePath, toReturn });
				return toReturn;
			}

			std::shared_ptr<T> check = dynamic_pointer_cast<T>(map[filePath].lock());
			if (check == nullptr)
			{
				std::cout << "ERROR: AssetManager - GetAsset has been invoked with a mismatch type compared to the filepath supplied" << std::endl;

			}
			return check;
		}

	private:
		static inline std::unordered_map<std::filesystem::path, std::weak_ptr<Asset>> map;
	};
}

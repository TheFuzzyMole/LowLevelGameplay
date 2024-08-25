#include <Core/AssetManager.h>

namespace LLGP
{
	std::unordered_map<std::string, std::weak_ptr<Asset>> AssetManager::map;
}

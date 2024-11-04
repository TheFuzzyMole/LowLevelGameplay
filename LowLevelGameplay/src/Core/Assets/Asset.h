#pragma once
#include <Core/Object.h>
#include <string>
#include <filesystem>

namespace LLGP
{
	class Asset : public Object
	{
	public:
		Asset(std::filesystem::path path) : Object(), m_AssetPath(path) {}
		virtual ~Asset() = default;

		inline std::filesystem::path& GetAssetPath() { return m_AssetPath; }

	protected:
		std::filesystem::path m_AssetPath;
	};
}

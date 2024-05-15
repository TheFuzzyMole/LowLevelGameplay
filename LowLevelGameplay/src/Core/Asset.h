#pragma once
#include <Core/Object.h>
#include <string>

namespace LLGP
{
	class Asset : public Object
	{
	public:
		Asset(std::string path) : Object(), m_AssetPath(path) {}
		virtual ~Asset() = default;

		inline std::string& GetAssetPath() { return m_AssetPath; }

	private:
		std::string m_AssetPath;
	};
}

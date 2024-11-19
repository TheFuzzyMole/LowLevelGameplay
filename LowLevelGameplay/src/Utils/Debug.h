#pragma once
#include <string>

namespace LLGP
{
	class GameObject;

	static class Debug
	{
	public:
		static void Assert(bool pred, const std::string& message = "", LLGP::GameObject* context = nullptr);
		static void Log(const std::string& message, LLGP::GameObject* context = nullptr);
		static void LogWarning(const std::string& message, LLGP::GameObject* context = nullptr);
		static void LogError(const std::string& message, LLGP::GameObject* context = nullptr);

	private:
		static void LogImpl(const std::string& message, LLGP::GameObject* context = nullptr);
	};
}

#include "Debug.h"
#include <iostream>
#include <Core/GameObject.h>
#include <cassert>
#if defined(_WIN32)
#include <Windows.h>
#endif


namespace LLGP
{
    void Debug::Assert(bool pred, const std::string& message, LLGP::GameObject* context)
    {
		if (!pred)
		{
#if defined(_WIN32)
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(hConsole, 12);
#endif
			std::cout << "ASSERT FAILED: ";
			LogImpl(message, context);
			assert(pred); //use the c++ assert library to actually pause execution
		}
    }
    void Debug::Log(const std::string& message, LLGP::GameObject* context)
	{
#if defined(_WIN32)
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 15);
#endif
		std::cout << "Log: ";
		LogImpl(message, context);
	}

	void Debug::LogWarning(const std::string& message, LLGP::GameObject* context)
	{
#if defined(_WIN32)
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 14);
#endif
		std::cout << "Wrn: ";
		LogImpl(message, context);
	}

	void Debug::LogError(const std::string& message, LLGP::GameObject* context)
	{
#if defined(_WIN32)
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 12);
#endif
		std::cout << "Err: ";
		LogImpl(message, context);
	}

	void Debug::LogImpl(const std::string& message, LLGP::GameObject* context)
	{
		if (context)
		{
			std::cout << context->GetName() << " ";
		}
		std::cout << message << std::endl;
	}
}

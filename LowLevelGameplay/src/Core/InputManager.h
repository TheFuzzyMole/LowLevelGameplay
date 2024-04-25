//#pragma once
//#include <unordered_map>
//#include <string>
//#include <vector>
//#include <iostream>
//#include <variant>
//#include <cassert>
//#include <SFML/Window.hpp>
//#include <Core/Event.h>
//#include <Core/Vector2.h>
//
//namespace LLGP
//{
//	//TODO: template the shit out of this for bool, float and Vector2
//
//	struct InputBinding
//	{
//		InputBinding(sf::Keyboard::Key _key, bool _mult) : key(_key), multiplier(_mult) {}
//		InputBinding(sf::Keyboard::Key _key, float _mult) : key(_key), multiplier(_mult) {}
//		InputBinding(sf::Keyboard::Key _key, LLGP::Vector2f _mult) : key(_key), multiplier(_mult) {}
//
//		sf::Keyboard::Key key;
//		std::variant<bool, float, LLGP::Vector2f> multiplier;
//
//		template<typename T>
//		T GetValue()
//		{
//			assert(std::holds_alternative<T>(multiplier));
//			try
//			{
//				return (T)(std::get<T>(multiplier) * (int)sf::Keyboard::isKeyPressed(key));
//			}
//			catch (const std::bad_variant_access& ex)
//			{
//				std::cout << ex.what() << std::endl;
//				return T();
//			}
//		}
//	};
//
//	enum ActionType
//	{
//		Button = 0,
//		Axis = 1,
//		Joystick = 2
//	};
//
//	class InputAction
//	{
//	public:
//		InputAction(ActionType _type) : type(_type)
//		{
//			switch (type)
//			{
//			case LLGP::Button:
//				m_prevValue = false;
//				break;
//			case LLGP::Axis:
//				m_prevValue = 0.0f;
//				break;
//			case LLGP::Joystick:
//				m_prevValue = LLGP::Vector2f::zero;
//				break;
//			}
//		}
//		
//		ActionType type;
//		std::vector<LLGP::InputBinding> bindings;
//
//		LLGP::Event<std::variant<bool, float, LLGP::Vector2f>> OnStarted;
//		LLGP::Event<std::variant<bool, float, LLGP::Vector2f>> OnHeld;
//		LLGP::Event<std::variant<bool, float, LLGP::Vector2f>> OnCancelled;
//		
//		void ProcessBindings();
//		void AddBinding(sf::Keyboard::Key _key, std::variant<bool, float, LLGP::Vector2f> _mult);
//
//	private:
//		std::variant<bool, float, LLGP::Vector2f> m_prevValue;
//	};
//
//	class InputManager
//	{
//	public:
//		InputManager() = delete;
//
//		static std::unordered_map<std::string, InputAction> Actions;
//
//		static void ProcessInput();
//		static void AddAction(std::string _name, ActionType _type);
//	};
//}

#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <variant>
#include <SFML/Window.hpp>
#include <Core/Event.h>
#include <Core/Maths/Vector2.h>

namespace LLGP
{
	enum ActionType
	{
		Button = 0,
		Axis = 1,
		Vector = 2
	};

	struct JoyWrapper
	{
		int deviceID;
		int buttonID;
	};

	struct CompositeButtonAxis
	{
		std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> positive;
		std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> negative;
		float EvaluateAxis()
		{
			float output = 0.f;
			switch (positive.index())
			{
			case 0:
				output += sf::Keyboard::isKeyPressed(std::get<0>(positive));
				break;
			case 1:
				output += sf::Mouse::isButtonPressed(std::get<1>(positive));
				break;
			case 2:
				output += sf::Joystick::isButtonPressed(std::get<2>(positive).deviceID, std::get<2>(positive).buttonID);
				break;
			}
			switch (negative.index())
			{
			case 0:
				output -= sf::Keyboard::isKeyPressed(std::get<0>(negative));
				break;
			case 1:
				output -= sf::Mouse::isButtonPressed(std::get<1>(negative));
				break;
			case 2:
				output -= sf::Joystick::isButtonPressed(std::get<2>(negative).deviceID, std::get<2>(negative).buttonID);
				break;
			}

			return output;
		}
	};

	struct CompositeButtonVector
	{
		CompositeButtonVector(std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> _up, std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> _down, std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> _left, std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> _right) :
			up(_up), down(_down), left(_left), right(_right) {}
		std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> up;
		std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> down;
		std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> left;
		std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> right;
		LLGP::Vector2f EvaluateVector()
		{
			LLGP::Vector2f output = LLGP::Vector2f::zero;
			switch (up.index())
			{
			case 0:
				output += LLGP::Vector2f::up * sf::Keyboard::isKeyPressed(std::get<0>(up));
				break;
			case 1:
				output += LLGP::Vector2f::up * sf::Mouse::isButtonPressed(std::get<1>(up));
				break;
			case 2:
				output += LLGP::Vector2f::up * sf::Joystick::isButtonPressed(std::get<2>(up).deviceID, std::get<2>(up).buttonID);
				break;
			}
			switch (down.index())
			{
			case 0:
				output += LLGP::Vector2f::down * sf::Keyboard::isKeyPressed(std::get<0>(down));
				break;
			case 1:
				output += LLGP::Vector2f::down * sf::Mouse::isButtonPressed(std::get<1>(down));
				break;
			case 2:
				output += LLGP::Vector2f::down * sf::Joystick::isButtonPressed(std::get<2>(down).deviceID, std::get<2>(down).buttonID);
				break;
			}
			switch (left.index())
			{
			case 0:
				output += LLGP::Vector2f::left * sf::Keyboard::isKeyPressed(std::get<0>(left));
				break;
			case 1:
				output += LLGP::Vector2f::left * sf::Mouse::isButtonPressed(std::get<1>(left));
				break;
			case 2:
				output += LLGP::Vector2f::left * sf::Joystick::isButtonPressed(std::get<2>(left).deviceID, std::get<2>(left).buttonID);
				break;
			}
			switch (right.index())
			{
			case 0:
				output += LLGP::Vector2f::right * sf::Keyboard::isKeyPressed(std::get<0>(right));
				break;
			case 1:
				output += LLGP::Vector2f::right * sf::Mouse::isButtonPressed(std::get<1>(right));
				break;
			case 2:
				output += LLGP::Vector2f::right * sf::Joystick::isButtonPressed(std::get<2>(right).deviceID, std::get<2>(right).buttonID);
				break;
			}
			return output;
		}
	};

	struct CompositeAxisVector
	{
		std::variant<CompositeButtonAxis, JoyWrapper> x;
		std::variant<CompositeButtonAxis, JoyWrapper> y;
		LLGP::Vector2f EvaluateVector()
		{
			LLGP::Vector2f output = LLGP::Vector2f::zero;
			switch (x.index())
			{
			case 0:
				output += LLGP::Vector2f::right * std::get<0>(x).EvaluateAxis();
				break;
			case 1:
				output += LLGP::Vector2f::right * sf::Joystick::getAxisPosition(std::get<1>(x).deviceID, (sf::Joystick::Axis)std::get<1>(x).buttonID);
				break;
			}
			switch (y.index())
			{
			case 0:
				output += LLGP::Vector2f::up * std::get<0>(y).EvaluateAxis();
				break;
			case 1:
				output += LLGP::Vector2f::up * sf::Joystick::getAxisPosition(std::get<1>(y).deviceID, (sf::Joystick::Axis)std::get<1>(y).buttonID);
				break;
			}
			return output;
		}
	};

#pragma region Input Binding Types
	class InputBinding
	{
	public:
		InputBinding(float _mult) : m_Multiplier(_mult) {}

		virtual int GetType() { return -1; }

	protected:
		float m_Multiplier;
	};

	class InputBinding_Button : public InputBinding
	{
	public:
		InputBinding_Button(std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> _button, float _mult) : InputBinding(_mult), m_Button(_button) {}
		InputBinding_Button(InputBinding_Button&) = default;

		bool GetValue()
		{
			switch (m_Button.index())
			{
			case 0:
				return sf::Keyboard::isKeyPressed(std::get<0>(m_Button)) * m_Multiplier > 0;
				break;
			case 1:
				return sf::Mouse::isButtonPressed(std::get<1>(m_Button)) * m_Multiplier > 0;
				break;
			case 2:
				return sf::Joystick::isButtonPressed(std::get<2>(m_Button).deviceID, std::get<2>(m_Button).buttonID) * m_Multiplier > 0;
				break;
			}
		}

		int GetType() override { return 0; }

	private:
		std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> m_Button;
	};

	class InputBinding_Axis : public InputBinding
	{
	public:
		InputBinding_Axis(std::variant<CompositeButtonAxis, JoyWrapper> _axis, float _mult) : InputBinding(_mult), m_Axis(_axis) {}
		InputBinding_Axis(InputBinding_Axis&) = default;

		float GetValue()
		{
			switch (m_Axis.index())
			{
			case 0:
				return std::get<0>(m_Axis).EvaluateAxis() * m_Multiplier;
				break;
			case 1:
				return sf::Joystick::getAxisPosition(std::get<1>(m_Axis).deviceID, (sf::Joystick::Axis)std::get<1>(m_Axis).buttonID) * m_Multiplier;
				break;
			}
		}

		int GetType() override { return 1; }

	private:
		std::variant<CompositeButtonAxis, JoyWrapper> m_Axis;
	};

	class InputBinding_Vector : public InputBinding
	{
	public:
		InputBinding_Vector(std::variant<CompositeButtonVector, CompositeAxisVector> _vec, float _mult) : InputBinding(_mult), m_Vector(_vec) {}
		InputBinding_Vector(InputBinding_Vector&) = default;

		LLGP::Vector2f GetValue()
		{
			switch (m_Vector.index())
			{
			case 0:
				return std::get<0>(m_Vector).EvaluateVector() * m_Multiplier;
				break;
			case 1:
				return std::get<1>(m_Vector).EvaluateVector() * m_Multiplier;
				break;
			}
		}

		int GetType() override { return 2; }

	private:
		std::variant<CompositeButtonVector, CompositeAxisVector> m_Vector;
	
	};
#pragma endregion


	class InputAction
	{
	public:
		InputAction(LLGP::ActionType _type = LLGP::ActionType::Button) : type(_type)
		{
			switch (_type)
			{
			case LLGP::Button:
				m_prevValue = false;
				break;
			case LLGP::Axis:
				m_prevValue = 0.0f;
				break;
			case LLGP::Vector:
				m_prevValue = LLGP::Vector2f::zero;
				break;
			}
		}
		
		ActionType type;

		LLGP::Event<std::variant<bool, float, LLGP::Vector2f>> OnStarted;
		LLGP::Event<std::variant<bool, float, LLGP::Vector2f>> OnHeld;
		LLGP::Event<std::variant<bool, float, LLGP::Vector2f>> OnCancelled;
		
		void ProcessBindings();
		
		void AddBinding(InputBinding_Button* _binding);
		void AddBinding(InputBinding_Axis* _binding);
		void AddBinding(InputBinding_Vector* _binding);

	private:
		std::vector<std::unique_ptr<LLGP::InputBinding>> m_Bindings;
		std::variant<bool, float, LLGP::Vector2f> m_prevValue;
	};

	class InputManager
	{
	public:
		InputManager() = delete;

		static inline std::unordered_map<std::string, InputAction> Actions;

		static void ProcessInput();
		static InputAction* AddAction(std::string _name, ActionType _type);
		static InputAction* FindAction(const std::string& _name);
	};
}

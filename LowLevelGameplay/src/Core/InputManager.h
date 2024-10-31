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

	enum BindingModifier : int
	{
		LSHIFT = 1 << 0,
		RSHIFT = 1 << 1,
		CTRL = 1 << 2,
		ALT = 1 << 3
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

		bool operator==(const CompositeButtonAxis& other)
		{
			if (positive.index() != other.positive.index()) { return false; }

			switch (positive.index())
			{
			case 0:
				if (std::get<0>(positive) != std::get<0>(other.positive)) { return false; }
				break;
			case 1:
				if (std::get<1>(positive) != std::get<1>(other.positive)) { return false; }
				break;
			case 2:
				if (std::get<2>(positive).buttonID != std::get<2>(other.positive).buttonID) { return false; }
				break;
			}

			if (negative.index() != other.negative.index()) { return false; }

			switch (negative.index())
			{
			case 0:
				if (std::get<0>(negative) != std::get<0>(other.negative)) { return false; }
				break;
			case 1:
				if (std::get<1>(negative) != std::get<1>(other.negative)) { return false; }
				break;
			case 2:
				if (std::get<2>(negative).buttonID != std::get<2>(other.negative).buttonID) { return false; }
				break;
			}

			return true;
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

		bool operator==(const CompositeButtonVector& other)
		{
			if(up.index() != other.up.index()){ return false; }

			switch (up.index())
			{
			case 0:
				if (std::get<0>(up) != std::get<0>(other.up)) { return false; }
				break;
			case 1:
				if(std::get<1>(up) != std::get<1>(other.up)) { return false; }
				break;
			case 2:
				if(std::get<2>(up).buttonID != std::get<2>(other.up).buttonID) { return false; }
				break;
			}

			if (down.index() != other.down.index()) { return false; }

			switch (down.index())
			{
			case 0:
				if (std::get<0>(down) != std::get<0>(other.down)) { return false; }
				break;
			case 1:
				if (std::get<1>(down) != std::get<1>(other.down)) { return false; }
				break;
			case 2:
				if (std::get<2>(down).buttonID != std::get<2>(other.down).buttonID) { return false; }
				break;
			}

			if (left.index() != other.left.index()) { return false; }

			switch (left.index())
			{
			case 0:
				if (std::get<0>(left) != std::get<0>(other.left)) { return false; }
				break;
			case 1:
				if (std::get<1>(left) != std::get<1>(other.left)) { return false; }
				break;
			case 2:
				if (std::get<2>(left).buttonID != std::get<2>(other.left).buttonID) { return false; }
				break;
			}

			if (right.index() != other.right.index()) { return false; }

			switch (right.index())
			{
			case 0:
				if (std::get<0>(right) != std::get<0>(other.right)) { return false; }
				break;
			case 1:
				if (std::get<1>(right) != std::get<1>(other.right)) { return false; }
				break;
			case 2:
				if (std::get<2>(right).buttonID != std::get<2>(other.right).buttonID) { return false; }
				break;
			}

			return true;
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

		bool operator==(const CompositeAxisVector& other)
		{
			if (x.index() != other.x.index()) { return false; }

			switch (x.index())
			{
			case 0:
				if (std::get<0>(x) != std::get<0>(other.x)) { return false; }
				break;
			case 1:
				if (std::get<1>(x).buttonID != std::get<1>(other.x).buttonID) { return false; }
				break;
			}

			if (y.index() != other.y.index()) { return false; }

			switch (y.index())
			{
			case 0:
				if (std::get<0>(y) != std::get<0>(other.y)) { return false; }
				break;
			case 1:
				if (std::get<1>(y).buttonID != std::get<1>(other.y).buttonID) { return false; }
				break;
			}

			return true;
		}
	};

#pragma region Input Binding Types
	class InputBinding
	{
	public:
		InputBinding(float _mult, int _modifier) : m_Multiplier(_mult), m_Modifier(_modifier) {}

		virtual int GetType() { return -1; }

	protected:
		float m_Multiplier;
		int m_Modifier; //stored as BindingModifier

		bool CheckModifiers();

	};

	class InputBinding_Button : public InputBinding
	{
	public:
		InputBinding_Button(std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> _button, float _mult, int _modifier) : InputBinding(_mult, _modifier), m_Button(_button) {}
		InputBinding_Button(InputBinding_Button&) = default;

		bool GetValue()
		{
			if (!CheckModifiers()) { return false; }

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

		bool operator==(const InputBinding_Button& other)
		{
			if (m_Button.index() != other.m_Button.index()) { return false; }
			
			switch (m_Button.index())
			{
			case 0:
				return std::get<0>(m_Button) == std::get<0>(other.m_Button);
			case 1:
				return std::get<1>(m_Button) == std::get<1>(other.m_Button);
			case 2:
				return std::get<2>(m_Button).buttonID == std::get<2>(other.m_Button).buttonID;
			}
			return false;
		}

	private:
		std::variant<sf::Keyboard::Key, sf::Mouse::Button, JoyWrapper> m_Button;
	};

	class InputBinding_Axis : public InputBinding
	{
	public:
		InputBinding_Axis(std::variant<CompositeButtonAxis, JoyWrapper> _axis, float _mult, int _modifier) : InputBinding(_mult, _modifier), m_Axis(_axis) {}
		InputBinding_Axis(InputBinding_Axis&) = default;

		float GetValue()
		{
			if (!CheckModifiers()) { return 0.f; }

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

		bool operator==(const InputBinding_Axis& other)
		{
			if (m_Axis.index() != other.m_Axis.index()) { return false; }

			switch (m_Axis.index())
			{
			case 0:
				return std::get<0>(m_Axis) == std::get<0>(other.m_Axis);
			case 1:
				return std::get<1>(m_Axis).buttonID == std::get<1>(other.m_Axis).buttonID;
			}
		}

	private:
		std::variant<CompositeButtonAxis, JoyWrapper> m_Axis;
	};

	class InputBinding_Vector : public InputBinding
	{
	public:
		InputBinding_Vector(std::variant<CompositeButtonVector, CompositeAxisVector> _vec, float _mult, int _modifier) : InputBinding(_mult, _modifier), m_Vector(_vec) {}
		InputBinding_Vector(InputBinding_Vector&) = default;

		LLGP::Vector2f GetValue()
		{
			if (!CheckModifiers()) { return LLGP::Vector2f::zero; }
			
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

		bool operator==(const InputBinding_Vector& other)
		{
			if (m_Vector.index() != other.m_Vector.index()) { return false; }

			switch (m_Vector.index())
			{
			case 0:
				return	std::get<0>(m_Vector) == std::get<0>(other.m_Vector);
			case 1:
				return	std::get<1>(m_Vector) == std::get<1>(other.m_Vector);
			}
		}

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

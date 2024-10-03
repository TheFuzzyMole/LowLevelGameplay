#include <Core/InputManager.h>
#include <iostream>

namespace LLGP
{
	void InputAction::ProcessBindings()
	{
		switch (type)
		{
		case LLGP::Button:
		{
			bool total = false;
			for (int bindingIndex = 0; bindingIndex < m_Bindings.size(); bindingIndex++)
			{
				if (InputBinding_Button* buttonBind = dynamic_cast<InputBinding_Button*>(m_Bindings[bindingIndex].get()))
				{
					total = (total + buttonBind->GetValue() > 0);
				}
			}

			if (total != std::get<0>(m_prevValue))
			{
				if (total) { OnStarted(total); OnHeld(total); }
				else { OnCancelled(total); }

				m_prevValue = total;
			}
		}
			break;
		case LLGP::Axis:
		{
			float total = 0.f;
			for (int bindingIndex = 0; bindingIndex < m_Bindings.size(); bindingIndex++)
			{
				if (InputBinding_Axis* axisBind = dynamic_cast<InputBinding_Axis*>(m_Bindings[bindingIndex].get()))
				{
					total += axisBind->GetValue();
				}
			}

			if (total != std::get<1>(m_prevValue))
			{
				if (std::get<1>(m_prevValue) == 0.f) { OnStarted(total); OnHeld(total); }
				else if (total == 0.f) { OnCancelled(total); }
				else { OnHeld(total); }
				
				m_prevValue = total;
			}
		}
			break;
		case LLGP::Vector:
		{
			LLGP::Vector2f total = LLGP::Vector2f::zero;
			for (int bindingIndex = 0; bindingIndex < m_Bindings.size(); bindingIndex++)
			{
				if (InputBinding_Vector* vectorBind = dynamic_cast<InputBinding_Vector*>(m_Bindings[bindingIndex].get()))
				{
					total += vectorBind->GetValue();
				}
			}

			if (total != std::get<2>(m_prevValue))
			{
				if (std::get<2>(m_prevValue) == LLGP::Vector2f::zero) { OnStarted(total); OnHeld(total); }
				else if (total == LLGP::Vector2f::zero) { OnCancelled(total); }
				else { OnHeld(total); }

				m_prevValue = total;
			}
		}
			break;
		}


		
	}

	void InputAction::AddBinding(InputBinding_Button* _binding)
	{
		if (_binding->GetType() != (int)type)
		{
			std::cout << "trying to add binding of type " << (ActionType)_binding->GetType() << " to an Action of type " << type << std::endl;
			return;
		}
		m_Bindings.push_back(std::make_unique<InputBinding_Button>(*_binding));
	}

	void InputAction::AddBinding(InputBinding_Axis* _binding)
	{
		if (_binding->GetType() != (int)type)
		{
			std::cout << "trying to add binding of type " << (ActionType)_binding->GetType() << " to an Action of type " << type << std::endl;
			return;
		}
		m_Bindings.push_back(std::make_unique<InputBinding_Axis>(*_binding));
	}

	void InputAction::AddBinding(InputBinding_Vector* _binding)
	{
		if (_binding->GetType() != (int)type)
		{
			std::cout << "trying to add binding of type " << (ActionType)_binding->GetType() << " to an Action of type " << type << std::endl;
			return;
		}
		m_Bindings.push_back(std::make_unique<InputBinding_Vector>(*_binding));
	}

	//std::unordered_map<std::string, LLGP::InputAction> InputManager::Actions;

	void InputManager::ProcessInput()
	{
		for (auto& action : Actions)
		{
			action.second.ProcessBindings();
		}
	}

	InputAction* InputManager::AddAction(std::string _name, ActionType _type)
	{
		Actions.insert({ _name, InputAction(_type) });
		return &Actions.at(_name);
	}
	InputAction* InputManager::FindAction(const std::string& _name)
	{
		if (Actions.find(_name) != Actions.end())
		{
			return &Actions.at(_name);
		}
		else
		{
			return nullptr;
		}
	}
}
#include "pch.h"
#include "InputManager.h"
#include "StateMachine/GamePadInputAtom.h"
#include "FightLabMove/Move.h"

InputManager::InputManager()
{
	ButtonAtomsPressed = new ButtonGamePadInputAtom[2] {ButtonGamePadInputAtom( "Button0Pressed", GamePadButtons::Button0, ButtonState::PRESSED), ButtonGamePadInputAtom("Button1Pressed", GamePadButtons::Button1, ButtonState::PRESSED)};
	ButtonAtomsReleased = new ButtonGamePadInputAtom[2]{ButtonGamePadInputAtom( "Button0Released",GamePadButtons::Button0, ButtonState::RELEASE), ButtonGamePadInputAtom("Button1Released", GamePadButtons::Button1, ButtonState::RELEASE)};
	ButtonAtomsRepeat = new ButtonGamePadInputAtom[2]  {ButtonGamePadInputAtom( "Button0Repeat",GamePadButtons::Button0, ButtonState::REPEAT)   , ButtonGamePadInputAtom("Button1Repeat", GamePadButtons::Button1, ButtonState::REPEAT)};
	ButtonAtomsUp = new ButtonGamePadInputAtom[2]      {ButtonGamePadInputAtom( "Button0Up",GamePadButtons::Button0, ButtonState::UP)           , ButtonGamePadInputAtom("Button1Up", GamePadButtons::Button1, ButtonState::UP)};

	DirectionalInputAtomMap[0] = GamePadDirInputAtom("Neutral",DPadDirections::Neutral, ButtonState::PRESSED);
	
	DirectionalInputAtomMap[1] = GamePadDirInputAtom("Up", DPadDirections::Up, ButtonState::PRESSED);
	DirectionalInputAtomMap[2] = GamePadDirInputAtom("Forward",DPadDirections::Forward, ButtonState::PRESSED);
	DirectionalInputAtomMap[3] = GamePadDirInputAtom("UpForward",DPadDirections::UpForward, ButtonState::PRESSED);
	DirectionalInputAtomMap[4] = GamePadDirInputAtom("Down",DPadDirections::Down, ButtonState::PRESSED);
	DirectionalInputAtomMap[6] = GamePadDirInputAtom("DownForward",DPadDirections::DownForward, ButtonState::PRESSED);
	DirectionalInputAtomMap[8] = GamePadDirInputAtom("Back",DPadDirections::Back, ButtonState::PRESSED);
	DirectionalInputAtomMap[9] = GamePadDirInputAtom("UpBack",DPadDirections::UpBack, ButtonState::PRESSED);
	DirectionalInputAtomMap[12] = GamePadDirInputAtom("DownBack",DPadDirections::DownBack, ButtonState::PRESSED);

	DirectionalInputAtomMap[17] = GamePadDirInputAtom("Up_Repeat",DPadDirections::Up, ButtonState::REPEAT);
	DirectionalInputAtomMap[18] = GamePadDirInputAtom("Forward_Repeat",DPadDirections::Forward, ButtonState::REPEAT);
	DirectionalInputAtomMap[19] = GamePadDirInputAtom("UpForward_Repeat",DPadDirections::UpForward, ButtonState::REPEAT);
	DirectionalInputAtomMap[20] = GamePadDirInputAtom("Down_Repeat",DPadDirections::Down, ButtonState::REPEAT);
	DirectionalInputAtomMap[22] = GamePadDirInputAtom("DownForward_Repeat",DPadDirections::DownForward, ButtonState::REPEAT);
	DirectionalInputAtomMap[24] = GamePadDirInputAtom("Back_Repeat",DPadDirections::Back, ButtonState::REPEAT);
	DirectionalInputAtomMap[25] = GamePadDirInputAtom("UpBack_Repeat",DPadDirections::UpBack, ButtonState::REPEAT);
	DirectionalInputAtomMap[28] = GamePadDirInputAtom("DownBack_Repeat",DPadDirections::DownBack, ButtonState::REPEAT);
													  
	DirectionalInputAtomMap[33] = GamePadDirInputAtom("Up_Charged",DPadDirections::Up, ButtonState::CHARGED);
	DirectionalInputAtomMap[34] = GamePadDirInputAtom("Forward_Charged",DPadDirections::Forward, ButtonState::CHARGED);
	DirectionalInputAtomMap[35] = GamePadDirInputAtom("UpForward_Charged",DPadDirections::UpForward, ButtonState::CHARGED);
	DirectionalInputAtomMap[36] = GamePadDirInputAtom("Down_Charged",DPadDirections::Down, ButtonState::CHARGED);
	DirectionalInputAtomMap[38] = GamePadDirInputAtom("DownForward_Charged",DPadDirections::DownForward, ButtonState::CHARGED);
	DirectionalInputAtomMap[40] = GamePadDirInputAtom("Back_Charged",DPadDirections::Back, ButtonState::CHARGED);
	DirectionalInputAtomMap[41] = GamePadDirInputAtom("UpBack_Charged",DPadDirections::UpBack, ButtonState::CHARGED);
	DirectionalInputAtomMap[44] = GamePadDirInputAtom("DownBack_Charged",DPadDirections::DownBack, ButtonState::CHARGED);

	Idle.InstancedBranches.push_back(&IdletoDown);
	Down.InstancedBranches.push_back(&DowntoDownForward);
	DownForward.InstancedBranches.push_back(&DownForwardtoForward);
	Forward.InstancedBranches.push_back(&ForwardtoJab);
	Jab.InstancedBranches.push_back(&JabtoCompleted);

	IdletoDown.DestinationState = &Down;

	IdletoDown.ForbiddenButtons |= 1 << (uint16_t)GamePadButtons::Button0;
	IdletoDown.ForbiddenButtons |= 1 << (uint16_t)GamePadButtons::Button1;

	DowntoDownForward.ForbiddenButtons |= 1 << (uint16_t)GamePadButtons::Button0;
	DowntoDownForward.ForbiddenButtons |= 1 << (uint16_t)GamePadButtons::Button1;

	DownForwardtoForward.ForbiddenButtons |= 1 << (uint16_t)GamePadButtons::Button0;
	DownForwardtoForward.ForbiddenButtons |= 1 << (uint16_t)GamePadButtons::Button1;

	ForwardtoJab.RequiredButtons |= 1 << (uint16_t)GamePadButtons::Button0;
	ForwardtoJab.ForbiddenButtons |= 1 << (uint16_t)GamePadButtons::Button1;

	Move Idle;
	Move Hadouken;
	Move Jab;

	MoveLink IdletoJabLink;
	MoveLink IdletoHadoukenLink;
	MoveLink JabtoHadoukenLink;
	MoveLink JabtoIdleLink;
	MoveLink HadoukentoIdleLink;
}

InputManager::~InputManager()
{
	
}

void InputManager::Init(EventBus* _bus, GLFWwindow* _window)
{
	m_window = _window;
	m_eventBus = _bus;
	//glfwGetCursorPos(m_window, &lastMouseX, &lastMouseY);
	//mouseXPos = lastMouseX;
	//mouseYPos = lastMouseY;
}

void InputManager::ProcessInput()
{
	glfwPollEvents();

	if (glfwGetKey(m_window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		m_eventBus->publish(new ChangeAppStateEvent(ApplicationState::QUIT));

	processDPad();
	for (int i = 0; i < BUTTON_COUNT; i++)
	{
		getButtonState(i);
	}

	if (m_inputBuffer.size() > 30)
		m_inputBuffer.erase(m_inputBuffer.begin(), m_inputBuffer.begin() + 3);

	/*if (m_inputBuffer.size() == 30)
	{
		for (int i = 0; i < 30; i += 3)
		{
			std::cout << m_inputBuffer[i]->Name << std::endl;
			std::cout << m_inputBuffer[i + 1]->Name << std::endl;
			std::cout << m_inputBuffer[i + 2]->Name << std::endl;
		}
	}*/

	GLFWgamepadstate state;

	if (glfwJoystickIsGamepad(GLFW_JOYSTICK_1))
	{
		if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
		{
			lastGamePadState = currentGamePadState;
			currentGamePadState = 0;
			for (int i = 0; i < 9; i++)
			{
				currentGamePadState |= state.buttons[i] << i;
			}

			int count;
			const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);

			currentGamePadState |= axes[3] > 0.01 ? 1 << 9: 0 << 9;
			currentGamePadState |= axes[4]  > 0.01 ? 1 << 10 : 0 << 10;
			
			for (int i = 11; i < 15; i++)
			{
				currentGamePadState |= state.buttons[i] << i;
			}
		}
	}

	StateMachineResult res = Idle.RunState(m_inputBuffer, 0);
	if (res.completionType == StateMachineCompletionType::Accepted)
	{
		std::cout << "Completed!" << std::endl;
		int i;
		std::cin >> i;
	}
		
}

void InputManager::getButtonState(unsigned char buttonCode)
{
	if (!(currentGamePadState & 1 << buttonCode) && lastGamePadState & 1 << buttonCode)
	{
		m_inputBuffer.push_back(&ButtonAtomsReleased[(uint8_t)buttonCode]);
		//std::cout << (int)buttonCode << " Released!" << std::endl;
		buttonTimers[buttonCode] = 0;
		return;
	}
	else if (!(currentGamePadState & 1 << buttonCode) && !(lastGamePadState & 1 << buttonCode))
	{
		m_inputBuffer.push_back(&ButtonAtomsUp[(uint8_t)buttonCode]);
		//std::cout << (int)buttonCode << " Up!" << std::endl;
		return;
	}
		
	if ((currentGamePadState & 1 << buttonCode) && (lastGamePadState & 1 << buttonCode || !(lastGamePadState & 1 << buttonCode)))
	{
		if (buttonTimers[buttonCode] < 3)
		{
			m_inputBuffer.push_back(&ButtonAtomsPressed[(uint8_t)buttonCode]);
			//std::cout << (int)buttonCode << " Pressed! (time)" << std::endl;
		}
		else if (buttonTimers[buttonCode] >= 3)
		{
			m_inputBuffer.push_back(&ButtonAtomsRepeat[(uint8_t)buttonCode]);
			//std::cout << (int)buttonCode << " Repeat!" << std::endl;
		}
		
		//if (buttonTimers[buttonCode] == 60)
			//std::cout << (int)buttonCode << " Charged!" << std::endl;//m_inputBuffer[m_inputBuffer.size() - 1] = (InputAtom(buttonCode, ButtonState::CHARGED));
		buttonTimers[buttonCode] < 60 ? buttonTimers[buttonCode]++ : buttonTimers[buttonCode] = 60;
	}
}

void InputManager::processDPad()
{
	for (unsigned char i = 0; i < 4; i++)
	{
		if (!(currentGamePadState & 1 << (GLFW_GAMEPAD_BUTTON_DPAD_UP + i)) &&
			lastGamePadState & 1 << (GLFW_GAMEPAD_BUTTON_DPAD_UP + i))
		{
			unsigned char releasedButton = 0; 
			releasedButton |= 1 << i;
			directionMask &= ~(1 << i);
			directionMask &= ~(1 << CHARGED_DIRECTION_FLAG); 
			directionMask &= ~(1 << REPEAT_DIRECTION_FLAG);

			buttonTimers[(GLFW_GAMEPAD_BUTTON_DPAD_UP + i)] = 0;
		}
		if ((currentGamePadState & 1 << (GLFW_GAMEPAD_BUTTON_DPAD_UP + i)) && (lastGamePadState & 1 << (GLFW_GAMEPAD_BUTTON_DPAD_UP + i) || !(lastGamePadState & 1 << (GLFW_GAMEPAD_BUTTON_DPAD_UP + i))))
		{
			directionMask |= 1 << i;
			if (buttonTimers[(GLFW_GAMEPAD_BUTTON_DPAD_UP + i)] == 60)
			{
				directionMask &= ~(1 << REPEAT_DIRECTION_FLAG);
				directionMask |= 1 << CHARGED_DIRECTION_FLAG;
				//m_inputBuffer.push_back(&DirectionalInputAtomMap[directionMask]);
			}
			else if (buttonTimers[(GLFW_GAMEPAD_BUTTON_DPAD_UP + i)] >= 5)
			{
				directionMask |= 1 << REPEAT_DIRECTION_FLAG;
				//m_inputBuffer.push_back(&DirectionalInputAtomMap[directionMask]);
			}
			buttonTimers[(GLFW_GAMEPAD_BUTTON_DPAD_UP + i)] < 60 ? buttonTimers[(GLFW_GAMEPAD_BUTTON_DPAD_UP + i)]++ :
																				buttonTimers[(GLFW_GAMEPAD_BUTTON_DPAD_UP + i)] = 60;
		}
	}

	m_inputBuffer.push_back(&DirectionalInputAtomMap[directionMask]);
}

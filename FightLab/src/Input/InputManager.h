#pragma once
#include "FightLab_Input.h"
#include "StateMachine/GamePadInputAtom.h"
#include "StateMachine/FightLabSM.h"

#define CHARGED_DIRECTION_FLAG 5
#define	REPEAT_DIRECTION_FLAG 4

class InputManager
{
public:
	InputManager();
	~InputManager();

	void Init(EventBus* _bus, GLFWwindow* _window);

	void ProcessInput();
private:
	GLFWwindow* m_window = nullptr;
	EventBus* m_eventBus = nullptr;
	
	uint16_t currentGamePadState = 0;
	uint16_t lastGamePadState = 0;
	unsigned char directionMask = 0;
	
	std::vector<InputAtom*> m_inputBuffer;

	
	std::unordered_map<uint8_t, GamePadDirInputAtom> DirectionalInputAtomMap;

	ButtonGamePadInputAtom* ButtonAtomsPressed = nullptr;
	ButtonGamePadInputAtom* ButtonAtomsReleased = nullptr;
	ButtonGamePadInputAtom* ButtonAtomsRepeat = nullptr;
	ButtonGamePadInputAtom* ButtonAtomsUp = nullptr;

	State Completed = State(StateMachineCompletionType::Accepted);
	FLState Down = FLState(StateMachineCompletionType::NotAccepted);
	FLState DownForward = FLState(StateMachineCompletionType::NotAccepted);
	FLState Forward = FLState(StateMachineCompletionType::NotAccepted);
	FLState Jab = FLState(StateMachineCompletionType::NotAccepted);
	FLState Idle = FLState(StateMachineCompletionType::NotAccepted);

	FLBranch IdletoDown = FLBranch(DPadDirections::Down, &Down);
	FLBranch DowntoDownForward = FLBranch(DPadDirections::DownForward, &DownForward);
	FLBranch DownForwardtoForward = FLBranch(DPadDirections::Forward, &Forward);
	FLBranch ForwardtoJab = FLBranch(DPadDirections::Forward | DPadDirections:: Neutral | DPadDirections::UpForward, &Jab);
	BranchBase JabtoCompleted = BranchBase(&Completed);

	std::array<uint16_t, 16> buttonTimers {0};
	void getButtonState(unsigned char buttonCode);
	void processDPad();
};


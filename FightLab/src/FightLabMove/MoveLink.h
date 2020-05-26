#pragma once
#include "Move.h"
#include "StateMachine/FightLabSM.h"

class Move;

class MoveLink
{
public:
	MoveLink();
	~MoveLink();

	StateMachineResult TryLink(const Move* ref, const std::vector<InputAtom*>& DataSource, uint32_t DataIndex = 0, uint32_t RemainingSteps = -1);

	FLState* StateMachine;
	
	Move* LinktoMove;

	// Set of windows (in frames) when this link can be used.
	std::vector<glm::u16vec2> CancelWindows;

	//Clear Input when using this link
	uint32_t bClearInput : 1;
};
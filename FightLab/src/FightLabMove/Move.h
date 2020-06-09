#pragma once
#include "MoveLink.h"
#include "StateMachine/FightLabSM.h"
#include "ResourceManagement/SkinnedMesh/Animator/Animation.h"


class MoveLink;

struct LinkingResult
{
	LinkingResult()
	{
		Link = nullptr;
	}
	MoveLink* Link;

	StateMachineResult SMResult;
};

class Move
{
public:
	Move(const char* name = "", bool clearExit = false, bool clearEntry =false);
	~Move();

	const char* Name;

	Animation animation;

	std::vector<MoveLink*> Links;

	uint32_t bClearInputOnEntry : 1;

	uint32_t bClearInputOnExit : 1;

	LinkingResult TryLinks(const std::vector<InputAtom*>& DataSource, uint32_t DataIndex = 0, uint32_t RemainingSteps = -1);
};
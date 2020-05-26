#pragma once
#include "State.h"
#include "GamePadInputAtom.h"

class FLBranch : public BranchBase
{
public:
	FLBranch(DPadDirections acceptable_Dir, State* destination);
	~FLBranch() {}
	virtual State* TryBranch(const std::vector<InputAtom*>& dataSource,
		uint32_t dataIndex, uint32_t& outDataIndex) override;

	// Required buttons Mask
	uint16_t RequiredButtons = 0;

	//Forbidden buttons Mask 
	uint16_t ForbiddenButtons = 0;
	
protected:
	//Acceptable Dpad Direction Mask
	uint32_t AcceptableDirections = 0;
};

class FLState : public State
{
public:
	FLState(StateMachineCompletionType completionType, bool bterminateImediately = false, bool bloop = true);
	~FLState();
	virtual StateMachineResult LoopState(const std::vector<InputAtom*>& dataSource, uint32_t dataIndex, uint32_t remainingSteps) override;
};


#pragma once
//#include "InputAtom.h"

class State;

enum class StateMachineCompletionType: uint8_t
{
	NotAccepted,
	Accepted,
	Rejected,
	OutofSteps
};

struct StateMachineResult
{
	StateMachineCompletionType completionType;

	State* FinalState;

	int32_t DataIndex;
};

struct InputAtom
{
	std::string Name;
	InputAtom(){}
	InputAtom(std::string name)
	{
		Name = name;
	}
};

class BranchBase
{
public:
	BranchBase(State* destination = nullptr);
	~BranchBase();
	virtual State* TryBranch(const std::vector<InputAtom*>& dataSource,
		uint32_t dataIndex, uint32_t& outDataIndex);
	
	State* DestinationState;
protected:
	
};

class Branch : public BranchBase
{
public:
	Branch(State* destination = nullptr, bool breverseTest = false);
	~Branch();
	virtual State* TryBranch(const std::vector<InputAtom*>& dataSource,
							uint32_t dataIndex, uint32_t &outDataIndex);

	std::vector<InputAtom*> AcceptableInputs;
	
protected:
	uint32_t bReverseInputTest : 1;
};

class State
{
public:
	State() {}
	State(StateMachineCompletionType completionType, bool bterminateImediately = false, bool bloop = false);
	~State();
	
	virtual StateMachineResult RunState(const std::vector<InputAtom*>& dataSource, uint32_t dataIndex = 0, uint32_t remainingSteps = -1);
	std::vector<BranchBase*> InstancedBranches;
	std::vector<BranchBase*> SharedBranches;
protected:
	virtual StateMachineResult LoopState(const std::vector<InputAtom*>& dataSource, uint32_t dataIndex, uint32_t remainingSteps);
	
	StateMachineCompletionType CompletionType;
	uint32_t bTerminateImmediately : 1;
	uint32_t bLoopByDefault : 1;
	
};
#include "pch.h"
#include "State.h"

Branch::Branch(State* destination, bool breverseTest)
{
    bReverseInputTest = breverseTest;
    DestinationState = destination;
}

Branch::~Branch()
{
}

State* Branch::TryBranch(const std::vector<InputAtom*>& dataSource, uint32_t dataIndex, uint32_t& outDataIndex)
{
    outDataIndex = dataIndex;
    if (dataIndex <= dataSource.size() - 1)
    {
        for (int i = 0; i < AcceptableInputs.size(); i++)
        {
            if (AcceptableInputs[i] == dataSource[dataIndex])
            {
                outDataIndex++;
                return bReverseInputTest ? nullptr : DestinationState;
            }
        }
    }
    return bReverseInputTest ? DestinationState : nullptr;
}

State::State(StateMachineCompletionType completionType, bool bterminateImediately /*= false*/, bool bloop /*= false*/)
{
    CompletionType = completionType;
    bTerminateImmediately = bterminateImediately;
    bLoopByDefault = bloop;
}

State::~State()
{
}

StateMachineResult State::RunState(const std::vector<InputAtom*>& dataSource, uint32_t dataIndex, uint32_t remainingSteps)
{
    bool bMustEndNow = (bTerminateImmediately || dataIndex > dataSource.size());
    if (remainingSteps && !bMustEndNow)
    {
        State* destinationState = nullptr;
        uint32_t destinationDataIndex = dataIndex;
        for(uint32_t i = 0; i < InstancedBranches.size(); i++)
        {
            if (InstancedBranches[i])
            {
                destinationState = InstancedBranches[i]->TryBranch(dataSource, dataIndex, destinationDataIndex);
                if (destinationState)
                    return destinationState->RunState(dataSource, destinationDataIndex, remainingSteps);
            }
        }

        for (uint32_t i = 0; i < SharedBranches.size(); i++)
        {
            if (SharedBranches[i])
            {
                destinationState = SharedBranches[i]->TryBranch(dataSource, dataIndex, destinationDataIndex);
                if (destinationState)
                    return destinationState->RunState(dataSource, destinationDataIndex, remainingSteps);
            }
        }
        if (bLoopByDefault)
        {
            return LoopState(dataSource, dataIndex, remainingSteps);
        }
        bMustEndNow = true;
    }

    StateMachineResult Result;
    Result.FinalState = this;
    Result.DataIndex = dataIndex;
    Result.completionType = bMustEndNow ? CompletionType : StateMachineCompletionType::OutofSteps;
    return Result;
}

StateMachineResult State::LoopState(const std::vector<InputAtom*>& dataSource, uint32_t dataIndex, uint32_t remainingSteps)
{
    return RunState(dataSource, dataIndex + 1, remainingSteps - 1);
}

BranchBase::BranchBase(State* destination /*= nullptr*/)
{
    DestinationState = destination;
}

BranchBase::~BranchBase()
{
}

State* BranchBase::TryBranch(const std::vector<InputAtom*>& dataSource, uint32_t dataIndex, uint32_t& outDataIndex)
{
    outDataIndex = dataIndex;
    return DestinationState;
}

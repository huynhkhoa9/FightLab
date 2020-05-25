#include "pch.h"
#include "FightLabSM.h"

FLBranch::FLBranch(DPadDirections acceptable_Dir, State* destination)
{
	AcceptableDirections = (uint16_t)acceptable_Dir;
	DestinationState = destination;
}

State* FLBranch::TryBranch(const std::vector<InputAtom*>& dataSource, uint32_t dataIndex, uint32_t& outDataIndex)
{
    outDataIndex = dataIndex;

	if (RequiredButtons & ForbiddenButtons)
	{
		std::cout << "Impassable condition: Required button is also forbidden." << std::endl;
		return nullptr;
	}

	// Make sure we have at least the minimum data here.
	if ((dataIndex + (uint32_t)BUTTON_COUNT) >= dataSource.size())
	{
		//std::cout << "Error: Not enough data at index: " << dataIndex << std::endl;
		return nullptr;
	}

	// The first piece of data must be a DirectionalInput
	if (GamePadDirInputAtom* DirectionalInput = static_cast<GamePadDirInputAtom*>(dataSource[dataIndex]))
	{
		if (!(AcceptableDirections & ((uint16_t)DirectionalInput->Direction)))
		{
			return nullptr;
		}
		else
			std::cout << "current Direction: " <<(uint16_t)DirectionalInput->Direction << std::endl;
	}
	else
	{
		std::cout << "Error: No directional input at index " << dataIndex << std::endl;
		return nullptr;
	}
	++outDataIndex;
	bool bRequiredButtonPressed = false;
	for (uint32_t i = 0; i < BUTTON_COUNT; ++i, ++outDataIndex)
	{
		if (ButtonGamePadInputAtom* ButtonAtom = static_cast<ButtonGamePadInputAtom*>(dataSource[outDataIndex]))
		{
			if (RequiredButtons & (1 << (i)))
			{
				if (ButtonAtom->State == ButtonState::PRESSED)
				{
					bRequiredButtonPressed = true;
					continue;
				}
				else if (ButtonAtom->State == ButtonState::REPEAT)
				{
					continue;
				}
				else if (ButtonAtom->State == ButtonState::CHARGED)
				{
					continue;
				}
				return nullptr;
			}
			else if (ForbiddenButtons & (1 << (i))) 
			{
				// Any state other than having just pressed the forbidden button is OK, even holding it down.
				if (ButtonAtom->State != ButtonState::PRESSED)
				{
					continue;
				}
				return nullptr;
			}
		}
		else
		{
			std::cout << "Error: Expected " << BUTTON_COUNT <<" button inputs, only found " << i << std::endl;
			return nullptr;
		}
	}
	// If there were any required buttons, make sure at least one of them was just pressed.
	if (RequiredButtons && !bRequiredButtonPressed)
	{
		return nullptr;
	}

    return DestinationState;
}

FLState::FLState(StateMachineCompletionType completionType, bool bterminateImediately /*= false*/, bool bloop /*= false*/)
{
	CompletionType = completionType;
	bTerminateImmediately = bterminateImediately;
	bLoopByDefault = bloop;
}

FLState::~FLState()
{
}

StateMachineResult FLState::LoopState(std::vector<InputAtom*>& dataSource, uint32_t dataIndex, uint32_t remainingSteps)
{
	StateMachineResult result = RunState(dataSource, dataIndex + 1 + BUTTON_COUNT, remainingSteps - 1);
	//std::cout << "Current State: " << this << " Final State: " << result.FinalState << std::endl;
	return result;
}

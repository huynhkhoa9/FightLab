#include "pch.h"
#include "MoveLink.h"

MoveLink::MoveLink()
{
}

MoveLink::~MoveLink()
{
}

StateMachineResult MoveLink::TryLink(const Move* ref, const std::vector<InputAtom*>& dataSource, uint32_t dataIndex, uint32_t remainingSteps)
{
	if (StateMachine && LinktoMove)
	{
		bool bCanCancel = false;
		//Check if we are in the cancel window
		for (const glm::u16vec2& CancelWindow : CancelWindows)
		{
			if(ref->animation.CurrentTime)
			{
				bCanCancel = true;
				break;
			}
		}
		// If no cancel windows are provided, assume the move is always available.
		if (bCanCancel || !CancelWindows.size())
		{
			return StateMachine->RunState(dataSource, dataIndex, remainingSteps);
		}
	}
	return StateMachineResult();
}

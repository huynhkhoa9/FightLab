#include "pch.h"
#include "Move.h"

Move::Move(const char* name /*=""*/, bool clearExit /*=false*/ , bool clearEntry /*=false*/)
{
	Name = name;
	bClearInputOnEntry = clearEntry;
	bClearInputOnExit = clearExit;
}

Move::~Move()
{
}

LinkingResult Move::TryLinks(const std::vector<InputAtom*>& dataSource, uint32_t dataIndex, uint32_t remainingSteps)
{
	LinkingResult result;

	for (int i = 0; i < Links.size(); i++)
	{
		result.SMResult = Links[i]->TryLink(this, dataSource, dataIndex, remainingSteps);
		if (result.SMResult.completionType == StateMachineCompletionType::Accepted)
		{
			result.Link = Links[i];
			return result;
		}
	}
	return result;
}

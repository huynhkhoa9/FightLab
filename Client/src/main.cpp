#include "Core/FightLab.h"

int main(int argc, char* argv)
{
	State Completed(StateMachineCompletionType::Accepted, true);

	Application app;
	app.Init();
	app.Run();
	app.CleanUp();

	return 0;
}
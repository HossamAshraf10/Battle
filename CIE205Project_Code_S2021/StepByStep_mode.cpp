#include "StepByStep_mode.h"

void StepByStep_mode::Run()
{

	while (!isDone())
	{

		battle->incrementTimeStep();
		battle->ActivateEnemies();

		move_all_enimies();
		battle->GetCastle()->Fight(battle, battle->getCurrentTimeStep());
		Act_all_enemies();

		//Drawing
		pGUI->ResetDrawingList();
		battle->AddAllListsToDrawingList();
		pGUI->UpdateInterface(battle->StatusBarInformation());
		Sleep(50);
	}

}

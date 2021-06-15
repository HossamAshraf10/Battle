#include "StepByStep_mode.h"

void StepByStep_mode::Run()
{

	while (!isDone())
	{

		battle->incrementTimeStep();
		battle->ActivateEnemies();

		move_all_enimies();
		Act_all_enemies();

		//Drawing
		pGUI->ResetDrawingList();
		battle->AddAllListsToDrawingList();
		pGUI->UpdateInterface(battle->GetCastle()->GetHealth());
		pGUI->waitForClick();
	}

}
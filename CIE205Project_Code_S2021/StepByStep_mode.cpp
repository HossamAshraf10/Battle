#include "StepByStep_mode.h"

void StepByStep_mode::Run()
{

	while (!isDone())
	{

		battle->incrementTimeStep();
		battle->ActivateEnemies();

		move_all_enimies();
		Act_all_enemies();
		battle->LetCastleAttach(battle->getCurrentTimeStep());

		//Drawing
		pGUI->ResetDrawingList();
		battle->AddAllListsToDrawingList();
		pGUI->UpdateInterface(battle->GetCastle()->GetHealth());
		Sleep(1000);
	}

}

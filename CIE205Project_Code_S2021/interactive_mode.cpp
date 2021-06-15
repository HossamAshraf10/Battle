#include "interactive_mode.h"
Interactive_mode::Interactive_mode(Battle* b) :Battle_mode(b)
{

}


/*
Interactive_mode* Interactive_mode::getInstance(Battle* b)
{
	if (OBJ == nullptr)
		OBJ = new Interactive_mode(b);
	return OBJ;
}*/


void Interactive_mode::Run()
{

	while (!isDone())
	{
		battle->incrementTimeStep();
		battle->ActivateEnemies();
		
		move_all_enimies();
		Act_all_enemies();

		battle->GetCastle()->Fight(battle,battle->getCurrentTimeStep());

		//Drawing
		pGUI->ResetDrawingList();
		battle->AddAllListsToDrawingList();
		pGUI->UpdateInterface(battle->GetCastle()->GetHealth());
		pGUI->UpdateInterface(battle->StatusBarInformation());
		pGUI->waitForClick();
	}
	
}

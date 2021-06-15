#include "Silent_mode.h"


void Silent_mode::Run()
{

	while (!isDone())
	{

		battle->incrementTimeStep();
		battle->ActivateEnemies();

		move_all_enimies();
		Act_all_enemies();
		battle->GetCastle()->Fight(battle, battle->getCurrentTimeStep());

	}

}
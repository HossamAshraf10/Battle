#include "Silent_mode.h"


void Silent_mode::Run()
{

	while (!isDone())
	{

		battle->incrementTimeStep();
		battle->ActivateEnemies();

		move_all_enimies();
		battle->GetCastle()->Fight(battle, battle->getCurrentTimeStep());
		Act_all_enemies();

	}

}

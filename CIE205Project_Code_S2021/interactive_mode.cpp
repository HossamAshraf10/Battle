#include "interactive_mode.h"
Interactive_mode::Interactive_mode(Battle* b) :Battle_mode(b)
{
	superEnemyisOn = false;
	superCount = 0;
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

		superSoliders* superSolider1 = new superSoliders;
		battle->ActivateSuperSoliders(superSolider1, 1, 1000, battle->getCurrentTimeStep(), 1, 5, 1000);


		superSoliders* superSolider2 = new superSoliders;
		battle->ActivateSuperSoliders(superSolider2, 2, 1000, battle->getCurrentTimeStep(), 1, 5, 1000);


		superSoliders* superSolider3 = new superSoliders;
		battle->ActivateSuperSoliders(superSolider3, 3, 1000, battle->getCurrentTimeStep(), 1, 5, 1000);

		superSoliders* tempSuper;
		Queue<superSoliders*> tempSuperQueue;
		while (!battle->getSupersoliders()->isEmpty())
		{
			battle->getSupersoliders()->pop(tempSuper);
			tempSuper->Fight(battle, battle->getCurrentTimeStep());
			//battle->getSupersoliders()->push(tempSuper);
			tempSuperQueue.enqueue(tempSuper);
		}
		while (!tempSuperQueue.isEmpty())
		{
			tempSuperQueue.dequeue(tempSuper);
			battle->getSupersoliders()->push(tempSuper);
		}

		//Drawing
		pGUI->ResetDrawingList();
		battle->AddAllListsToDrawingList();
		pGUI->UpdateInterface(battle->GetCastle()->GetHealth());
		pGUI->waitForClick();

		//
		
		
		
				
				//*superEnemyisOn = true;
		
		
	}
	
}

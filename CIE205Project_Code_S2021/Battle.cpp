#include "Battle.h"
#include <time.h>

#include "ReadInput.h"
#include "WriteOutput.h"

#include "interactive_mode.h"
#include "StepByStep_mode.h"
#include "Silent_mode.h"

Battle::Battle()
{	
	EnemyCount = 0;
	KilledCount = 0;
	ActiveCount = 0;
	FrostedCount = 0;
	DemoListCount = 0;
	CurrentTimeStep = 0;
	this->isSuperNeededd = false;
	pGUI = new GUI;
	actv_fighters = new PriorityQueue<Fighter*>();
	actv_healers = new ArrayStack<Healer*>(100);
	actv_freezers = new Queue<Freezer*>;
	frzn_enms = new PriorityQueue<Enemy*>;
	kld_enms = new Queue<Enemy*>;
	actv_SuperSoliders = new ArrayStack<superSoliders*>(3);
}

Castle * Battle::GetCastle()
{
	return &BCastle;
}

void Battle::setCastle(Castle* castle) { this->BCastle = *(castle); }

void Battle::setInactiveQueue(Queue<Enemy*>* enemies) { this->inactive = enemies; }

void Battle::setNumberOfEnemies(int number) { this->EnemyCount = number; }

void Battle::RunSimulation()
{
	
	ReadInput *r= new ReadInput(this); //Reading input file

	PROG_MODE	mode = pGUI->getGUIMode();

	switch (mode)	//Add a function for each mode in next phases
	{
	case MODE_INTR:
		crnt_mode = new Interactive_mode(this);
		break;
	case MODE_STEP:
		crnt_mode = new StepByStep_mode(this);
		break;
	case MODE_SLNT:
		crnt_mode = new Silent_mode(this);
		break;
	}

	crnt_mode->Run();


	WriteOutput *w = new WriteOutput(this); //writing output file
	
	delete pGUI,r,w;
	
}




//Add enemy lists (inactive, active,.....) to drawing list to be displayed on user interface
void Battle::AddAllListsToDrawingList()
{	//////////////////////inactive//////////////////////
	int InactiveCount;
	Enemy* const* EnemyList = inactive->toArray(InactiveCount);
	for (int i = 0; i < InactiveCount; i++)
		pGUI->AddToDrawingList(EnemyList[i]);


	//////////////////////Active Fighters//////////////////////
	Fighter* currentFighter;
	Queue<Fighter*> tempFighters;

	while (!actv_fighters->isEmpty())
	{
		actv_fighters->dequeue(currentFighter);
		pGUI->AddToDrawingList(currentFighter);
		tempFighters.enqueue(currentFighter);


	}

	Fighter* currentFighterBack;
	while (!tempFighters.isEmpty())
	{
		tempFighters.dequeue(currentFighterBack);
		actv_fighters->enqueue(currentFighterBack);

	}

	////////////////////// Active Freezers//////////////////////
	Freezer* currentFreezer;
	Queue<Freezer*> tempFreezers;
	while (actv_freezers->dequeue(currentFreezer))
	{
		tempFreezers.enqueue(currentFreezer);
		pGUI->AddToDrawingList(currentFreezer);

	}

	Freezer* currentFreezerBack;
	while (tempFreezers.dequeue(currentFreezerBack))
	{
		actv_freezers->enqueue(currentFreezerBack);

	}

	////////////////////// Active Healers//////////////////////
	ArrayStack<Healer*> tempHealers(100);
	Healer* currentHealer;
	while (actv_healers->pop(currentHealer))
	{
		pGUI->AddToDrawingList(currentHealer);
		tempHealers.push(currentHealer);
	}

	Healer* currentHealerBack;
	while (tempHealers.pop(currentHealerBack))
	{
		actv_healers->push(currentHealerBack);
	}

	//////////////////////Frozen Enemies//////////////////////
	Enemy* currentEnemy;
	Queue<Enemy*> tempEnemies;

	while (!frzn_enms->isEmpty())
	{
		frzn_enms->dequeue(currentEnemy);
		pGUI->AddToDrawingList(currentEnemy);
		tempEnemies.enqueue(currentEnemy);


	}

	Enemy* currentEnemyBack;
	while (!tempEnemies.isEmpty())
	{
		tempEnemies.dequeue(currentEnemyBack);
		frzn_enms->enqueue(currentEnemyBack);

	}
	
	
	//////////////////////Killed Enemies//////////////////////
	
	Enemy* currentKldEnemy;
	Queue<Enemy*> tempKldEnemies;

	while (!kld_enms->isEmpty())
	{
		kld_enms->dequeue(currentKldEnemy);
		pGUI->AddToDrawingList(currentKldEnemy);
		tempKldEnemies.enqueue(currentKldEnemy);


	}

	Enemy* currentKldEnemyBack;
	while (!tempKldEnemies.isEmpty())
	{
		tempKldEnemies.dequeue(currentKldEnemyBack);
		kld_enms->enqueue(currentKldEnemyBack);

	}
	//////////////////////Super Soliders//////////////////////
	superSoliders* superDraw;
	Queue<superSoliders*> tempSuperDraw;
	while (!actv_SuperSoliders->isEmpty())
	{
		actv_SuperSoliders->pop(superDraw);
		pGUI->AddToDrawingList(superDraw);
		tempSuperDraw.enqueue(superDraw);
	}

	superSoliders* currentSuperSolider;
	while (!tempFighters.isEmpty())
	{
		tempSuperDraw.dequeue(currentSuperSolider);
		actv_SuperSoliders->push(currentSuperSolider);
	}
}

//check the inactive list and activate all enemies that has arrived
void Battle::ActivateEnemies()
{
	Enemy* currentEnemy;
	while (inactive->peekFront(currentEnemy))	//as long as there are more inactive enemies
	{
		if (currentEnemy->GetArrvTime() > CurrentTimeStep)	//no more arrivals at current time
			return;

		inactive->dequeue(currentEnemy);	//remove enemy from the queue
		currentEnemy->SetStatus(ACTV);	//make status active
		Fighter* fighter = dynamic_cast<Fighter*>(currentEnemy);
		Freezer* freezer = dynamic_cast<Freezer*>(currentEnemy);
		Healer* healer = dynamic_cast<Healer*>(currentEnemy);
		if (fighter != NULL)
		{

			actv_fighters->enqueue(fighter);
		}
		else if (freezer != NULL)
		{
			actv_freezers->enqueue(freezer);
		}

		else if (healer != NULL)
		{
			actv_healers->push(healer);

		}

	}
}

// Super solider activaton
void Battle::ActivateSuperSoliders(superSoliders* superSolider, int ID, int power, int arrTime, int reloadTime, int speed, double maxHealth)
{
	if (isSuperNeeded())
	{
		superSolider->setID(ID);
		superSolider->setPower(power); superSolider->setArrTime(getCurrentTimeStep()); superSolider->setReload(reloadTime);
		superSolider->setSpeed(speed); superSolider->setHealth(maxHealth);
		actv_SuperSoliders->push(superSolider);
		superSolider->setDistance(MinDistance);
		int initialDist = 2;
		int finalDistance = getMaxEnemDistance();
		//
		for (int dist = 2; dist <= finalDistance; dist++)
		{
			superSolider->setDistance(dist);
		}
	}
}

Queue<Enemy*>* Battle::getInactive()
{
	return  inactive;
}

PriorityQueue<Fighter*>* Battle::getActvFighters()
{
	return actv_fighters;
}

ArrayStack<Healer*>* Battle::getActvHealers()
{
	return  actv_healers;
}

Queue<Freezer*>* Battle::getActvFreezers()
{
	return actv_freezers;
}

PriorityQueue<Enemy*>* Battle::getFrzonEnimies()
{
	return frzn_enms;

}

Queue<Enemy*>* Battle::getKilledEnimies()
{
	return kld_enms;
}

void Battle::incrementTimeStep()
{
	CurrentTimeStep++;
}

GUI* Battle::getGUI()
{
	return pGUI;
}

int Battle::getCurrentTimeStep()
{
	return CurrentTimeStep;
}

int Battle::getEnemycount()
{
	return EnemyCount;
}


//functions used to count
int Battle::getNumActiveTotal()
{
	return getNumActivFighters() + getNumActivHealers() + getNumActivFreezeers();
}



/*int Battle::getNumFrostedTotal()
{

	return getNumFrostedFighters() + getNumFrostedHealers() + getNumFrostedFreezeers();
}
*/
int Battle::getNumActivFighters()
{
	int totalActive = 0;
	Fighter* currentFighter;
	Queue<Fighter*> tempFighters;

	while (!actv_fighters->isEmpty())
	{
		currentFighter = actv_fighters->dequeue();
		tempFighters.enqueue(currentFighter);

	}


	Fighter* currentFighter1;
	while (!tempFighters.isEmpty())
	{
		tempFighters.dequeue(currentFighter1);
		if (currentFighter->GetStatus() == ACTV)
			totalActive++;
		actv_fighters->enqueue(currentFighter1);

	}
	return totalActive;
}
//
int Battle::getNumActivFightersAtDist(int distance)
{
	int totalActive = 0;
	Fighter* currentFighter;
	Queue<Fighter*> tempFighters;

	while (!actv_fighters->isEmpty())
	{
		currentFighter = actv_fighters->dequeue();
		if (currentFighter->GetDistance() == distance)
		{
			tempFighters.enqueue(currentFighter);
		}
	}

	Fighter* currentFighter1;
	while (!tempFighters.isEmpty())
	{
		tempFighters.dequeue(currentFighter1);
		if (currentFighter->GetStatus() == ACTV)
			totalActive++;
		actv_fighters->enqueue(currentFighter1);
	}
	return totalActive;
}


int Battle::getNumActivHealers()
{
	int totalActive = 0;
	ArrayStack<Healer*> tempHealers(100);
	Healer* currentHealer;
	while (actv_healers->pop(currentHealer))
	{
		if (currentHealer->GetStatus() == ACTV)
			totalActive++;
		tempHealers.push(currentHealer);
	}

	Healer* currentHealer1;
	while (tempHealers.pop(currentHealer1))
	{
		actv_healers->push(currentHealer1);
	}
	return totalActive;
}
//
int Battle::getNumActivHealersAtDist(int distance)
{
	int totalActive = 0;
	ArrayStack<Healer*> tempHealers(100);
	Healer* currentHealer;

	while (actv_healers->pop(currentHealer))
	{
		if (currentHealer->GetStatus() == ACTV && currentHealer->GetDistance() == distance)
			totalActive++;
		tempHealers.push(currentHealer);
	}

	Healer* currentHealer1;
	while (tempHealers.pop(currentHealer1))
	{
		actv_healers->push(currentHealer1);
	}
	return totalActive;
}

int Battle::getNumActivFreezeersAtDist(int distance)
{
	int totalActive = 0;
	Freezer* currentFreezer;
	Queue<Freezer*> tempFreezers;
	while (actv_freezers->dequeue(currentFreezer))
	{
		if (currentFreezer->GetStatus() == ACTV && currentFreezer->GetDistance() == distance)
			totalActive++;
		tempFreezers.enqueue(currentFreezer);

	}
	Freezer* currentFreezer1;
	while (tempFreezers.dequeue(currentFreezer1))
		actv_freezers->enqueue(currentFreezer1);
	return totalActive;
}
//
int Battle::getNumActivFreezeers()
{
	int totalActive = 0;
	Freezer* currentFreezer;
	Queue<Freezer*> tempFreezers;
	while (actv_freezers->dequeue(currentFreezer))
	{
		if (currentFreezer->GetStatus() == ACTV)
			totalActive++;
		tempFreezers.enqueue(currentFreezer);

	}
	Freezer* currentFreezer1;
	while (tempFreezers.dequeue(currentFreezer1))
		actv_freezers->enqueue(currentFreezer1);
	return totalActive;
}
///////
int Battle::getNumActivTotalAtDist(int distance)
{
	return getNumActivFightersAtDist(distance) + getNumActivHealersAtDist(distance) + getNumActivFreezeersAtDist(distance);
}
/*
int Battle::getNumFrostedFighters()
{
	int totalFrosrted = 0;
	Fighter* currentFighter;
	Queue<Fighter*> tempFighters;

	while (!fighters->isEmpty())
	{
		currentFighter = fighters->dequeue();
		tempFighters.enqueue(currentFighter);

	}


	Fighter* currentFighter1;
	while (!tempFighters.isEmpty())
	{
		tempFighters.dequeue(currentFighter1);
		if (currentFighter->GetStatus() == FRST)
			totalFrosrted++;
		fighters->enqueue(currentFighter1);

	}
	return totalFrosrted;
}

int Battle::getNumFrostedHealers()
{
	int totalFrosrted = 0;
	ArrayStack<Healer*> tempHealers(100);
	Healer* currentHealer;
	while (healers->pop(currentHealer))
	{
		if (currentHealer->GetStatus() == FRST)
			totalFrosrted++;
		tempHealers.push(currentHealer);
	}

	Healer* currentHealer1;
	while (tempHealers.pop(currentHealer1))
	{
		healers->push(currentHealer1);
	}
	return totalFrosrted;

}

int Battle::getNumFrostedFreezeers()
{
	int totalFrosrted = 0;
	Freezer* currentFreezer;
	Queue<Freezer*> tempFreezers;
	while (freezers->dequeue(currentFreezer))
	{
		if (currentFreezer->GetStatus() == FRST)
			totalFrosrted++;
		tempFreezers.enqueue(currentFreezer);

	}
	Freezer* currentFreezer1;
	while (tempFreezers.dequeue(currentFreezer1))
		freezers->enqueue(currentFreezer1);

	return totalFrosrted;
}


int Battle::getNumKilled()
{

}
int Battle::getNumAlive()
{
}
*/

void Battle::LetCastleAttach(int crntTime)
{
	BCastle.Fight(this, crntTime);
}

//Bonus
int Battle::getMaxEnemDistance() 
{
	int distance;
	int finalDistance=0;
	for (distance = MinDistance; distance < MaxDistance; distance++)
	{
		for (int j = MinDistance; j < MaxDistance; j++)
		{
			if (getNumActivFightersAtDist(distance) > getNumActivFightersAtDist(j))
			{
				finalDistance = distance;
			}
		}
	}
	return finalDistance;
}

ArrayStack<superSoliders*>* Battle::getSupersoliders()
{
	return actv_SuperSoliders;
}
bool Battle::isSuperNeeded()
{
	if (GetCastle()->GetHealth() < 100)
	{
		isSuperNeededd = true;
		return true;
	}
	return false;
}
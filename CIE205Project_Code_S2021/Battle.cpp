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
	pGUI = new GUI;
	actv_fighters = new PriorityQueue<Fighter*>();
	actv_healers = new ArrayStack<Healer*>(100);
	actv_freezers = new Queue<Freezer*>;
	frzn_enms = new PriorityQueue<Enemy*>;
	kld_enms = new Queue<Enemy*>;
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

Queue<string>* Battle::StatusBarInformation()
{
	int c1,c2,c3,c4;
	Queue<string>* msgs = new Queue<string>;
	Queue<int>* tempQ = this->getNumFrznAndKilled();

	string output1 = "";
	output1 = ("Time Step: " + to_string(CurrentTimeStep)+"          "+"Castle Health: "+ to_string(this->GetCastle()->GetHealth())+"          ");
	if (this->GetCastle()->isCastleFreezed()) output1 += "Castle Is Freezed";
	else output1 += "Castle Is Active";

	string output2 = "Active Fighters: "+to_string(getNumActivFighters())+ "        Active Freezers: "+to_string(getNumActivFreezeers())
		+"        Active Healers"+ to_string(getNumActivHealers()) +"        Active Total"+ to_string(getNumActiveTotal()) ;


	tempQ->dequeue(c1), tempQ->dequeue(c2), tempQ->dequeue(c3), tempQ->dequeue(c4);
	string output3 ="Frozen Fighters: "+to_string(c1)+"        Frozen Freezers: "+to_string(c2)+"        Frozen Healers: "+to_string(c3)+"        Frozen Total: "+to_string(c4);

	tempQ->dequeue(c1), tempQ->dequeue(c2), tempQ->dequeue(c3), tempQ->dequeue(c4);
	string output4 = "Killed Fighters: " + to_string(c1) + "        Killed Freezers: " + to_string(c2) + "        Killed Healers: " + to_string(c3) + "        Killed Total: " + to_string(c4);


	msgs->enqueue(output1),msgs->enqueue(output2),msgs->enqueue(output3),msgs->enqueue(output4);
	delete tempQ;
	return msgs;

}

void Battle::LetCastleAttach(int crntTime)
{
	BCastle.Fight(this, crntTime);
}

//functions used to count
int Battle::getNumActiveTotal()
{
	return getNumActivFighters() + getNumActivHealers() + getNumActivFreezeers();
}



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



Queue<int>* Battle::getNumFrznAndKilled()
{
	Queue<int>* ints=new Queue<int>;
	PriorityQueue<Enemy*> frzn;
	Queue<Enemy*>* temp =new Queue<Enemy*>;
	Enemy* e;
	Fighter* fit = nullptr;
	Freezer* frz = nullptr;
	Healer* hel = nullptr;
	int c1, c2, c3, c4, c5, c6, c7, c8;
	c1 = c2 = c3 = c4 = c5 = c6 = c7 = c8 = 0;

	while (frzn_enms->dequeue(e))
	{
		fit = dynamic_cast<Fighter*>(e);
		frz = dynamic_cast<Freezer*>(e);
		hel = dynamic_cast<Healer*>(e);
		if (fit != nullptr) c1++;
		if (frz != nullptr) c2++;
		if (hel != nullptr) c3++;
		temp->enqueue(e);
	}
	while (temp->dequeue(e)) frzn_enms->enqueue(e);

	while (kld_enms->dequeue(e))
	{
		fit = dynamic_cast<Fighter*>(e);
		frz = dynamic_cast<Freezer*>(e);
		hel = dynamic_cast<Healer*>(e);
		if (fit != nullptr) c5++;
		if (frz != nullptr) c6++;
		if (hel != nullptr) c7++;
		temp->enqueue(e);
	}
	while (temp->dequeue(e)) kld_enms->enqueue(e);
	c4 = c1 + c2 + c3;
	c8 = c5 + c6 + c7;
	ints->enqueue(c1),ints->enqueue(c2),ints->enqueue(c3),ints->enqueue(c4),ints->enqueue(c5),ints->enqueue(c6),ints->enqueue(c7),ints->enqueue(c8);
	delete temp;
	return ints;
}



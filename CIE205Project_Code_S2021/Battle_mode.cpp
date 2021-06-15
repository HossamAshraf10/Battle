#include "Battle_mode.h"

Battle_mode::Battle_mode(Battle* battle)
{
	this->battle = battle;
	pGUI = battle->getGUI();
}






bool Battle_mode::isDone()
{
	if ( (battle->getInactive()->isEmpty() &&
		battle->getActvFighters()->isEmpty() &&
		battle->getActvHealers()->isEmpty() &&
		battle->getActvFreezers()->isEmpty() &&
		battle->getFrzonEnimies()->isEmpty() ) || battle->GetCastle()->GetHealth() <=0)
		return true;

	return false;
}

void Battle_mode::move_all_enimies()
{
	//I will move only the active enemies!!

	//Active fighters
	moveFighters();
	moveFreezers();
	moveHealers();

}
void Battle_mode::Act_all_enemies()
{
	actFighters();
}

void Battle_mode::actFighters() 
{
	static Castle* castle = battle->GetCastle();
	PriorityQueue<Fighter*> tempFighters;
	Fighter* currentFighter = NULL;
	PriorityQueue<Fighter*>* actv_fighters = battle->getActvFighters();


	while (!actv_fighters->isEmpty())
	{
		actv_fighters->dequeue(currentFighter);

		currentFighter->fight(castle);

		tempFighters.enqueue(currentFighter);
	}
	Fighter* currentFighterBack;
	while (!tempFighters.isEmpty())
	{
		tempFighters.dequeue(currentFighterBack);
		actv_fighters->enqueue(currentFighterBack);
	}
}
void Battle_mode::actHealers()
{
	ArrayStack<Healer*>* actv_healers = battle->getActvHealers();
	int size = actv_healers->getSize();
	ArrayStack<Healer*> tempHealers(size);
	Healer* currentHealer;


	while (actv_healers->pop(currentHealer))
	{
		currentHealer->Heal(battle);

		tempHealers.push(currentHealer);
	}

	Healer* currentHealerBack;
	while (tempHealers.pop(currentHealerBack))
	{
		actv_healers->push(currentHealerBack);
	}
}
void Battle_mode::actFreezers()
{
	Castle* castle = battle->GetCastle();
	Queue<Freezer*> tempFreezers;
	Freezer* currentFreezer;
	Queue<Freezer*>* actv_freezers = battle->getActvFreezers();

	while (actv_freezers->dequeue(currentFreezer))
	{
		currentFreezer->Freeze(castle);
		tempFreezers.enqueue(currentFreezer);
	}

	Freezer* tempFreezersBack;
	while (tempFreezers.dequeue(tempFreezersBack))
	{
		actv_freezers->enqueue(tempFreezersBack);
	}
}


void Battle_mode::moveFighters()
{
	PriorityQueue<Fighter*> tempFighters;
	Fighter* currentFighter = NULL;
	PriorityQueue<Fighter*>* actv_fighters=battle->getActvFighters();
	
	while (!actv_fighters->isEmpty())
	{
		actv_fighters->dequeue(currentFighter);

		currentFighter->Move();
		tempFighters.enqueue(currentFighter);
	}
	Fighter* currentFighterBack;
	while (!tempFighters.isEmpty())
	{
		tempFighters.dequeue(currentFighterBack);
		actv_fighters->enqueue(currentFighterBack);
	}
}

void Battle_mode::moveFreezers()
{
	Queue<Freezer*> tempFreezers;
	Freezer* currentFreezer;
	Queue<Freezer*>* actv_freezers = battle->getActvFreezers();

	while (actv_freezers->dequeue(currentFreezer))
	{
		currentFreezer->Move();
		tempFreezers.enqueue(currentFreezer);

	}

	Freezer* tempFreezersBack;
	while (tempFreezers.dequeue(tempFreezersBack))
	{
		actv_freezers->enqueue(tempFreezersBack);
	}
}


void Battle_mode::moveHealers()
{ 
	ArrayStack<Healer*>* actv_healers = battle->getActvHealers();
	int size= actv_healers->getSize();
	ArrayStack<Healer*> tempHealers(size);
	Healer* currentHealer;
	while (actv_healers->pop(currentHealer))
	{
		currentHealer->Move();
		tempHealers.push(currentHealer);

	}

	Healer* currentHealerBack;
	while (tempHealers.pop(currentHealerBack))
	{
		actv_healers->push(currentHealerBack);
	}
}

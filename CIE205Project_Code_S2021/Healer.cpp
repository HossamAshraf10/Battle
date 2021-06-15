#include "Healer.h"

void Healer::Move()
{
	static bool towardsCastle = true; 

	if (towardsCastle)
	{
		if (health <= 50) //not healthy
		{
			if (Distance - speed / 2 < MinDistance) //return back due to over flow
			{
				SetDistance(2 * MinDistance + speed / 2 - Distance);
				towardsCastle = false;
			}
			else SetDistance(Distance - speed / 2);
		}

		else //healthy 
		{
			if (Distance - speed < MinDistance) //return back due to over flow
			{
				SetDistance(2 * MinDistance + speed - Distance);
				towardsCastle = false;
			}
			else SetDistance(Distance - speed);
		}
	}

	else if (!towardsCastle)
	{
		if (health <= 50) //not healthy
		{
			if (Distance + speed > MaxDistance) //moves again towards castle
			{
				SetDistance(2 * MaxDistance - speed / 2 - Distance);
				towardsCastle = true;
			}

			else SetDistance(Distance + speed / 2);
		}

		else //healthy
		{
			if (Distance + speed > MaxDistance) //moves again towards castle
			{
				SetDistance(2 * MaxDistance - speed - Distance);
				towardsCastle = true;
			}

			else SetDistance(Distance + speed);
		}
	}
}

void Healer::Heal(Battle* battle)
{

	healFighters(battle);
	healFreezers(battle);
	healHealers(battle);

}

void Healer::_heal(Healer* healer, Enemy* healed)
{
	int dist;
	double k1,k2,k3;
	dist = healer->GetDistance()- healed->GetDistance();

	if (dist <= 2 && dist >= -2) //min 0.25, max 2
	{

		if (healed->GetHealth() <= 50) k1 = 0.5;
		else k1 = 1;

		if (healer->GetHealth() <= 50) k2 = 0.5;
		else k2 = 1;
		
		if (dist = 0) k3 = 2;
		else if (dist = 1 || dist == -1) k3 = 1.5;
		else k3 = 1;
	}
	healed->SetHealth(healed->GetHealth()+k1*k2*k3);
}

void Healer::healFighters(Battle* battle)
{
	static Castle* castle = battle->GetCastle();
	PriorityQueue<Fighter*> tempFighters;
	Fighter* currentFighter = NULL;
	PriorityQueue<Fighter*>* actv_fighters = battle->getActvFighters();
	

	while (!actv_fighters->isEmpty())
	{
		actv_fighters->dequeue(currentFighter);
		
		if (!currentFighter->isHealthFul()) _heal(this, currentFighter);

		tempFighters.enqueue(currentFighter);
	}
	Fighter* currentFighterBack;
	while (!tempFighters.isEmpty())
	{
		tempFighters.dequeue(currentFighterBack);
		actv_fighters->enqueue(currentFighterBack);
	}
}
void Healer::healHealers(Battle* battle)
{
	ArrayStack<Healer*>* actv_healers = battle->getActvHealers();
	int size = actv_healers->getSize();
	ArrayStack<Healer*> tempHealers(size);
	Healer* currentHealer;

	while (actv_healers->pop(currentHealer))
	{
		
		if (!currentHealer->isHealthFul()) _heal(this, currentHealer);

		tempHealers.push(currentHealer);

	}

	Healer* currentHealerBack;
	while (tempHealers.pop(currentHealerBack))
	{
		actv_healers->push(currentHealerBack);
	}
}
void Healer::healFreezers(Battle* battle)
{
	Castle* castle = battle->GetCastle();
	Queue<Freezer*> tempFreezers;
	Freezer* currentFreezer;
	Queue<Freezer*>* actv_freezers = battle->getActvFreezers();

	while (actv_freezers->dequeue(currentFreezer))
	{

		if (!currentFreezer->isHealthFul()) _heal(this, currentFreezer);

		tempFreezers.enqueue(currentFreezer);

	}

	Freezer* tempFreezersBack;
	while (tempFreezers.dequeue(tempFreezersBack))
	{
		actv_freezers->enqueue(tempFreezersBack);
	}
}

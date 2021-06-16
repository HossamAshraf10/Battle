#include "SuperSoliders.h"
#include <iostream>

using namespace std;

//Constructor
superSoliders::superSoliders()
{
	this->distance = 0; this->power = 0; this->arrTime = 0; this->reloadTime = 0; this->speed = 0; this->health = maxHealth; this->ID = 0;
}
superSoliders::superSoliders(int ID, int power, int arrTime, int reloadTime, int speed, double maxHealth)
{
	this->ID = ID; this->power = power; this->arrTime = arrTime; this->reloadTime = reloadTime; this->speed = speed; this->maxHealth = maxHealth;
	health = maxHealth;
	this->distance = 0;

}

//Getters
int superSoliders::getID() const { return ID;}
int superSoliders::getArrTime() const { return arrTime;}
int superSoliders::getPower() const { return power;}
double superSoliders::getHealth() const { return health;}
int superSoliders::getReload() const { return reloadTime;}
int superSoliders::getSpeed() const { return speed;}
double superSoliders::getMaxHealth() const { return maxHealth;}
int superSoliders::getDistnce() const { return distance; }

//Setters
void superSoliders::setID(int ID) { this->ID = ID; }
void  superSoliders::setArrTime(int arrTime) { this->arrTime = arrTime; }
void  superSoliders::setPower(int power) { this->power = power; }
void  superSoliders::setReload(int reloadTime) { this->reloadTime = reloadTime; }
void  superSoliders::setSpeed(int speed) { this->speed = speed; }
void  superSoliders::setHealth(double health) { this->health = health; }
void  superSoliders::setDistance(int distance) { this->distance = distance; }

//Functions
/*
void superSoliders::Move()
{

}

void superSoliders::Act()
{

}
*/

void superSoliders::Fight(Battle* battle, int curntTime)
{
	int numAttacked = 0;
	numAttacked += attackFighters(battle->getActvFighters(), battle->getKilledEnimies(), 3,curntTime);

	numAttacked += attackHealers(battle->getActvHealers(), battle->getKilledEnimies(), 3, curntTime);
	
	numAttacked += attackFreezers(battle->getActvFreezers(), battle->getKilledEnimies(), 3, curntTime);
}

int superSoliders::attackFighters(PriorityQueue<Fighter*>* actvFighters, Queue<Enemy*>* kld_enms, int max, int crntTime)
{
	PriorityQueue<Fighter*>* tmpQ = new PriorityQueue<Fighter*>();
	int totalAttacked = 0;
	while (!actvFighters->isEmpty())
	{
		Fighter* tmpFighter = actvFighters->dequeue();
		if (IsWorthFighterAttack(tmpFighter, ACTV) && totalAttacked <= max)
		{
			totalAttacked++;
			tmpFighter->SetHealth(0);
			//setFirstShotTime(tmpFighter, crntTime);

			cout << "Kld: " << tmpFighter->GetID() << endl;
			kld_enms->enqueue(tmpFighter);
			tmpFighter->SetStatus(KILD);
		}
	}

	//putting them back:
	while (!tmpQ->isEmpty())
	{
		Fighter* tmpFighter = tmpQ->dequeue();
		actvFighters->enqueue(tmpFighter);
	}

	return totalAttacked;
}
bool superSoliders::IsWorthFighterAttack(Fighter* fighter, ENMY_STATUS status)
{
	if ((fighter->GetDistance() - getDistnce() >= 3 && fighter->GetDistance() - getDistnce() <= 0) || (fighter->GetDistance() - getDistnce() >=  -3 && fighter->GetDistance() - getDistnce() <= 0) )
	{
		double totalPercentage = 0;
		if (status == ACTV) totalPercentage += 30;
		if (status == FRST) totalPercentage += 15;
		if ((fighter->GetHealth() / fighter->GetMaxHealth()) * 100 >= 50) totalPercentage += 30;
		//if (fighter->GetHealth() < 50) totalPercentage += 15;
		totalPercentage = totalPercentage + ((60 - fighter->GetDistance()) / 60.0) * 25;
		totalPercentage = totalPercentage + (fighter->GetPower() / 100.0) * 15;
		if (totalPercentage > 40) return true;
	}
	return false;
}
int superSoliders::attackHealers(ArrayStack<Healer*>* healers, Queue<Enemy*>* kld_enms, int max, int crntTime)
{
	ArrayStack<Healer*>* tmpH = new ArrayStack<Healer*>(healers->getSize());
	int totalAttacked = 0;
	while (!healers->isEmpty())
	{
		Healer* tmpHealer;
		healers->pop(tmpHealer);
		if (totalAttacked < max)
		{
			totalAttacked++;
			tmpHealer->SetHealth(0);
			//setFirstShotTime(tmpHealer, crntTime);
		}

		cout << "Kld: " << tmpHealer->GetID() << endl;
		kld_enms->enqueue(tmpHealer);
		tmpHealer->SetStatus(KILD);

		
	}
	//reverse
	while (!tmpH->isEmpty())
	{
		Healer* tmpHealer;
		tmpH->pop(tmpHealer);
		healers->push(tmpHealer);
	}
	return totalAttacked;
}
int superSoliders::attackFreezers(Queue<Freezer*>* actv_freezers, Queue<Enemy*>* kld_enms, int max, int crntTime)
{
	Queue<Freezer*>* tmpF = new Queue<Freezer*>();
	int totalAttacked = 0;
	while (!actv_freezers->isEmpty())
	{
		Freezer* tmpFreezer;
		actv_freezers->dequeue(tmpFreezer);
		if (totalAttacked < max)
		{
			totalAttacked++;
			tmpFreezer->SetHealth(0);
			//setFirstShotTime(tmpFreezer, crntTime);
		}
		
		kld_enms->enqueue(tmpFreezer);
		//kld_enms->enqueue(tmpFreezer);
		tmpFreezer->SetStatus(KILD);
		
	}
	//reverse
	while (!tmpF->isEmpty())
	{
		Freezer* tmpFreezer;
		tmpF->dequeue(tmpFreezer);
		actv_freezers->enqueue(tmpFreezer);
	}
	return totalAttacked;
}
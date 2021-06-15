#include "Castle.h"
#include "..\Battle.h"

#include <iostream>
using namespace std;

Castle::Castle()
{
	Health = 0;
	power = 0;
	N = 0;
}

Castle::Castle(double Health, int N, double power)
{
	SetOriginalHealth(Health);
	this->Health = Health;
	this->N = N;
	this->power = power;
	this->accumulatedIce = 0;
	IceThreshold = originalHealth / 4;
}

bool Castle::isCastleFreezed()
{
	return (getAccumulatedIce() > IceThreshold);
}

double Castle::getAccumulatedIce() { return accumulatedIce; }

void Castle::accumulateIce(double ice) { accumulatedIce += ice; }


void Castle::SetHealth(double h)
{
	if (h > 0)
		Health = h;
	else
		Health = 0; // killed
}

void Castle::SetN(int N)
{
	this->N = N;
}

void Castle::SetPower(double power)
{
	this->power = power;
}

double Castle::GetHealth() const
{
	return Health;
}

double Castle::GetN() const
{
	return N;
}

double Castle::GetPower() const
{
	return power;
}

void Castle::SetOriginalHealth(double h)
{
	originalHealth = h;
}

double Castle::GetOriginalHealth() const
{
	return originalHealth;
}


/*
	Formula for percentage to attack
	30% For status --> if actv then 30%. if frosted get 15% . if kld =0%
	30% for health --> if health>50 then 30%. if less than 50 then 15%
	25% for distance--> ((60-dist)/60)*25 --> so, if distance is equal to 0, then 25 completely. if not the will be partial fo 25%
	15% for power--> (power/10 )*15
	total:= 30+30+25+15=100
	now to attack a fighter the percentage must be greater than 60% :)
*/
void Castle::Fight(Battle* battle, int curntTime)
{
	int numAttacked = 0;
	numAttacked += AttackFighters(battle->getActvFighters(), battle->getKilledEnimies(), N, curntTime);
	if (numAttacked < N)
		//Attack healers
		numAttacked += AttachHealers(battle->getActvHealers(), battle->getKilledEnimies(), N, curntTime);
	if (numAttacked < N)
		//Attack freezers
		AttackFrozen(battle->getActvFreezers(), battle->getKilledEnimies(), N, curntTime);

}

int Castle::AttackFighters(PriorityQueue<Fighter*>* actvFighters, Queue<Enemy*>* kld_enms, int max, int crntTime)
{
	PriorityQueue<Fighter*>* tmpQ = new PriorityQueue<Fighter*>();
	int totalAttacked = 0;
	while (!actvFighters->isEmpty())
	{
		Fighter* tmpFighter = actvFighters->dequeue();
		if (IsWorthFighterAttack(tmpFighter, ACTV) && totalAttacked <= max)
		{
			totalAttacked++;
			tmpFighter->SetHealth(tmpFighter->GetHealth() - GetDamagetToFighter(tmpFighter));
			setFirstShotTime(tmpFighter, crntTime);

		}
		if (tmpFighter->GetHealth() > 0)
			tmpQ->enqueue(tmpFighter);
		else
		{
			cout << "Kld: "<<tmpFighter->GetID()<<endl;
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

bool Castle::IsWorthFighterAttack(Fighter* fighter, ENMY_STATUS status)
{
	double totalPercentage = 0;
	if (status == ACTV) totalPercentage += 30;
	if (status == FRST) totalPercentage += 15;
	if ((fighter->GetHealth()/fighter->GetMaxHealth())*100 >= 50) totalPercentage += 30;
	//if (fighter->GetHealth() < 50) totalPercentage += 15;
	totalPercentage = totalPercentage+((60 - fighter->GetDistance()) / 60.0) * 25;
	totalPercentage = totalPercentage + (fighter->GetPower() / 100.0) * 15;
	if (totalPercentage > 40) return true;
	return false;
}

double Castle::GetDamagetToFighter(Fighter* fighter)
{
	double k = 1;
	return (1.0 / fighter->GetDistance()) * this->GetPower() * (1.0 / k);
}

int Castle::GetDamagetToOthers(Enemy* enmy)
{
	int k = 2;
	return (1.0 / enmy->GetDistance()) * this->GetPower() * (1.0 / k);
}

int Castle::AttachHealers(ArrayStack<Healer*>* healers, Queue<Enemy*>* kld_enms, int max, int crntTime)
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
			tmpHealer->SetHealth(tmpHealer->GetHealth() - GetDamagetToOthers(tmpHealer));
			setFirstShotTime(tmpHealer, crntTime);
		}
		if (tmpHealer->GetHealth() > 0)
			tmpH->push(tmpHealer);
		else
		{
			cout << "Kld: " << tmpHealer->GetID() << endl;
			kld_enms->enqueue(tmpHealer);
			tmpHealer->SetStatus(KILD);

		}
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

int Castle::AttackFrozen(Queue<Freezer*>* actv_freezers, Queue<Enemy*>* kld_enms, int max, int crntTime)
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
			tmpFreezer->SetHealth(tmpFreezer->GetHealth() - GetDamagetToOthers(tmpFreezer));
			setFirstShotTime(tmpFreezer, crntTime);
		}
		if (tmpFreezer->GetHealth() > 0)
			tmpF->enqueue(tmpFreezer);
		else
		{
			kld_enms->enqueue(tmpFreezer);
			//kld_enms->enqueue(tmpFreezer);
			tmpFreezer->SetStatus(KILD);
		}
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

void Castle::setFirstShotTime(Enemy* enmy, int curntTIme)
{
	if (enmy->GetFirstShotTime() != -1)
		enmy->SetFirstShotTime(curntTIme);
}

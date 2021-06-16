#include "Castle.h"
#include "..\Battle.h"

#include <iostream>

#include <stdlib.h>
#include <ctime>


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
}

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
	if (this->isCastleFreezed())
	{
		this->UnfreezeCastle();
		return;
	}
	UnfreezeEnms(battle, curntTime);
	srand(time(0));
	double result = 0 + (std::rand() % (100 - 0 + 1));
	if (result > 20)
		FightBullets(battle, curntTime);
	else
		FightIce(battle, curntTime);


}

void Castle::FightBullets(Battle* battle, int curntTime)
{
	int numAttacked = 0;
	numAttacked += AttackFighters(battle->getActvFighters(), battle->getKilledEnimies(), N-1, curntTime);
	if (numAttacked < N)
		numAttacked += AttackFrozenFighters(battle->getFrzonEnimies(), battle->getKilledEnimies(), N-1, curntTime, numAttacked);

	//Attack healers
	if (numAttacked < N)
		numAttacked += AttachHealers(battle->getActvHealers(), battle->getKilledEnimies(), N-1, curntTime, numAttacked);
	if (numAttacked < N)
		numAttacked += AttachFrozenHealers(battle->getFrzonEnimies(), battle->getKilledEnimies(), N-1, curntTime, numAttacked);

	//Attack freezers
	if (numAttacked < N)
		AttackFrozen(battle->getActvFreezers(), battle->getKilledEnimies(), N-1, curntTime, numAttacked);
	if (numAttacked < N)
		numAttacked += AttackFrozednFrozen(battle->getFrzonEnimies(), battle->getKilledEnimies(), N-1, curntTime, numAttacked);
}

void Castle::FightIce(Battle* battle, int curntTime)
{
	int numAttacked = 0;
	numAttacked += AttackFightersByIce(battle->getActvFighters(), battle->getFrzonEnimies(), N, curntTime, numAttacked);
	if (numAttacked < N)
		//Attack healers
		numAttacked += AttachHealersByIce(battle->getActvHealers(), battle->getFrzonEnimies(), N, curntTime, numAttacked);
	if (numAttacked < N)
		//Attack freezers
		AttackFrozen(battle->getActvFreezers(), battle->getKilledEnimies(), N, curntTime, numAttacked);
}

/*
	Fighters Take 3 time step to unfreeze
	healers take 2
	freezers take 1
*/
void Castle::UnfreezeEnms(Battle* battle, int curntTime)
{
	PriorityQueue<Enemy*>* frzn = battle->getFrzonEnimies();
	PriorityQueue<Enemy*>* tmpFrzn = new PriorityQueue<Enemy*>();
	while (!frzn->isEmpty())
	{
		Enemy* tmpEmy;
		frzn->dequeue(tmpEmy);
		Fighter* fighter = dynamic_cast<Fighter*>(tmpEmy);
		Freezer* freezer = dynamic_cast<Freezer*>(tmpEmy);
		Healer* healer = dynamic_cast<Healer*>(tmpEmy);
		if (fighter != nullptr)
		{
			if ((curntTime - tmpEmy->GetFrostingTime()) > 3)
			{
				battle->getActvFighters()->enqueue(fighter);
				fighter->SetStatus(ACTV);
			}
			else
			{
				tmpFrzn->enqueue(fighter);
			}
		}

		else if (freezer != nullptr)
		{
			if ((curntTime - tmpEmy->GetFrostingTime()) > 2)
			{
				battle->getActvFreezers()->enqueue(freezer);
				freezer->SetStatus(ACTV);
			}
			else
			{
				tmpFrzn->enqueue(freezer);
			}
		}
		else if (healer != nullptr)
		{
			if ((curntTime - tmpEmy->GetFrostingTime()) > 2)
			{
				battle->getActvHealers()->push(healer);
				healer->SetStatus(ACTV);
			}
			else
			{
				tmpFrzn->enqueue(healer);
			}
		}

	}

	//reverse
	while (!tmpFrzn->isEmpty())
	{
		Enemy* tmpEmy;
		tmpFrzn->dequeue(tmpEmy);
		battle->getFrzonEnimies()->enqueue(tmpEmy);
	}

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
			tmpFighter->SetHealth(tmpFighter->GetHealth() - GetDamagetToOthers(tmpFighter));
			setFirstShotTime(tmpFighter, crntTime);

		}
		if (tmpFighter->GetHealth() > 0)
		{
			tmpQ->enqueue(tmpFighter);

		}
		else
		{
			cout << "Kld: " << tmpFighter->GetID() << endl;
			kld_enms->enqueue(tmpFighter);
			tmpFighter->SetStatus(KILD);
			tmpFighter->SetKillTime(crntTime);

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

int Castle::AttackFrozenFighters(PriorityQueue<Enemy*>* frzn_enms, Queue<Enemy*>* kld_enms, int max, int crntTime, int alreadyKld)
{
	PriorityQueue<Enemy*>* tmpFrzns = new PriorityQueue<Enemy*>();
	int totalAttacked = alreadyKld;
	while (!frzn_enms->isEmpty())
	{
		Enemy* tmpEmy;
		frzn_enms->dequeue(tmpEmy);
		Fighter* fighter = dynamic_cast<Fighter*>(tmpEmy);
		Freezer* freezer = dynamic_cast<Freezer*>(tmpEmy);
		Healer* healer = dynamic_cast<Healer*>(tmpEmy);
		if (fighter != nullptr)
		{
			if (IsWorthFighterAttack(fighter, ACTV) && totalAttacked <= max)
			{
				totalAttacked++;
				fighter->SetHealth(fighter->GetHealth() - GetDamagetToOthers(fighter));

			}
			if (fighter->GetHealth() > 0)
			{
				tmpFrzns->enqueue(fighter);

			}
			else
			{
				cout << "Kld: " << fighter->GetID() << endl;
				kld_enms->enqueue(fighter);
				fighter->SetStatus(KILD);
				fighter->SetKillTime(crntTime);

			}
		}
		else
		{
			tmpFrzns->enqueue(tmpEmy);
		}



	}

	//reverse

	while (!tmpFrzns->isEmpty())
	{
		Enemy* tmpEnmy = tmpFrzns->dequeue();
		frzn_enms->enqueue(tmpEnmy);

	}
	return totalAttacked;
}

bool Castle::IsWorthFighterAttack(Fighter* fighter, ENMY_STATUS status)
{
	double totalPercentage = 0;
	if (status == ACTV) totalPercentage += 30;
	if (status == FRST) totalPercentage += 15;
	if ((fighter->GetHealth() / fighter->GetMaxHealth()) * 100 >= 50) totalPercentage += 30;
	//if (fighter->GetHealth() < 50) totalPercentage += 15;
	totalPercentage = totalPercentage + ((60 - fighter->GetDistance()) / 60.0) * 25;
	totalPercentage = totalPercentage + (fighter->GetPower() / 100.0) * 15;
	if (totalPercentage > 40) return true;
	return false;
}

double Castle::GetDamagetToHealers(Healer* fighter)
{
	double k = 1;
	return (1.0 / fighter->GetDistance()) * this->GetPower() * (1.0 / k);
}

int Castle::GetDamagetToOthers(Enemy* enmy)
{
	double k = 2;
	return (1.0 / enmy->GetDistance()) * this->GetPower() * (1.0 / k);
}

int Castle::AttachHealers(ArrayStack<Healer*>* healers, Queue<Enemy*>* kld_enms, int max, int crntTime, int alreadyKld)
{
	ArrayStack<Healer*>* tmpH = new ArrayStack<Healer*>(healers->getSize());
	int totalAttacked = alreadyKld;
	while (!healers->isEmpty())
	{
		Healer* tmpHealer;
		healers->pop(tmpHealer);
		if (totalAttacked < max)
		{
			totalAttacked++;
			tmpHealer->SetHealth(tmpHealer->GetHealth() - GetDamagetToHealers(tmpHealer));
			setFirstShotTime(tmpHealer, crntTime);
		}
		if (tmpHealer->GetHealth() > 0)
		{
			tmpH->push(tmpHealer);
		}
		else
		{
			cout << "Kld: " << tmpHealer->GetID() << endl;
			kld_enms->enqueue(tmpHealer);
			tmpHealer->SetStatus(KILD);
			tmpHealer->SetKillTime(crntTime);


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

int Castle::AttachFrozenHealers(PriorityQueue<Enemy*>* frzn_enms, Queue<Enemy*>* kld_enms, int max, int crntTime, int alreadyKld)
{
	PriorityQueue<Enemy*>* tmpFrzns = new PriorityQueue<Enemy*>();
	int totalAttacked = alreadyKld;
	while (!frzn_enms->isEmpty())
	{
		Enemy* tmpEmy;
		frzn_enms->dequeue(tmpEmy);

		Fighter* fighter = dynamic_cast<Fighter*>(tmpEmy);
		Freezer* freezer = dynamic_cast<Freezer*>(tmpEmy);
		Healer* healer = dynamic_cast<Healer*>(tmpEmy);
		if (healer != nullptr)
		{
			if (totalAttacked <= max)
			{
				totalAttacked++;
				healer->SetHealth(healer->GetHealth() - GetDamagetToHealers(healer));

			}
			if (healer->GetHealth() > 0)
			{
				tmpFrzns->enqueue(healer);

			}
			else
			{
				cout << "Kld: " << healer->GetID() << endl;
				kld_enms->enqueue(healer);
				healer->SetStatus(KILD);
				healer->SetKillTime(crntTime);

			}
		}
		else
		{
			tmpFrzns->enqueue(tmpEmy);
		}



	}

	//reverse

	while (!tmpFrzns->isEmpty())
	{
		Enemy* tmpEnmy = tmpFrzns->dequeue();
		frzn_enms->enqueue(tmpEnmy);

	}
	return totalAttacked;

}

int Castle::AttackFrozen(Queue<Freezer*>* actv_freezers, Queue<Enemy*>* kld_enms, int max, int crntTime, int alreadyKld)
{
	Queue<Freezer*>* tmpF = new Queue<Freezer*>();
	int totalAttacked = alreadyKld;
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
		{
			tmpF->enqueue(tmpFreezer);
		}
		else
		{
			kld_enms->enqueue(tmpFreezer);
			//kld_enms->enqueue(tmpFreezer);
			tmpFreezer->SetStatus(KILD);
			tmpFreezer->SetKillTime(crntTime);

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

int Castle::AttackFrozednFrozen(PriorityQueue<Enemy*>* frzn_enms, Queue<Enemy*>* kld_enms, int max, int crntTime, int alreadyKld)
{
	PriorityQueue<Enemy*>* tmpFrzns = new PriorityQueue<Enemy*>();
	int totalAttacked = alreadyKld;
	while (!frzn_enms->isEmpty())
	{
		Enemy* tmpEmy;
		frzn_enms->dequeue(tmpEmy);

		Fighter* fighter = dynamic_cast<Fighter*>(tmpEmy);
		Freezer* freezer = dynamic_cast<Freezer*>(tmpEmy);
		Healer* healer = dynamic_cast<Healer*>(tmpEmy);
		if (freezer != nullptr)
		{
			if (totalAttacked <= max)
			{
				totalAttacked++;
				freezer->SetHealth(freezer->GetHealth() - GetDamagetToOthers(freezer));

			}
			if (freezer->GetHealth() > 0)
			{
				tmpFrzns->enqueue(freezer);

			}
			else
			{
				cout << "Kld: " << freezer->GetID() << endl;
				kld_enms->enqueue(freezer);
				freezer->SetStatus(KILD);
				freezer->SetKillTime(crntTime);

			}
		}
		else
		{
			tmpFrzns->enqueue(tmpEmy);
		}



	}

	//reverse

	while (!tmpFrzns->isEmpty())
	{
		Enemy* tmpEnmy = tmpFrzns->dequeue();
		frzn_enms->enqueue(tmpEnmy);

	}
	return totalAttacked;

}

void Castle::setFirstShotTime(Enemy* enmy, int curntTIme)
{
	if (enmy->GetFirstShotTime() == -1)
		enmy->SetFirstShotTime(curntTIme);
}

int Castle::AttackFightersByIce(PriorityQueue<Fighter*>* actvFighters, PriorityQueue<Enemy*>* frzn_enms, int max, int crntTime, int alrdyKild)
{
	PriorityQueue<Fighter*>* tmpQ = new PriorityQueue<Fighter*>();
	int totalAttacked = alrdyKild;
	while (!actvFighters->isEmpty())
	{
		Fighter* tmpFighter = actvFighters->dequeue();
		if (IsWorthFighterAttack(tmpFighter, ACTV) && totalAttacked <= max)
		{
			totalAttacked++;
			setFirstShotTime(tmpFighter, crntTime);
			tmpFighter->SetStatus(FRST);
			setFirstShotTime(tmpFighter, crntTime);


		}
		if (tmpFighter->GetStatus() == FRST)
		{
			frzn_enms->enqueue(tmpFighter);
			tmpFighter->SetFrostingTime(crntTime);

		}
		else
		{
			cout << "Frozen: " << tmpFighter->GetID() << endl;
			tmpQ->enqueue(tmpFighter);

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

int Castle::AttachHealersByIce(ArrayStack<Healer*>* healers, PriorityQueue<Enemy*>* frzn_enms, int max, int crntTime, int alrdyKild)
{
	ArrayStack<Healer*>* tmpH = new ArrayStack<Healer*>(healers->getSize());
	int totalAttacked = alrdyKild;
	while (!healers->isEmpty())
	{
		Healer* tmpHealer;
		healers->pop(tmpHealer);
		if (totalAttacked < max)
		{
			totalAttacked++;
			setFirstShotTime(tmpHealer, crntTime);
			tmpHealer->SetStatus(FRST);
			setFirstShotTime(tmpHealer, crntTime);
		}
		if (tmpHealer->GetStatus() == FRST)
		{
			frzn_enms->enqueue(tmpHealer);
			tmpHealer->SetFrostingTime(crntTime);


		}
		else
		{
			cout << "Frozen: " << tmpHealer->GetID() << endl;
			tmpH->push(tmpHealer);

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

int Castle::AttackFrozenByIce(Queue<Freezer*>* actv_freezers, PriorityQueue<Enemy*>* frzn_enms, int max, int crntTime)
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
			setFirstShotTime(tmpFreezer, crntTime);
			tmpFreezer->SetStatus(FRST);
			setFirstShotTime(tmpFreezer, crntTime);
		}
		if (tmpFreezer->GetStatus() == FRST)
		{
			frzn_enms->enqueue(tmpFreezer);
			tmpFreezer->SetFrostingTime(crntTime);

		}
		else
		{
			cout << "Frozen: " << tmpFreezer->GetID() << endl;
			tmpF->enqueue(tmpFreezer);
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



bool Castle::isCastleFreezed()
{
	return (getAccumulatedIce() > IceThreshold);
}

void Castle::UnfreezeCastle()
{
	accumulatedIce = 0;
}

double Castle::getAccumulatedIce() { return accumulatedIce; }

void Castle::accumulateIce(double ice) { accumulatedIce += ice; }
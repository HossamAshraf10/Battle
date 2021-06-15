#pragma once
#ifndef  CASTLE_H
#define CASTLE_H


#include "..\Defs.h"
#include"..\Generic_DS\ArrayStack.h"
#include "../Fighter.h"
#include "..\Generic_DS\PriorityQueue.h"
#include "..\Generic_DS\Queue.h"

class Battle;
class Fighter;
class Healer;
class Freezer;

class Castle
{
	double Health;
	int N; //The number of eniemes attacked per attack
	double power; // the power of the fire
	double originalHealth;
	//ArrayStack<int> Bullets(500);
	//ArrayStack<int> Ice(500);
	//
	// TODO: Add More Data Members As Needed
	//

	int AttackFighters(PriorityQueue<Fighter*>*, Queue<Enemy*>* kld_enms, int max, int crntTime);
	int AttackFrozenFighters(PriorityQueue<Enemy*>* frzn_enms, Queue<Enemy*>* kld_enms, int max, int crntTime, int alreadyKld);
	bool IsWorthFighterAttack(Fighter* fighter, ENMY_STATUS status);
	double GetDamagetToFighter(Fighter* fighter);
	int GetDamagetToOthers(Enemy* fighter);
	int AttachHealers(ArrayStack<Healer*>*, Queue<Enemy*>* kld_enms, int max, int crntTime,  int alreadyKld);
	int AttachFrozenHealers(PriorityQueue<Enemy*>* frzn_enms, Queue<Enemy*>* kld_enms, int max, int crntTime,  int alreadyKld);

	int AttackFrozen(Queue<Freezer*>* actv_freezers, Queue<Enemy*>* kld_enms, int max, int crntTime, int alreadyKld);
	int AttackFrozednFrozen(PriorityQueue<Enemy*>* frzn_enms, Queue<Enemy*>* kld_enms, int max, int crntTime, int alreadyKld);
	void setFirstShotTime(Enemy* enmy, int curntTIme);


	int AttackFightersByIce(PriorityQueue<Fighter*>*, PriorityQueue<Enemy*>* frzn_enms, int max, int crntTime, int alrdyKild);
	int AttachHealersByIce(ArrayStack<Healer*>*, PriorityQueue<Enemy*>* frzn_enms, int max, int crntTime, int alrdyKild);
	int AttackFrozenByIce(Queue<Freezer*>* actv_freezers, PriorityQueue<Enemy*>* frzn_enms, int max, int crntTime);




	void FightBullets(Battle* battle, int curntTime);
	void FightIce(Battle* battle, int curntTime);
	void UnfreezeEnms(Battle* battle, int curntTime);


public:
	Castle();
	Castle(double Health, int N, double power);
	void SetHealth(double h);
	void SetN(int N);
	void SetPower(double power);
	double GetHealth() const;
	double GetN() const;
	double GetPower() const;

	double GetOriginalHealth() const;
	void SetOriginalHealth(double h);

	//void Fight(Battle* battle);
	//
	// TODO: Add More Member Functions As Needed
	//
	void Fight(Battle* battle, int curntTime);
	


};

#endif
#pragma once
#ifndef  SUPERSOLIDERS_H
#define SUPERSOLIDERS_H


//#include "..\Defs.h"
//#include"..\Generic_DS\ArrayStack.h"
//#include "../Fighter.h"
//#include "..\Generic_DS\PriorityQueue.h"
//#include "..\Generic_DS\Queue.h"
#include "..\Battle.h"

class superSoliders
{
private:
	int distance;
protected:
	int ID, power, arrTime, reloadTime, speed;
	double health, maxHealth=3000;
	//ArrayStack<superSoliders*> soliders[3];
public:
	//Constructor
	superSoliders();
	superSoliders(int ID, int power, int arrTime, int reloadTime, int speed, double maxHealth);

	//Getters
	int getID() const;
	int getPower() const;
	int getArrTime() const;
	int getReload() const;
	int getSpeed() const;
	double getHealth() const;
	double getMaxHealth() const;
	int getDistnce() const;
	
	//Setters
	void setID(int ID);
	void setPower(int power);
	void setArrTime(int arrTime);
	void setReload(int Reload);
	void setSpeed(int speed);
	void setHealth(double health);
	void setDistance(int distance);

	//Functions
	//void Move();
	//void Act();
	void Fight(Battle* battle, int curntTime);
	int attackFighters(PriorityQueue<Fighter*>* actvFighters, Queue<Enemy*>* kld_enms, int max, int crntTime);
	int attackHealers(ArrayStack<Healer*>* healers, Queue<Enemy*>* kld_enms, int max, int crntTime);
	int attackFreezers(Queue<Freezer*>* actv_freezers, Queue<Enemy*>* kld_enms, int max, int crntTime);
	bool IsWorthFighterAttack(Fighter* fighter, ENMY_STATUS status);
};


#endif
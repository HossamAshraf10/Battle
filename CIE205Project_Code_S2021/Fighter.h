#pragma once
#include "Enemies/Enemy.h"

class Castle;

class Fighter : public Enemy
{
private: int waitTillNextShot;
public:
	Fighter(int id, ENMY_TYPE type, int arrTime, double health, int power,
		int reload, int speed) : Enemy(id, type, arrTime, health, power, reload, speed) {
		waitTillNextShot = reload;
	}

	void Move();
	void Act();
	void fight(Castle* castle);
};


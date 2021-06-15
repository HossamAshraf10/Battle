#pragma once
#include "Enemies/Enemy.h"
#include "Battle.h"
class Battle;
class Healer :
    public Enemy
{
private:
	void healFighters(Battle* battle);
	void healHealers(Battle* battle);
	void healFreezers(Battle* battle);
	void _heal(Healer* healer, Enemy* healed);
public:
	Healer(int id, ENMY_TYPE type, int arrTime, double health, int power,
		int reload, int speed) : Enemy(id, type, arrTime, health, power, reload, speed) {}
	void Move();
	void Heal(Battle* battle);
};


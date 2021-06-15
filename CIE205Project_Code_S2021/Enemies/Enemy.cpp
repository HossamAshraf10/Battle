#include "Enemy.h"


Enemy::Enemy(int id, ENMY_TYPE type, int arrTime, double health, int power,
	int reload, int speed, int distance)
{
	SetID(id);
	SetType(type);
	SetArrvTime(arrTime);
	this->maxHealth = health;
	SetHealth(health);
	SetPower(power);
	SetReload(reload);
	SetSpeed(speed);
	SetDistance(distance);
	SetStatus(INAC);
	//
	FirstShotTime = -1;
	KillTime = -1;
}

Enemy::~Enemy()
{
}



bool Enemy::operator>(Enemy* other)
{
	return this->priority > other->GetPriority();
}

bool Enemy::operator>=(Enemy* other)
{
	return this->priority >= other->GetPriority();
}

void Enemy::DecrementDist()
{
	if (Distance > MinDistance)
		Distance--;
}


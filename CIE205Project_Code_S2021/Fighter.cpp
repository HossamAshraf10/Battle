#include "Fighter.h"

void Fighter::Move()
{
	//one directional movement
	if (Distance > MinDistance)
	{
		if (health <= 50) //not healthy
		{
			if (Distance - speed/2 < MinDistance) SetDistance(MinDistance); 
			else SetDistance(Distance - speed/2);
		}
		else //healthy 
		{
			if (Distance - speed < MinDistance) SetDistance(MinDistance);
			else SetDistance(Distance - speed);
		}
	}
}



void Fighter::fight(Castle* castle)
{
	double k = 0;
	double damage = 0;

	if (!isInReloadPeriod()) 
	{
		if (health < 50) k = 0.5;
		else k = 1;

		damage = k * ((double)power) / Distance;
		castle->SetHealth(castle->GetHealth() - damage);

		waitTillNextShot = 0; //to wait again till period finishes
	}

	
}


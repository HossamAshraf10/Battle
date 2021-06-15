#include "Freezer.h"

void Freezer::Move()
{
	//one directional movement
	if (Distance > MinDistance)
	{
		if (health <= 50) //not healthy
		{
			if (Distance - speed / 2 < MinDistance) SetDistance(MinDistance);
			else SetDistance(Distance - speed / 2);
		}
		else //healthy 
		{
			if (Distance - speed < MinDistance) SetDistance(MinDistance);
			else SetDistance(Distance - speed);
		}
	}
}


void Freezer::Freeze(Castle* castle)
{
	double k = 0;
	double ice = 0;

	if (!isInReloadPeriod())
	{
		if (health < 50) k = 0.5;
		else k = 1;

		ice = k * ((double)power) / Distance; 
		castle->accumulateIce(ice);

		waitTillNextShot = 0; //to wait again till period finishes
	}
}

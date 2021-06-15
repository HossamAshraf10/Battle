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

}

#include "Castle.h"



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
	if(h > 0)
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
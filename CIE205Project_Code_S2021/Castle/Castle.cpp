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
}

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
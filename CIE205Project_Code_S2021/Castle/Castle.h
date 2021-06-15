#pragma once
#include "..\Defs.h"
#include"..\Generic_DS\ArrayStack.h"

class Castle
{
	double Health;
	int N; //The number of eniemes attacked per attack
	double power; // the power of the fire
	double originalHealth;

	double IceThreshold;
	double accumulatedIce; //I choose it be 1/4 original health value


	//

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
	
	bool isCastleFreezed();
	double getAccumulatedIce();
	void accumulateIce(double ice);
	//
	// TODO: Add More Member Functions As Needed
	//

};
 

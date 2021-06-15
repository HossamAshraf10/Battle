#pragma once
#include "Battle.h"
class Battle;

#include "Castle\Castle.h"
#include "Generic_DS\Queue.h"
#include "Generic_DS\PriorityQueue.h"
#include "Generic_DS/ArrayStack.h"
#include "Fighter.h"
#include "Healer.h"
#include "Freezer.h"
class Battle_mode
{

private:
	void moveFighters();
	void moveHealers();
	void moveFreezers();

	void actFighters();
	void actHealers();
	void actFreezers();
protected:
	GUI* pGUI;
	Battle* battle;
	bool isDone();
	void move_all_enimies();
	void Act_all_enemies();
public:
	Battle_mode(Battle* battle);
	virtual void Run()=0;

};

#pragma once
#ifndef  BATTLE_H
#define BATTLE_H

#include "Enemies\Enemy.h"
#include "Castle\Castle.h"
#include "Generic_DS\Queue.h"
#include "Generic_DS\PriorityQueue.h"
#include "Generic_DS/ArrayStack.h"
#include "Fighter.h"
#include "Healer.h"
#include "Freezer.h"
#include "GUI\GUI.h"
#include "Battle_mode.h"
#include "Castle/SuperSoliders.h"

// it is the controller of the project
class Battle_mode;
class Healer;
class Battle
{
private:
	GUI* pGUI;
	Castle BCastle;
	int EnemyCount;	//the actual number of enemies in the game
	int ActiveCount, FrostedCount, KilledCount;	//no. of enemies (Active, Frosted, killed so far)
	int CurrentTimeStep;
	//Enemy * BEnemiesForDraw[MaxEnemyCount]; // This Array of Pointers is used for drawing elements in the GUI
								  			  // No matter what list type you are using to hold enemies, 
											  // you must pass the enemies to the GUI function as an array of enemy pointers. 
											  // At every time step, you should update those pointers 
											  // to point to the current active enemies 
											  // then pass the pointers list to the GUI function

	Queue<Enemy*>* inactive;		//Queue of inactive enemies
	PriorityQueue<Fighter*>* actv_fighters;
	ArrayStack<Healer*>* actv_healers;
	Queue<Freezer*>* actv_freezers;
	PriorityQueue<Enemy*>* frzn_enms;
	Queue<Enemy*>* kld_enms;
	ArrayStack<superSoliders*>* actv_SuperSoliders;
	Battle_mode* crnt_mode;
	bool isSuperNeededd;
	/// ==> 
	//	DEMO-related members. Should be removed in phases 1&2
	//Queue for all enemies in the battle
	int DemoListCount;
	Enemy* DemoList[MaxEnemyCount];	//Important: This is just for demo
	/// ==>

	//
	// TODO: Add More Data Members As Needed
	//

public:
	
	Battle();
	void AddAllListsToDrawingList();		//Add enemy lists (inactive, active,.....) to drawing list to be displayed on user interface
	Castle * GetCastle();
	void setCastle(Castle*);
	void setInactiveQueue(Queue<Enemy*>*);
	void setNumberOfEnemies(int);
	void RunSimulation();
	void ActivateEnemies();		//check the inactive list and activate all enemies that has arrived
	//BONUS
	void ActivateSuperSoliders(superSoliders* superSolider,int ID, int power, int arrTime, int reloadTime, int speed, double maxHealth);
	int getMaxEnemDistance();
	ArrayStack<superSoliders*>* getSupersoliders();

	void AddtoDemoList(Enemy* Ptr);		//Add Enemy to the demo queue of enemies (for demo purposes only)
	

	//Getter and setter for main components; will be needed in battle_state
	Queue<Enemy*>* getInactive();
	PriorityQueue<Fighter*>* getActvFighters();
	ArrayStack<Healer*>* getActvHealers();
	Queue<Freezer*>* getActvFreezers();
	PriorityQueue<Enemy*>* getFrzonEnimies();
	Queue<Enemy*>* getKilledEnimies();


	void incrementTimeStep();
	GUI* getGUI();
	int getCurrentTimeStep();
	int getEnemycount();


	// function to count

	int getNumActivFighters();
	int getNumActivHealers();
	int getNumActivFreezeers();
	int getNumActiveTotal();

	int getNumActivFightersAtDist(int distance);
	int getNumActivHealersAtDist(int distance);
	int getNumActivFreezeersAtDist(int distance);
	int getNumActivTotalAtDist(int distance);

	int getNumFrostedFighters();
	int getNumFrostedHealers();
	int getNumFrostedFreezeers();
	int getNumFrostedTotal();

	int getNumKilled();
	int getNumAlive();

	void LetCastleAttach(int crntTime);
	bool isSuperNeeded();
};

#endif // ! BATTLE
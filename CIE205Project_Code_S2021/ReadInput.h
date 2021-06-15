#pragma once
#include <fstream>
#include <iostream>
#include "Battle.h"
#include "Generic_DS/Queue.h"
#include "Castle/Castle.h"
#include "Enemies/Enemy.h"
#include "Fighter.h"
#include "Healer.h"
#include "Freezer.h"
using namespace std;
class Battle;
class ReadInput
{
private:
	Castle* createCastle(ifstream& file)
	{
		char* pch = NULL;
		char* context = NULL;
		const int size = 50;
		char line[size];
		int castleHealth, n, castlePower;
		file.getline(line, size);

		pch = strtok_s(line, " ", &context);
		castleHealth = stoi(pch);
		pch = strtok_s(NULL, " ", &context);
		n = stoi(pch);
		pch = strtok_s(NULL, " ", &context);
		castlePower = stoi(pch);
		pch = strtok_s(NULL, " ", &context);

		Castle* castle = new Castle(castleHealth, n, castlePower);
		return castle;
	}

	int getNumberOfEnemies(ifstream& file)
	{
		char* pch = NULL;
		char* context = NULL;
		const int size = 50;
		char line[size];

		file.getline(line, size);//get enemies number
		pch = strtok_s(line, " ", &context);
		int count = stoi(pch);
		return count;
	}

	Queue<Enemy*>* createEnemyQueue(ifstream& file)
	{
		Queue < Enemy*>* enemies = new Queue < Enemy*>();
		Enemy* enemy;
		int  enemyId, enemyType, enemyArrival, enemyHealth,
			enemyPower, enemyReload, enemySpeed;

		char* pch = NULL;
		char* context = NULL;
		const int size = 50;
		char line[size];


		while (file.getline(line, size)) //get enemies data
		{
			pch = strtok_s(line, " ", &context);
			enemyId = stoi(pch), pch = strtok_s(NULL, " ", &context);
			enemyType = stoi(pch), pch = strtok_s(NULL, " ", &context);
			enemyArrival = stoi(pch), pch = strtok_s(NULL, " ", &context);
			enemyHealth = stoi(pch), pch = strtok_s(NULL, " ", &context);
			enemyPower = stoi(pch), pch = strtok_s(NULL, " ", &context);
			enemyReload = stoi(pch), pch = strtok_s(NULL, " ", &context);
			enemySpeed = stoi(pch), pch = strtok_s(NULL, " ", &context);

			//use data to fill enemies
			switch (enemyType)
			{
			case 0: enemy = new Fighter(enemyId, FIGHTER, enemyArrival, enemyHealth,
				enemyPower, enemyReload, enemySpeed);
				break;
			case 1: enemy = new Healer(enemyId, HEALER, enemyArrival, enemyHealth,
				enemyPower, enemyReload, enemySpeed);
				break;
			case 2: enemy = new Freezer(enemyId, FREEZER, enemyArrival, enemyHealth,
				enemyPower, enemyReload, enemySpeed);
				break;
			}
			enemies->enqueue(enemy);
		}
		return enemies;
	}

	void read(Battle* pBattle)
	{
		ifstream file("Files\\The Input.txt");
		if (!file) cout << "Error! failed to open input file";


		pBattle->setCastle(createCastle(file));
		pBattle->setNumberOfEnemies(getNumberOfEnemies(file));
		pBattle->setInactiveQueue(createEnemyQueue(file));


		file.close();
	}

public:
	//once you create an object from this class it will read the input
	ReadInput(Battle* battle)
	{
		read(battle);
	}
};
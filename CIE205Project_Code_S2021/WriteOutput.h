#pragma once
#include <fstream>
#include <iomanip>
#include "Battle.h"

using namespace std;

class WriteOutput
{
private:
	int firstShotDelay(Enemy* enemy)
	{
		return (enemy->GetFirstShotTime() - enemy->GetArrvTime());
	}
	int killDelay(Enemy* enemy)
	{
		return (enemy->GetKillTime() - enemy->GetFirstShotTime());
	}
	int lifeTime(Enemy* enemy)
	{
		return (enemy->GetKillTime() - enemy->GetArrvTime());
	}

	void writeOutput(Battle* battle)
	{
		Queue<Enemy*>* Q_killed = battle->getKilledEnimies();
		Castle* castle = battle->GetCastle();
		ofstream file;
		file.open("Files\\Output.txt");


		//first line
		if (castle->GetHealth() > 0.1) file << "Game Is WIN!";
		else file << "Game Is Loss";
		file << endl;

		//2nd line
		file << "KTS\t" << "ID\t" << "FD\t" << "KD\t" << "LT\t" << endl;

		//Enemies (3rd to n-4)
		Enemy* enemy;
		double totalFD, totalKD;
		int currentFD, currentKD;
		totalFD = totalKD = currentFD = currentKD = 0;
		while (Q_killed->dequeue(enemy))
		{
			currentFD = firstShotDelay(enemy);
			currentKD = killDelay(enemy);
			totalFD += currentFD;
			totalKD += currentKD;

			file << enemy->GetKillTime() << "\t";
			file << enemy->GetID() << "\t";
			file << currentFD << "\t";
			file << currentKD << "\t";
			file << lifeTime(enemy) << "\t" << endl;
		}


		//last 4 lines
		int n = battle->getEnemycount();

		file << "Castle Total Damage         = " << (castle->GetOriginalHealth() - castle->GetHealth()) << endl;

		//hossam revise n and fd and kd

//		if (castle->GetHealth() > 0.1)
	//	{
			file << "Total Enemies                   = " << n << endl;
		//}
		/*else
		{
			file << "Killed Enemies                  = " << battle->getNumKilled() << endl;
			file << "Alive Enemies                   = " << battle->getNumAlive() << endl;
		}*/

		file << "Average First - Shot Delay = " << (totalFD / n) << endl;
		file << "Average Kill Delay	         = " << (totalKD / n) << endl;
	

	}
public:
	//once you create an object from this class it will write the output file
	WriteOutput(Battle* battle)
	{
		writeOutput(battle);
	}


};
//
//1 - In case of game “win”
//a.Total number of enemies
//b.Average “First - Shot Delay” and Average “Kill Delay”
//2 - In case of game “loss / drawn”
//a.Number of killed enemies
//b.Number of alive enemies(active and inactive)
//c.Average “First - Shot Delay” and Average “Kill Delay” for killed enemies only
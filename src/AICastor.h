/*
  Copyright (C) 2001-2004 Stephane Magnenat & Luc-Olivier de Charrière
  for any question or comment contact us at nct@ysagoon.com or nuage@ysagoon.com

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*/

#ifndef __AI_CASTOR_H
#define __AI_CASTOR_H

#include <list>

#include "BuildingType.h"
#include "AIImplementation.h"

class Game;
class Map;
class Order;
class Player;
class Team;
class Building;

class AICastor : public AIImplementation
{
public:
	class Project
	{
	public:
		Project(BuildingType::BuildingTypeShortNumber shortTypeNum, const char* debugName);
		Project(BuildingType::BuildingTypeShortNumber shortTypeNum, Sint32 amount, Sint32 mainWorkers, const char* debugName);
		void init();

	public:
		BuildingType::BuildingTypeShortNumber shortTypeNum;
		Sint32 amount; // number of buildings wanted
		bool food; // place closer to wheat of further
		
		const char *debugName;
		
		int subPhase;
		
		Uint32 successWait; // wait a number of success in the hope to find a better one just a bit later.
		bool blocking; // if true, no other project can be added..
		bool critical; // if true, place building at any cost.
		Sint32 priority; // the lower is the number, the higher is the priority
		
		Sint32 mainWorkers;
		Sint32 foodWorkers;
		Sint32 otherWorkers;
		
		bool multipleStart;
		bool waitFinished;
		Sint32 finalWorkers;
		
		bool finished;
		
		Uint32 timer;
	};
	
	class Strategy
	{
	public:
		Strategy();
		Strategy& Strategy::operator+=(const Strategy &strategy);
	
	public:
		bool defined;
		
		Sint32 successWait;
		
		Sint32 swarm;
		Sint32 speed;
		Sint32 attack;
		Sint32 heal;
		Sint32 defense;
		Sint32 science;
		Sint32 swim;
		
		Sint32 warLevelTriger;
		Sint32 warTimeTriger;
		Sint32 maxAmountGoal;
		
		Sint32 foodWorkers;
		Sint32 swarmWorkers;
		Uint8 wheatCareLimit;
	};
	
private:
	void firstInit();
public:
	AICastor(Player *player);
	AICastor(SDL_RWops *stream, Player *player, Sint32 versionMinor);
	void init(Player *player);
	~AICastor();

	Player *player;
	Team *team;
	Game *game;
	Map *map;

	bool load(SDL_RWops *stream, Player *player, Sint32 versionMinor);
	void save(SDL_RWops *stream);
	
	Order *getOrder(void);
	
private:
	Order *controlSwarms();
	Order *expandFood();
	Order *controlFood(int index);
	
	bool addProject(Project *project);
	void addProjects();
	
	void choosePhase();
	
	Order *continueProject(Project *project);
	
	int getFreeWorkers();
	
	void computeCanSwim();
	void computeNeedSwim();
	void computeBuildingSum();
	
	void computeObstacleUnitMap();
	void computeObstacleBuildingMap();
	void computeSpaceForBuildingMap(int max);
	void computeBuildingNeighbourMap(int dw, int dh);
	void computeTwoSpaceNeighbourMap();
	
	void computeWorkPowerMap();
	void computeWorkRangeMap();
	void computeWorkAbilityMap();
	void computeHydratationMap();
	void computeWheatGrowthMap(int dw, int dh);
	
	Order *findGoodBuilding(Sint32 typeNum, bool food, bool critical);
	
	void computeRessourcesCluster();
	
public:
	void updateGlobalGradientNoObstacle(Uint8 *gradient);

	std::list<Project *> projects;
	
	Uint32 timer;
	bool canSwim;
	bool needSwim;
	int buildingSum[BuildingType::NB_BUILDING][2]; // [shortTypeNum][isBuildingSite]
	bool war;
	Uint32 lastNeedPoolComputed;
	Uint32 computeNeedSwimTimer;
	Uint32 controlSwarmsTimer;
	Uint32 expandFoodTimer;
	Uint32 controlFoodTimer;
	int controlFoodToogle;
	
	Strategy strategy;
	
	bool hydratationMapComputed;
	
public:
	Uint8 *obstacleUnitMap; // where units can go. included in {0, 1}
	Uint8 *obstacleBuildingMap; // where buildings can be built. included in {0, 1}
	Uint8 *spaceForBuildingMap; // where building can be built, of size X*X. included in {0, 1, 2}. More iterations can provide arbitrary size.
	Uint8 *buildingNeighbourMap; // bit 0: bad flag, bits [1, 3]: direct neighbours count, bit 4: zero, bits [5, 7]; far neighbours count.
	
	Uint8 *twoSpaceNeighbourMap; // TODO: remove.
	
	Uint8 *workPowerMap;
	Uint8 *workRangeMap;
	Uint8 *workAbilityMap;
	Uint8 *hydratationMap;
	Uint8 *wheatGrowthMap;
	Uint8 *wheatCareMap;
	
	Uint8 *goodBuildingMap; // TODO: remove.
	
	Uint16 *ressourcesCluster;
	
private:
	FILE *logFile;
};

#endif

 


#pragma once

#include "Common.h"
#include "BWTA.h"
#include "base/BuildOrderQueue.h"
#include "InformationManager.h"
#include "base/WorkerManager.h"
#include "base/StarcraftBuildOrderSearchManager.h"
#include <sys/stat.h>
#include <cstdlib>

#include "..\..\StarcraftBuildOrderSearch\Source\starcraftsearch\StarcraftData.hpp"

typedef std::pair<int, int> IntPair;
typedef std::pair<MetaType, UnitCountType> MetaPair;
typedef std::vector<MetaPair> MetaPairVector;

class StrategyManager 
{
	StrategyManager();
	~StrategyManager() {}

	std::vector<std::string>	protossOpeningBook;
	std::vector<std::string>	terranOpeningBook;
	std::vector<std::string>	zergOpeningBook;

	std::string					readDir;
	std::string					writeDir;
	std::vector<IntPair>		results;
	std::vector<int>			usableStrategies;
	int							currentStrategy;

	BWAPI::Race					selfRace;
	BWAPI::Race					enemyRace;

	bool						firstAttackSent;

	void	addStrategies();
	void	setStrategy();
	void	readResults();
	void	writeResults();

	const	int					getScore(BWAPI::Player * player) const;
	const	double				getUCBValue(const size_t & strategy) const;
	
	// protoss strategy
	const	bool				expandProtossZealotRush() const;
	const	std::string			getProtossZealotRushOpeningBook() const;
	const	MetaPairVector		getProtossZealotRushBuildOrderGoal() const;

	const	bool				expandProtossDarkTemplar() const;
	const	std::string			getProtossDarkTemplarOpeningBook() const;
	const	MetaPairVector		getProtossDarkTemplarBuildOrderGoal() const;

	const	bool				expandProtossDragoons() const;
	const	std::string			getProtossDragoonsOpeningBook() const;
	const	MetaPairVector		getProtossDragoonsBuildOrderGoal() const;

	const	MetaPairVector		getTerranBuildOrderGoal() const;
	const	MetaPairVector		getZergBuildOrderGoal() const;

	const	MetaPairVector		getProtossOpeningBook() const;
	const	MetaPairVector		getTerranOpeningBook() const;
	const	MetaPairVector		getZergOpeningBook() const;

public:

	enum { ProtossZealotRush=0, ProtossDarkTemplar=1, ProtossDragoons=2, NumProtossStrategies=3 };
	enum
	{
		TerranMarineRush = 0, TerranDoubleRaxMnM = 1,
		TerranTriRaxMnMRush = 2, TerranProxyRaxMarineRush = 3,
		Terran3FactoryVultureRush = 4, TerranGundamRush = 5,
		Terran1FastExpoDef = 6, Terran1FastExpoNoDef = 7,
		Terran2FactMechBuild = 8,
		TerranGoliathBuild = 9, TerranGoliathDrop = 10,
		Terran1FastPortBuild = 11, TerranWraithRush1Port = 12, 
		TerranWraithRush2PortsTvZ = 13, TerranWraithRush2PortsTvT = 14,	
		NumTerranStrategies=15 };
	//enum { ZergZerglingRush = 0, NumZergStrategies = 1 };	// ext
	
	enum { ZergCerver4PoolRush = 0, Zerg9PoolHatch = 1, Zerg7PoolRush = 2,
		Zerg9PoolSpeedlingsRush = 3, NumZergStrategies = 4 };
	enum { ZergMidOne, NumZergMid = 1 };

	static	StrategyManager &	Instance();

			void				onEnd(const bool isWinner);
	
	const	bool				regroup(int numInRadius);
	const	bool				doAttack(const std::set<BWAPI::Unit *> & freeUnits);
	const	int				    defendWithWorkers();
	const	bool				rushDetected();

	const	int					getCurrentStrategy();

	const	MetaPairVector		getBuildOrderGoal();
	const	std::string			getOpeningBook() const;


	// Extensions
public:
	bool isMidGame;			// sets to true when opening build order has finished

private:
	//int _usableStrategiesNo;			// Number of strategies to be chosen from	

	void CreateZergUsableStrategies();
	void CreateTerranUsableStrategies();

	int GetStrategyIdx();
	int GenerateRandomStrategy(const int min, const int max);

	std::vector<std::string>	zergMidgameBook;

	// Attack checkers
	bool doAttackZergCerver4PoolRush();
	bool doAttackZerg9PoolHatch();
	bool doAttackZerg7PoolRush();
	bool doAttackZerg9PoolSpeedlingsRush();
	bool doAttackZergMidgame();


};

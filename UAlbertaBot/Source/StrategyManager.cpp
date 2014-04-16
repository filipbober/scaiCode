#include "Common.h"
#include "StrategyManager.h"

// constructor
StrategyManager::StrategyManager()
: firstAttackSent(false)
, currentStrategy(0)
, selfRace(BWAPI::Broodwar->self()->getRace())
, enemyRace(BWAPI::Broodwar->enemy()->getRace())
, isMidGame(false)
{
	addStrategies();
	setStrategy();
}

// get an instance of this
StrategyManager & StrategyManager::Instance()
{
	static StrategyManager instance;
	return instance;
}

void StrategyManager::addStrategies()
{
	protossOpeningBook = std::vector<std::string>(NumProtossStrategies);
	terranOpeningBook = std::vector<std::string>(NumTerranStrategies);
	zergOpeningBook = std::vector<std::string>(NumZergStrategies);

	zergMidgameBook = std::vector<std::string>(NumZergMid);

	//protossOpeningBook[ProtossZealotRush]	= "0 0 0 0 1 0 0 3 0 0 3 0 1 3 0 4 4 4 4 4 1 0 4 4 4";
	protossOpeningBook[ProtossZealotRush] = "0 0 0 0 1 0 3 3 0 0 4 1 4 4 0 4 4 0 1 4 3 0 1 0 4 0 4 4 4 4 1 0 4 4 4";
	//protossOpeningBook[ProtossZealotRush]	= "0";
	//protossOpeningBook[ProtossDarkTemplar]	= "0 0 0 0 1 3 0 7 5 0 0 12 3 13 0 22 22 22 22 0 1 0";
	protossOpeningBook[ProtossDarkTemplar] = "0 0 0 0 1 0 3 0 7 0 5 0 12 0 13 3 22 22 1 22 22 0 1 0";
	protossOpeningBook[ProtossDragoons] = "0 0 0 0 1 0 0 3 0 7 0 0 5 0 0 3 8 6 1 6 6 0 3 1 0 6 6 6";
	
	//terranOpeningBook[TerranMarineRush] = "0 0 0 0 0 1 0 0 3 0 0 3 0 1 0 4 0 0 0 6";			// deprecated

	terranOpeningBook[TerranMarineRush] = "0 0 0 0 0 17 0 0 19 0 0 19 17 0 18 0 0 20";
	terranOpeningBook[TerranDoubleRaxMnM] = "0 0 0 0 0 17 0 19 0 0 17 18 17";
	terranOpeningBook[TerranTriRaxMnMRush] = "0 0 0 0 0 17 0 0 19 0 0 19 0 0 17 19 18 21 20";
	terranOpeningBook[TerranProxyRaxMarineRush] = "0 0 17 0 0 0 0 19 19 0 0 1 1";		// hard to implement
	terranOpeningBook[Terran3FactoryVultureRush] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 1 0 0 22 1 0 22 1 0 23 17 0 3 47 3 23 0 17 0 22 38";		// <- Preferred build order
	terranOpeningBook[TerranGundamRush] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 1 22 0 1 0 1 22 1 0 23 17 1 0 7 3 38 0 1 17 43";				// hard to implement, check if tanks are build
	terranOpeningBook[Terran1FastExpoDef] =		"0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 0 1 0 1 23 1 0 17 7 43 14";							// check if command center is build in a proper rotation
	terranOpeningBook[Terran1FastExpoNoDef] =	"0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 0 23 0 0 38 0 7 17 0 43 7 21";
	terranOpeningBook[Terran2FactMechBuild] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 22";
	terranOpeningBook[TerranGoliathBuild] = "0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 0 22 0 0 22";
	terranOpeningBook[TerranGoliathDrop] = "0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 0 22 0 0 22 0 25 24 0 23 17 0 0 26 48 6 6 17 9 6 6";			// hard to implement - needs Dropship micro
	terranOpeningBook[Terran1FastPortBuild] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 1 0 1 0 25 23 1 17 0 7 0 43 26 0 7 21 9";				// hard to implement - needs Dropship micro
	terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 1 0 1 0 25 1 0 17 1 20 10 19 0 26 27";
	terranOpeningBook[TerranWraithRush2PortsTvZ] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 1 0 1 0 25 0 25 17 0 1 26 10 20 10 10 39 19 21 0 27";
	terranOpeningBook[TerranWraithRush2PortsTvT] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 25 0 25 0 3 0 3 17 0 10 26 0 21";
	
	// For testing purposes
	//terranOpeningBook[TerranMarineRush] = "0";
	//terranOpeningBook[TerranDoubleRaxMnM] = "0";
	//terranOpeningBook[TerranTriRaxMnMRush] = "0";
	//terranOpeningBook[TerranProxyRaxMarineRush] = "0";
	//terranOpeningBook[Terran3FactoryVultureRush] = "0";
	//terranOpeningBook[TerranGundamRush] = "0";
	//terranOpeningBook[Terran1FastExpoDef] = "0";
	//terranOpeningBook[Terran1FastExpoNoDef] = "0";
	//terranOpeningBook[Terran2FactMechBuild] = "0";
	//terranOpeningBook[TerranGoliathBuild] = "0";
	//terranOpeningBook[TerranGoliathDrop] = "0";
	//terranOpeningBook[Terran1FastPortBuild] = "0";
	//terranOpeningBook[TerranWraithRush1Port] = "0";
	//terranOpeningBook[TerranWraithRush2PortsTvZ] = "0";
	//terranOpeningBook[TerranWraithRush2PortsTvT] = "0";


	//zergOpeningBook[ZergZerglingRush]		= "0 0 0 0 0 1 0 0 0 2 3 5 0 0 0 0 0 0 1 6";	// ext

	// Extensions
	zergOpeningBook[ZergCerver4PoolRush] = "3 0 4 4 4 0 0 1 2 4 4 4 5 0 0 0 6";
	zergOpeningBook[Zerg9PoolHatch] = "0 0 0 0 0 1 0 0 0 3 2 4 4 4";
	zergOpeningBook[Zerg7PoolRush] = "0 0 0 3 1 0 4 4 4";
	zergOpeningBook[Zerg9PoolSpeedlingsRush] = "0 0 0 0 0 3 0 5 1 0 21 4 4 4";

	//zergOpeningBook[ZergCerver4PoolRush] = "0";
	//zergOpeningBook[Zerg9PoolHatch] = "0";
	//zergOpeningBook[Zerg7PoolRush] = "0";
	//zergOpeningBook[Zerg9PoolSpeedlingsRush] = "0";


	// eof Extensions

	if (selfRace == BWAPI::Races::Protoss)
	{
		results = std::vector<IntPair>(NumProtossStrategies);

		if (enemyRace == BWAPI::Races::Protoss)
		{
			usableStrategies.push_back(ProtossZealotRush);
			usableStrategies.push_back(ProtossDarkTemplar);
			usableStrategies.push_back(ProtossDragoons);
		}
		else if (enemyRace == BWAPI::Races::Terran)
		{
			usableStrategies.push_back(ProtossZealotRush);
			usableStrategies.push_back(ProtossDarkTemplar);
			usableStrategies.push_back(ProtossDragoons);
		}
		else if (enemyRace == BWAPI::Races::Zerg)
		{
			usableStrategies.push_back(ProtossZealotRush);
			usableStrategies.push_back(ProtossDragoons);
		}
		else
		{
			BWAPI::Broodwar->printf("Enemy Race Unknown");
			usableStrategies.push_back(ProtossZealotRush);
			usableStrategies.push_back(ProtossDragoons);
		}
	}
	else if (selfRace == BWAPI::Races::Terran)
	{
		//results = std::vector<IntPair>(NumTerranStrategies);		// ext
		//usableStrategies.push_back(TerranMarineRush);				// ext

		results = std::vector<IntPair>(NumTerranStrategies);
		CreateTerranUsableStrategies();
	}
	else if (selfRace == BWAPI::Races::Zerg)
	{
		results = std::vector<IntPair>(NumZergStrategies);
		//usableStrategies.push_back(ZergZerglingRush);		// ext

		// Extensions
		CreateZergUsableStrategies();
		// eof Extensions

	}

	if (Options::Modules::USING_STRATEGY_IO)
	{
		readResults();
	}
}

void StrategyManager::readResults()
{
	// read in the name of the read and write directories from settings file
	struct stat buf;

	// if the file doesn't exist something is wrong so just set them to default settings
	if (stat(Options::FileIO::FILE_SETTINGS, &buf) == -1)
	{
		readDir = "bwapi-data/testio/read/";
		writeDir = "bwapi-data/testio/write/";
	}
	else
	{
		std::ifstream f_in(Options::FileIO::FILE_SETTINGS);
		getline(f_in, readDir);
		getline(f_in, writeDir);
		f_in.close();
	}

	// the file corresponding to the enemy's previous results
	std::string readFile = readDir + BWAPI::Broodwar->enemy()->getName() + ".txt";

	// if the file doesn't exist, set the results to zeros
	if (stat(readFile.c_str(), &buf) == -1)
	{
		std::fill(results.begin(), results.end(), IntPair(0, 0));
	}
	// otherwise read in the results
	else
	{
		std::ifstream f_in(readFile.c_str());
		std::string line;
		getline(f_in, line);
		results[ProtossZealotRush].first = atoi(line.c_str());
		getline(f_in, line);
		results[ProtossZealotRush].second = atoi(line.c_str());
		getline(f_in, line);
		results[ProtossDarkTemplar].first = atoi(line.c_str());
		getline(f_in, line);
		results[ProtossDarkTemplar].second = atoi(line.c_str());
		getline(f_in, line);
		results[ProtossDragoons].first = atoi(line.c_str());
		getline(f_in, line);
		results[ProtossDragoons].second = atoi(line.c_str());
		f_in.close();
	}

	BWAPI::Broodwar->printf("Results (%s): (%d %d) (%d %d) (%d %d)", BWAPI::Broodwar->enemy()->getName().c_str(),
		results[0].first, results[0].second, results[1].first, results[1].second, results[2].first, results[2].second);
}

void StrategyManager::writeResults()
{
	std::string writeFile = writeDir + BWAPI::Broodwar->enemy()->getName() + ".txt";
	std::ofstream f_out(writeFile.c_str());

	f_out << results[ProtossZealotRush].first << "\n";
	f_out << results[ProtossZealotRush].second << "\n";
	f_out << results[ProtossDarkTemplar].first << "\n";
	f_out << results[ProtossDarkTemplar].second << "\n";
	f_out << results[ProtossDragoons].first << "\n";
	f_out << results[ProtossDragoons].second << "\n";

	f_out.close();
}

void StrategyManager::setStrategy()
{
	// if we are using file io to determine strategy, do so
	if (Options::Modules::USING_STRATEGY_IO)
	{
		double bestUCB = -1;
		int bestStrategyIndex = 0;

		// UCB requires us to try everything once before using the formula
		for (size_t strategyIndex(0); strategyIndex < usableStrategies.size(); ++strategyIndex)
		{
			int sum = results[usableStrategies[strategyIndex]].first + results[usableStrategies[strategyIndex]].second;

			if (sum == 0)
			{
				currentStrategy = usableStrategies[strategyIndex];
				return;
			}
		}

		// if we have tried everything once, set the maximizing ucb value
		for (size_t strategyIndex(0); strategyIndex<usableStrategies.size(); ++strategyIndex)
		{
			double ucb = getUCBValue(usableStrategies[strategyIndex]);

			if (ucb > bestUCB)
			{
				bestUCB = ucb;
				bestStrategyIndex = strategyIndex;
			}
		}

		currentStrategy = usableStrategies[bestStrategyIndex];
	}
	else
	{
		// otherwise return a random strategy
		currentStrategy = GetStrategyIdx();
		// TODO: algorithm to choose the right strategy (or just plenty of if statements)



		if (selfRace == BWAPI::Races::Protoss)		// ext
		{
			std::string enemyName(BWAPI::Broodwar->enemy()->getName());

			if (enemyName.compare("Skynet") == 0)
			{
				currentStrategy = ProtossDarkTemplar;
			}
			else
			{
				currentStrategy = ProtossZealotRush;
			}
		}
	}

}

void StrategyManager::onEnd(const bool isWinner)
{
	// write the win/loss data to file if we're using IO
	if (Options::Modules::USING_STRATEGY_IO)
	{
		// if the game ended before the tournament time limit
		if (BWAPI::Broodwar->getFrameCount() < Options::Tournament::GAME_END_FRAME)
		{
			if (isWinner)
			{
				results[getCurrentStrategy()].first = results[getCurrentStrategy()].first + 1;
			}
			else
			{
				results[getCurrentStrategy()].second = results[getCurrentStrategy()].second + 1;
			}
		}
		// otherwise game timed out so use in-game score
		else
		{
			if (getScore(BWAPI::Broodwar->self()) > getScore(BWAPI::Broodwar->enemy()))
			{
				results[getCurrentStrategy()].first = results[getCurrentStrategy()].first + 1;
			}
			else
			{
				results[getCurrentStrategy()].second = results[getCurrentStrategy()].second + 1;
			}
		}

		writeResults();
	}
}

const double StrategyManager::getUCBValue(const size_t & strategy) const
{
	double totalTrials(0);
	for (size_t s(0); s < usableStrategies.size(); ++s)
	{
		totalTrials += results[usableStrategies[s]].first + results[usableStrategies[s]].second;
	}

	double C = 0.7;
	double wins = results[strategy].first;
	double trials = results[strategy].first + results[strategy].second;

	double ucb = (wins / trials) + C * sqrt(std::log(totalTrials) / trials);

	return ucb;
}

const int StrategyManager::getScore(BWAPI::Player * player) const
{
	return player->getBuildingScore() + player->getKillScore() + player->getRazingScore() + player->getUnitScore();
}

const std::string StrategyManager::getOpeningBook() const
{
	if (selfRace == BWAPI::Races::Protoss)
	{
		return protossOpeningBook[currentStrategy];
	}
	else if (selfRace == BWAPI::Races::Terran)
	{
		return terranOpeningBook[currentStrategy];
	}
	else if (selfRace == BWAPI::Races::Zerg)
	{
		return zergOpeningBook[currentStrategy];
	}

	// something wrong, return the protoss one
	return protossOpeningBook[currentStrategy];
}

// when do we want to defend with our workers?
// this function can only be called if we have no fighters to defend with
const int StrategyManager::defendWithWorkers()
{
	if (!Options::Micro::WORKER_DEFENSE)
	{
		return false;
	}

	// our home nexus position
	BWAPI::Position homePosition = BWTA::getStartLocation(BWAPI::Broodwar->self())->getPosition();;

	// enemy units near our workers
	int enemyUnitsNearWorkers = 0;

	// defense radius of nexus
	int defenseRadius = 300;

	// fill the set with the types of units we're concerned about
	BOOST_FOREACH(BWAPI::Unit * unit, BWAPI::Broodwar->enemy()->getUnits())
	{
		// if it's a zergling or a worker we want to defend
		if (unit->getType() == BWAPI::UnitTypes::Zerg_Zergling)
		{
			if (unit->getDistance(homePosition) < defenseRadius)
			{
				enemyUnitsNearWorkers++;
			}
		}
	}

	// if there are enemy units near our workers, we want to defend
	return enemyUnitsNearWorkers;
}

// called by combat commander to determine whether or not to send an attack force
// freeUnits are the units available to do this attack
const bool StrategyManager::doAttack(const std::set<BWAPI::Unit *> & freeUnits)
{
	int ourForceSize = (int)freeUnits.size();

	int numUnitsNeededForAttack = 1;

	bool doAttack = (selfRace == BWAPI::Races::Protoss && (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Dark_Templar) >= 1
		|| ourForceSize >= numUnitsNeededForAttack));

	// Extension

	// If it is the first attack
	if (!firstAttackSent)
	{
		// Always perform the first attack with any trained attack unity (usually Zerglings rush)
		doAttack = true;
	}
	else if (isMidGame == false && selfRace == BWAPI::Races::Zerg)
	{
		// If it is not the first attack, adjust an attack to the current opening strategy
		if (currentStrategy == ZergCerver4PoolRush)
		{
			doAttack = doAttackZergCerver4PoolRush();
		}
		else if (currentStrategy == Zerg9PoolHatch)
		{
			doAttack = doAttackZerg9PoolHatch();
		}
		else if (currentStrategy == Zerg7PoolRush)
		{
			doAttack = doAttackZerg7PoolRush();
		}
		else if (currentStrategy == Zerg9PoolSpeedlingsRush)
		{
			doAttack = doAttackZerg9PoolSpeedlingsRush();
		}
		else if (isMidGame == true)
		{
			// if it is midgame, adjust attacks to the current strategy, opponent etc.
			doAttack = doAttackZergMidgame();
		}
		else
		{
			// Error - don't attack for the error to be seen (change later to default true)
			doAttack = false;
		}


	}

	// eof Extension


	if (doAttack)
	{
		firstAttackSent = true;
	}

	return doAttack || firstAttackSent;
}

const bool StrategyManager::expandProtossZealotRush() const
{
	// if there is no place to expand to, we can't expand
	if (MapTools::Instance().getNextExpansion() == BWAPI::TilePositions::None)
	{
		return false;
	}

	int numNexus = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Nexus);
	int numZealots = BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Zealot);
	int frame = BWAPI::Broodwar->getFrameCount();

	// if there are more than 10 idle workers, expand
	if (WorkerManager::Instance().getNumIdleWorkers() > 10)
	{
		return true;
	}

	// 2nd Nexus Conditions:
	//		We have 12 or more zealots
	//		It is past frame 7000
	if ((numNexus < 2) && (numZealots > 12 || frame > 9000))
	{
		return true;
	}

	// 3nd Nexus Conditions:
	//		We have 24 or more zealots
	//		It is past frame 12000
	if ((numNexus < 3) && (numZealots > 24 || frame > 15000))
	{
		return true;
	}

	if ((numNexus < 4) && (numZealots > 24 || frame > 21000))
	{
		return true;
	}

	if ((numNexus < 5) && (numZealots > 24 || frame > 26000))
	{
		return true;
	}

	if ((numNexus < 6) && (numZealots > 24 || frame > 30000))
	{
		return true;
	}

	return false;
}

const MetaPairVector StrategyManager::getBuildOrderGoal()
{
	if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Protoss)
	{
		if (getCurrentStrategy() == ProtossZealotRush)
		{
			return getProtossZealotRushBuildOrderGoal();
		}
		else if (getCurrentStrategy() == ProtossDarkTemplar)
		{
			return getProtossDarkTemplarBuildOrderGoal();
		}
		else if (getCurrentStrategy() == ProtossDragoons)
		{
			return getProtossDragoonsBuildOrderGoal();
		}

		// if something goes wrong, use zealot goal
		return getProtossZealotRushBuildOrderGoal();
	}
	else if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Terran)
	{
		return getTerranBuildOrderGoal();
	}
	else
	{
		return getZergBuildOrderGoal();
	}
}

const MetaPairVector StrategyManager::getProtossDragoonsBuildOrderGoal() const
{
	// the goal to return
	MetaPairVector goal;

	int numDragoons = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon);
	int numProbes = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Probe);
	int numNexusCompleted = BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Nexus);
	int numNexusAll = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Nexus);
	int numCyber = BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Cybernetics_Core);
	int numCannon = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Photon_Cannon);

	int dragoonsWanted = numDragoons > 0 ? numDragoons + 6 : 2;
	int gatewayWanted = 3;
	int probesWanted = numProbes + 6;

	if (InformationManager::Instance().enemyHasCloakedUnits())
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Robotics_Facility, 1));

		if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Robotics_Facility) > 0)
		{
			goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Observatory, 1));
		}
		if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Observatory) > 0)
		{
			goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Observer, 1));
		}
	}
	else
	{
		if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Robotics_Facility) > 0)
		{
			goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Observatory, 1));
		}

		if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Observatory) > 0)
		{
			goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Observer, 1));
		}
	}

	if (numNexusAll >= 2 || numDragoons > 6 || BWAPI::Broodwar->getFrameCount() > 9000)
	{
		gatewayWanted = 6;
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Robotics_Facility, 1));
	}

	if (numNexusCompleted >= 3)
	{
		gatewayWanted = 8;
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Observer, 2));
	}

	if (numNexusAll > 1)
	{
		probesWanted = numProbes + 6;
	}

	if (expandProtossZealotRush())
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Nexus, numNexusAll + 1));
	}

	goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Dragoon, dragoonsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Gateway, gatewayWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Probe, std::min(90, probesWanted)));

	return goal;
}

const MetaPairVector StrategyManager::getProtossDarkTemplarBuildOrderGoal() const
{
	// the goal to return
	MetaPairVector goal;

	int numDarkTeplar = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dark_Templar);
	int numDragoons = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon);
	int numProbes = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Probe);
	int numNexusCompleted = BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Nexus);
	int numNexusAll = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Nexus);
	int numCyber = BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Cybernetics_Core);
	int numCannon = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Photon_Cannon);

	int darkTemplarWanted = 0;
	int dragoonsWanted = numDragoons + 6;
	int gatewayWanted = 3;
	int probesWanted = numProbes + 6;

	if (InformationManager::Instance().enemyHasCloakedUnits())
	{

		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Robotics_Facility, 1));

		if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Robotics_Facility) > 0)
		{
			goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Observatory, 1));
		}
		if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Observatory) > 0)
		{
			goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Observer, 1));
		}
	}

	if (numNexusAll >= 2 || BWAPI::Broodwar->getFrameCount() > 9000)
	{
		gatewayWanted = 6;
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Robotics_Facility, 1));
	}

	if (numDragoons > 0)
	{
		goal.push_back(MetaPair(BWAPI::UpgradeTypes::Singularity_Charge, 1));
	}

	if (numNexusCompleted >= 3)
	{
		gatewayWanted = 8;
		dragoonsWanted = numDragoons + 6;
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Observer, 1));
	}

	if (numNexusAll > 1)
	{
		probesWanted = numProbes + 6;
	}

	if (expandProtossZealotRush())
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Nexus, numNexusAll + 1));
	}

	goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Dragoon, dragoonsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Gateway, gatewayWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Dark_Templar, darkTemplarWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Probe, std::min(90, probesWanted)));

	return goal;
}

const MetaPairVector StrategyManager::getProtossZealotRushBuildOrderGoal() const
{
	// the goal to return
	MetaPairVector goal;

	int numZealots = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot);
	int numDragoons = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon);
	int numProbes = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Probe);
	int numNexusCompleted = BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Nexus);
	int numNexusAll = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Nexus);
	int numCyber = BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Cybernetics_Core);
	int numCannon = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Photon_Cannon);

	int zealotsWanted = numZealots + 8;
	int dragoonsWanted = numDragoons;
	int gatewayWanted = 3;
	int probesWanted = numProbes + 4;

	if (InformationManager::Instance().enemyHasCloakedUnits())
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Robotics_Facility, 1));

		if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Robotics_Facility) > 0)
		{
			goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Observatory, 1));
		}
		if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Observatory) > 0)
		{
			goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Observer, 1));
		}
	}

	if (numNexusAll >= 2 || BWAPI::Broodwar->getFrameCount() > 9000)
	{
		gatewayWanted = 6;
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Assimilator, 1));
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Cybernetics_Core, 1));
	}

	if (numCyber > 0)
	{
		dragoonsWanted = numDragoons + 2;
		goal.push_back(MetaPair(BWAPI::UpgradeTypes::Singularity_Charge, 1));
	}

	if (numNexusCompleted >= 3)
	{
		gatewayWanted = 8;
		dragoonsWanted = numDragoons + 6;
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Observer, 1));
	}

	if (numNexusAll > 1)
	{
		probesWanted = numProbes + 6;
	}

	if (expandProtossZealotRush())
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Nexus, numNexusAll + 1));
	}

	goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Dragoon, dragoonsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Zealot, zealotsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Gateway, gatewayWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Protoss_Probe, std::min(90, probesWanted)));

	return goal;
}

const MetaPairVector StrategyManager::getTerranBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	int numMedics = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Medic);
	int numVultures = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Vulture);
	int numFirebats = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Firebat);

	int scvsWanted;
	int marinesWanted;
	int medicsWanted;
	int vulturesWanted;
	int firebatsWanted;

	scvsWanted += 60;

	goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Terran_SCV, std::min(90, scvsWanted)));

	return (const std::vector< std::pair<MetaType, UnitCountType> >)goal;

	//// the goal to return
	//std::vector< std::pair<MetaType, UnitCountType> > goal;

	//int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	//int numMedics = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Medic);
	//int numWraith = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Wraith);

	//int marinesWanted = numMarines + 12;
	//int medicsWanted = numMedics + 2;
	//int wraithsWanted = numWraith + 4;

	////goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Terran_Marine,	marinesWanted));

	//int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	//int scvWanted = numSCV + 50;
	//goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, std::min(90, scvWanted)));

	//return (const std::vector< std::pair<MetaType, UnitCountType> >)goal;

	//// the goal to return
	//std::vector< std::pair<MetaType, UnitCountType> > goal;

	//int numScv = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);

	//int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	//int numMedics = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Medic);
	//int numWraith = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Wraith);

	//int marinesWanted = numMarines + 12;
	//int medicsWanted = numMedics + 2;
	//int wraithsWanted = numWraith + 4;

	//int scvWanted = numScv;

	////goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	//// ext

	//// This is working
	////int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	////int scvWanted = numSCV + 500;
	////goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Terran_SCV, std::min(90, scvWanted)));

	//scvWanted = numScv + 1;
	//marinesWanted = numMarines + 6;

	//goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, 10));
	////goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, std::min(90, scvWanted)));
	////goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));
	////goal.push_back(MetaPair(BWAPI::TechTypes::Stim_Packs, 1));

	//// eof ext

	//return (const std::vector< std::pair<MetaType, UnitCountType> >)goal;
}

const MetaPairVector StrategyManager::getZergBuildOrderGoal() const
{
	//// the goal to return
	//std::vector< std::pair<MetaType, UnitCountType> > goal;		// ext

	//int numMutas = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Mutalisk);		// ext
	//int numHydras = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Hydralisk);		// ext

	//int mutasWanted = numMutas + 6;		// ext
	//int hydrasWanted = numHydras + 6;		// ext


	//goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Zergling, 4));		// ext

	//Extension
	MetaPairVector goal;

	int overlordsNo = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Overlord);
	int dronesNo = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Drone);
	int mutasNo = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Mutalisk);
	int hydrasNo = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Hydralisk);
	int zerglingsNo = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Zergling);

	int dronesPerHatchery = 24;
	int dronesWanted = (BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Hatchery)) * dronesPerHatchery;
	int mutasWanted = mutasNo;
	int hydrasWanted = hydrasNo + 6;
	int zerglingsWanted = zerglingsNo;

	//goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Zergling, 90));

	// If enemy is Protoss
	if (enemyRace == BWAPI::Races::Protoss)
	{
		// Anti protoss strategies
		// TODO: move to separate function later

		// Mutas vs Zealots
		int enemyZealotsNo = BWAPI::Broodwar->enemy()->allUnitCount(BWAPI::UnitTypes::Protoss_Zealot);
		int enemyDragoonsNo = BWAPI::Broodwar->enemy()->allUnitCount(BWAPI::UnitTypes::Protoss_Dragoon);

		// Enemy has Zealots but no aa units -> build Mutalisks
		if (enemyDragoonsNo < 1)
		{
			mutasWanted = mutasNo + 6;
		}
		else
		{
			hydrasWanted = hydrasNo + 6;
			zerglingsWanted = zerglingsWanted + 6;
		}

		BWAPI::Broodwar->printf("                                           DebExt: Zealots = %d", enemyZealotsNo);

		if (mutasWanted > 0)
		{
			goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Mutalisk, mutasWanted));
			goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Zerg_Flyer_Attacks, 1));
			goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Zerg_Flyer_Carapace, 1));
		}
		BWAPI::Broodwar->printf("                                           DebExt: mutasWanted = %d", mutasWanted);

		if (zerglingsWanted > 0)
		{
			goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Zergling, zerglingsWanted));
			goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Metabolic_Boost, 1));
		}

		if (hydrasWanted > 0)
		{
			goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Hydralisk, hydrasWanted));
			goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Zerg_Missile_Attacks, 1));
			goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Zerg_Carapace, 1));
		}		

	}
	else if (enemyRace == BWAPI::Races::Terran)
	{
		// Go for mass Zerglings
		zerglingsWanted = zerglingsWanted + 6;
		goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Zergling, zerglingsWanted));
		goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Metabolic_Boost, 1));
		goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Zerg_Melee_Attacks, 1));
		goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Zerg_Carapace, 1));
	}
	else
	{
		zerglingsWanted = zerglingsWanted + 6;
		goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Zergling, zerglingsWanted));
		goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Metabolic_Boost, 1));
		goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Zerg_Melee_Attacks, 1));
		goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Zerg_Carapace, 1));
	}




	// Drones goal always active
	//dronesWanted += 2;
	//if (dronesNo < dronesWanted)
	//{
	//	goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Drone, std::min(90, dronesWanted)));
	//}

	// Works if drones to build number is less than 15
	//dronesWanted = std::max(dronesWanted, 6);
	//goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Drone, std::min(90, dronesWanted)));


	

	// this works for at least 30 drones
	//dronesWanted = dronesNo + 1;

	// This works only once. After first 15 drones are built, searching fails
	//dronesWanted = dronesNo + 15;


	
	// Terran Mirror---------------------------------------------
	//zerglingsWanted += 6;
	//dronesWanted = dronesNo + 1;
	// Constraint maximum Drones with value of 90
	//goal.push_back(MetaPair(BWAPI::UnitTypes::Zerg_Drone, std::min(90, dronesWanted)));
	//goal.push_back(MetaPair(BWAPI::UnitTypes::Zerg_Zergling, zerglingsWanted));
	//goal.push_back(MetaPair(BWAPI::UpgradeTypes::Metabolic_Boost, 1));
	// ----------------------------------------------------------
	//goal.push_back(MetaPair(BWAPI::UpgradeTypes::Zerg_Melee_Attacks, 1));

	//goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Drone, dronesNo + 30));


	// This works for infinite drones and overlords (tested on two loops
	//goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Drone, dronesNo + 4));
	//goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Overlord, overlordsNo + 1));
	// ----------

	return goal;

	// eof Extension






	////////////////////////////////////////////////////////////////

	//std::vector< std::pair<MetaType, UnitCountType> > goal;

	//int numMutas = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Mutalisk);
	//int numHydras = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Hydralisk);

	//int mutasWanted = numMutas + 6;
	//int hydrasWanted = numHydras + 6;

	//goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Hydralisk, 4));

	//return (const std::vector< std::pair<MetaType, UnitCountType> >)goal;

	////////////////////////////////////////////////////////////////
	//MetaPairVector goal;

	//int numMutas = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Mutalisk);
	//int numHydras = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Zerg_Hydralisk);

	//int mutasWanted = numMutas + 6;
	//int hydrasWanted = numHydras + 6;

	//goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Hydralisk, 4));
	//goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Drone, 4));

	//return goal;

	////////////////////////////////////////////////////////////////


	//goal.push_back(std::pair<MetaType, int>(BWAPI::TechTypes::Stim_Packs,	1));

	//goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Terran_Medic,		medicsWanted));

	//return (const std::vector< std::pair<MetaType, UnitCountType> >)goal;			// ext
}

const int StrategyManager::getCurrentStrategy()
{
	return currentStrategy;
}

void StrategyManager::CreateZergUsableStrategies()
{
	usableStrategies.push_back(ZergCerver4PoolRush);
	usableStrategies.push_back(Zerg9PoolHatch);
	usableStrategies.push_back(Zerg7PoolRush);
	usableStrategies.push_back(Zerg9PoolSpeedlingsRush);
}

void StrategyManager::CreateTerranUsableStrategies()
{
	// Specific build orders
	if (enemyRace == BWAPI::Races::Protoss)
	{
		usableStrategies.push_back(TerranTriRaxMnMRush);
		usableStrategies.push_back(TerranProxyRaxMarineRush);
		usableStrategies.push_back(Terran3FactoryVultureRush);
		usableStrategies.push_back(TerranGundamRush);
		usableStrategies.push_back(Terran1FastExpoDef);			// better against protoss
		usableStrategies.push_back(Terran1FastExpoNoDef);		// worse against protoss
		usableStrategies.push_back(Terran2FactMechBuild);
		usableStrategies.push_back(Terran1FastPortBuild);	

		// Multi
		usableStrategies.push_back(TerranDoubleRaxMnM);
		usableStrategies.push_back(TerranTriRaxMnMRush);
	}
	else if (enemyRace == BWAPI::Races::Terran)
	{
		usableStrategies.push_back(TerranProxyRaxMarineRush);
		usableStrategies.push_back(Terran1FastExpoNoDef);
		usableStrategies.push_back(Terran2FactMechBuild);
		usableStrategies.push_back(TerranGoliathBuild);
		usableStrategies.push_back(TerranGoliathDrop);
		usableStrategies.push_back(Terran1FastPortBuild);
		usableStrategies.push_back(Terran1FastPortBuild);
		usableStrategies.push_back(TerranWraithRush2PortsTvT);

		// Multi
		usableStrategies.push_back(Terran3FactoryVultureRush);
		usableStrategies.push_back(TerranDoubleRaxMnM);
		usableStrategies.push_back(TerranTriRaxMnMRush);
	}
	else if (enemyRace == BWAPI::Races::Zerg)
	{
		usableStrategies.push_back(TerranDoubleRaxMnM);
		usableStrategies.push_back(TerranTriRaxMnMRush);
		usableStrategies.push_back(TerranProxyRaxMarineRush);
		usableStrategies.push_back(Terran1FastPortBuild);
		usableStrategies.push_back(TerranWraithRush1Port);
		usableStrategies.push_back(TerranWraithRush2PortsTvZ);

		// Multi
		usableStrategies.push_back(Terran3FactoryVultureRush);
		usableStrategies.push_back(Terran2FactMechBuild);
	}
	else
	{
		// Multi
		usableStrategies.push_back(TerranDoubleRaxMnM);
		usableStrategies.push_back(TerranTriRaxMnMRush);
		usableStrategies.push_back(Terran3FactoryVultureRush);
		usableStrategies.push_back(Terran2FactMechBuild);
	}	
}

int StrategyManager::GetStrategyIdx()
{
	int chosenStrategy = 0;

	int strategyNo;
	if (selfRace == BWAPI::Races::Protoss)
	{
		strategyNo = NumProtossStrategies;
	}

	if (selfRace == BWAPI::Races::Terran)
	{
		//strategyNo = NumTerranStrategies;
		//chosenStrategy = GenerateRandomStrategy(0, usableStrategies.size());		// uncomment after testing
		chosenStrategy = Terran3FactoryVultureRush;			// for testing purposes

	}

	if (selfRace == BWAPI::Races::Zerg)
	{
		strategyNo = usableStrategies.size();

		// choose strategy
		chosenStrategy = GenerateRandomStrategy(0, usableStrategies.size());

	}

	BWAPI::Broodwar->printf("Chosen strategy: %d", chosenStrategy);
	return chosenStrategy;
}

int StrategyManager::GenerateRandomStrategy(const int min, const int max)
{
	srand(time(NULL));
	return rand() % max + min;
}

bool StrategyManager::doAttackZergCerver4PoolRush()
{
	// Attack with 6 Zerglings
	if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Zerg_Zergling) >= 2 || isMidGame)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool StrategyManager::doAttackZerg9PoolHatch()
{
	// Attack with 6 Zerglings
	if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Zerg_Zergling) >= 6 || isMidGame)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool StrategyManager::doAttackZerg7PoolRush()
{
	// Attack with 6 Zerglings
	if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Zerg_Zergling) >= 6 || isMidGame)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool StrategyManager::doAttackZerg9PoolSpeedlingsRush()
{
	// Attack with 6 Zerglings
	if (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Zerg_Zergling) >= 6 || isMidGame)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool StrategyManager::doAttackZergMidgame()
{
	return true;
}
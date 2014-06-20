// Modification to UAlbertaBot (University of Alberta - AIIDE StarCraft Competition)
// by David Churchill <dave.churchill@gmail.com>  
// Author: Filip C. Bober <filip.bober@gmail.com>

#include "Common.h"
#include "StrategyManager.h"

// constructor
StrategyManager::StrategyManager()
: firstAttackSent(false)
, currentStrategy(0)
, selfRace(BWAPI::Broodwar->self()->getRace())
, enemyRace(BWAPI::Broodwar->enemy()->getRace())
, isMidGame(false)
, isAttackOrderGranted(true)
, isAttackGrantedPermanently(false)
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

	protossOpeningBook[ProtossZealotRush] = "0 0 0 0 1 0 3 3 0 0 4 1 4 4 0 4 4 0 1 4 3 0 1 0 4 0 4 4 4 4 1 0 4 4 4";
	protossOpeningBook[ProtossDarkTemplar] = "0 0 0 0 1 0 3 0 7 0 5 0 12 0 13 3 22 22 1 22 22 0 1 0";
	protossOpeningBook[ProtossDragoons] = "0 0 0 0 1 0 0 3 0 7 0 0 5 0 0 3 8 6 1 6 6 0 3 1 0 6 6 6";
	
	//terranOpeningBook[TerranMarineRush] = "0 0 0 0 0 1 0 0 3 0 0 3 0 1 0 4 0 0 0 6";			// deprecated

	terranOpeningBook[TerranMarineRush] = "0 0 0 0 0 17 0 0 19 0 0 19 17 0 18 0 0 20";
	terranOpeningBook[TerranDoubleRaxMnM] = "0 0 0 0 0 17 0 19 0 0 17 18 17 1 19 0 50 0 0 50 0 1 20 0 1 15 37";
	terranOpeningBook[TerranTriRaxMnMRush] = "0 0 0 0 0 17 0 0 19 0 0 19 0 0 17 19 18 21 20 32";
	terranOpeningBook[TerranProxyRaxMarineRush] = "0 0 17 0 0 0 0 19 19 0 0 1 1";		// hard to implement
	terranOpeningBook[Terran3FactoryVultureRush] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 1 0 0 22 1 0 22 1 0 23 21 17 0 3 0 0 47 0 0 3 23 0 17 0 22 0 0 0 30 38";		// <- Preferred build order
	terranOpeningBook[TerranGundamRush] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 1 22 0 1 0 1 22 1 0 23 17 1 0 7 3 38 0 1 17 43";				// hard to implement, check if tanks are build
	terranOpeningBook[Terran1FastExpoDef] =		"0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 0 1 0 1 23 1 0 17 7 43 14";							// check if command center is build in a proper rotation
	terranOpeningBook[Terran1FastExpoNoDef] =	"0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 0 23 0 0 38 0 7 17 0 43 7 21";
	terranOpeningBook[Terran2FactMechBuild] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 22";
	terranOpeningBook[TerranGoliathBuild] = "0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 0 22 0 0 22";
	terranOpeningBook[TerranGoliathDrop] = "0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 0 22 0 0 22 0 25 24 0 23 17 0 0 26 48 6 6 17 9 6 6";			// hard to implement - needs Dropship micro
	terranOpeningBook[Terran1FastPortBuild] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 1 0 1 0 25 23 1 17 0 7 0 43 26 0 7 21 9";				// hard to implement - needs Dropship micro
	//terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 1 0 1 0 25 1 0 17 1 20 10 50 19 0 26 50 21 0 30 0 27 39";
	//terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 0 17 0 0 19 0 18 0 0 17 1 0 22"; // base build
	//terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 0 17 0 0 19 0 18 0 0 17 1 0 22 1 1 25 0 25 0 17 1 26 10 20 10 10 39 19 21 27"; // base 2 ports
	//terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 0 17 0 0 19 0 18 0 0 17 1 0 22 1 1 25 0 25 0 17 1 26 10 20 10 10 39 19 21 27";
	//terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 0 17 0 19 0 0 18 0 1 0 1 50 20 17 0 1 0 1 0 4 0 1 50 22 1 1 4 0 1 0 1 50 25 0 17 1 0 1 26 0 1 10 4 10 4 10 39 10 10";		// Against UAlberta Zealot
	//terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 0 17 0 0 19 0 18 0 0 17 1 0 22 1 1 25 0 25 0 17 1 26 10 20 10 10 39 19 21 27";		// Wins with default Protoss
	//terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 0 17 0 0 19 0 18 0 0 17 1 0 22 1 1 25 0 25 0 17 1 26 10 20 10 10 50 39 19 21 27";
	terranOpeningBook[TerranWraithRush2PortsTvZ] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 1 0 1 0 25 0 25 17 0 1 26 10 20 10 10 39 19 21 0 27";
	terranOpeningBook[TerranWraithRush2PortsTvT] = "0 0 0 0 0 17 0 0 19 0 18 0 0 0 17 0 1 0 22 25 0 25 0 3 0 3 17 0 10 26 0 21";

	// Peters
	//terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 0 17 0 0 19 0 18 0 0 17 1 0 22 1 1 25 0 25 0 17 1 26 10 20 10 10 50 39";		// Wraith rush vs Protoss
	//terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 17 0 19 0 18 0 0 0 22 23 0 0 17 22 0 0 38 3 7 3 7 3 7 3 7 3 3";
	terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 17 0 19 0 18 0 0 17 0 22 0 0 23 0 0 17 38 3 22 0 0 17 7 0 3 0 7 0 3 0 7 3 7 3 3";

	//terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 17 0 19 0 18 0 0 0 22";  // base build -> upgrade factory OR 2nd factory: 4 tanks and 6 vultures and attack Protoss
	//terranOpeningBook[TerranWraithRush1Port] = "0 0 0 0 17 0 19 0 18 0 0 0 22 23 0 0 17 22 3 7 3 7 3 7 3 7 3 3";
	//
	
	// For testing purposes
	//terranOpeningBook[TerranMarineRush] = "0";
	//terranOpeningBook[TerranDoubleRaxMnM] = "0 0 0 0 0";			// For testing purposes
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
		createTerranUsableStrategies();
	}
	else if (selfRace == BWAPI::Races::Zerg)
	{
		results = std::vector<IntPair>(NumZergStrategies);
		//usableStrategies.push_back(ZergZerglingRush);		// ext

		// Extensions
		createZergUsableStrategies();
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
		else
		{
			// otherwise return a random strategy
			currentStrategy = getStrategyIdx();
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
		// Always perform the first attack with any trained attack unit (usually Zerglings rush)
		doAttack = true;
	}
	else if ((isMidGame == false) && (selfRace == BWAPI::Races::Zerg))
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
	else if (selfRace == BWAPI::Races::Terran)
	{


	//	TerranMarineRush = 0, TerranDoubleRaxMnM = 1,
	//		TerranTriRaxMnMRush = 2, TerranProxyRaxMarineRush = 3,
	//		Terran3FactoryVultureRush = 4, TerranGundamRush = 5,
	//		Terran1FastExpoDef = 6, Terran1FastExpoNoDef = 7,
	//		Terran2FactMechBuild = 8,
	//		TerranGoliathBuild = 9, TerranGoliathDrop = 10,
	//		Terran1FastPortBuild = 11, TerranWraithRush1Port = 12,
	//		TerranWraithRush2PortsTvZ = 13, TerranWraithRush2PortsTvT = 14,
	//		NumTerranStrategies = 15
	//};

		if (currentStrategy == TerranDoubleRaxMnM)
		{
			// TODO: implement doAttack for the current strategy
			return doAttackTerranDoubleRaxMnM();
		}
		else if (currentStrategy == TerranTriRaxMnMRush)
		{
			// TODO: implement doAttack for the current strategy
			return doAttackTerranTriRaxMnMRush();
		}
		else if (currentStrategy == TerranProxyRaxMarineRush)
		{
			// TODO: implement doAttack for the current strategy
			return true;
		}
		else if (currentStrategy == Terran3FactoryVultureRush)
		{
			// TODO: implement doAttack for the current strategy
			return doAttackTerran3FactoryVultureRush();
		}
		else if (currentStrategy == TerranGundamRush)
		{
			// TODO: implement doAttack for the current strategy
			return true;
		}
		else if (currentStrategy == Terran1FastExpoDef)
		{
			// TODO: implement doAttack for the current strategy
			return true;
		}
		else if (currentStrategy == Terran1FastExpoNoDef)
		{
			// TODO: implement doAttack for the current strategy
			return true;
		}
		else if (currentStrategy == Terran2FactMechBuild)
		{
			// TODO: implement doAttack for the current strategy
			return true;
		}
		else if (currentStrategy == TerranGoliathBuild)
		{
			// TODO: implement doAttack for the current strategy
			return true;
		}
		else if (currentStrategy == TerranGoliathDrop)
		{
			// TODO: implement doAttack for the current strategy
			return true;
		}
		else if (currentStrategy == Terran1FastPortBuild)
		{
			// TODO: implement doAttack for the current strategy
			return true;
		}
		else if (currentStrategy == TerranWraithRush1Port)
		{
			return doAttackTerranWraithRush1Port();
			//return true;
		}
		else if (currentStrategy == TerranWraithRush2PortsTvZ)
		{
			// TODO: implement doAttack for the current strategy
			return true;
		}
		else if (currentStrategy == TerranWraithRush2PortsTvT)
		{
			// TODO: implement doAttack for the current strategy
			return true;
		}
	}

	// eof Extension


	if (doAttack)
	{
		firstAttackSent = true;
	}

	return doAttack || firstAttackSent;
}

const bool StrategyManager::expandTerranDoubleRaxMnM() const
{
	// if there is no place to expand to, we can't expand
	if (MapTools::Instance().getNextExpansion() == BWAPI::TilePositions::None)
	{
		return false;
	}

	int numCommandCenter = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Command_Center);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	int frame = BWAPI::Broodwar->getFrameCount();

	// if there are more than 4 idle workers, expand
	if (WorkerManager::Instance().getNumIdleWorkers() > 4)
	{
		return true;
	}

	if ((numCommandCenter < 2) && (numMarines > 12 || frame > 9000))
	{
		return true;
	}

	if ((numCommandCenter < 2) && (numMarines > 16 || frame > 15000))
	{
		return true;
	}

	if ((numCommandCenter < 2) && (numMarines > 24 || frame > 21000))
	{
		return true;
	}

	if ((numCommandCenter < 2) && (numMarines > 30 || frame > 26000))
	{
		return true;
	}

	return false;

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
		if (getCurrentStrategy() == TerranDoubleRaxMnM)
		{
			return getTerranDoubleRaxMnMBuildOrderGoal();
		}
		else if (getCurrentStrategy() == TerranTriRaxMnMRush)
		{
			return getTerranTriRaxMnMRushBuildOrderGoal();
		}
		else if (getCurrentStrategy() == TerranProxyRaxMarineRush)
		{
			return getTerranProxyRaxMarineRushBuildOrderGoal();
		}
		else if (getCurrentStrategy() == Terran3FactoryVultureRush)
		{
			return getTerran3FactoryVultureRushBuildOrderGoal();
		}
		else if (getCurrentStrategy() == TerranGundamRush)
		{
			return getTerranGundamRushBuildOrderGoal();
		}
		else if (getCurrentStrategy() == Terran1FastExpoDef)
		{
			return getTerran1FastExpoDefBuildOrderGoal();
		}
		else if (getCurrentStrategy() == Terran1FastExpoNoDef)
		{
			return getTerran1FastExpoNoDefBuildOrderGoal();
		}
		else if (getCurrentStrategy() == Terran2FactMechBuild)
		{
			return getTerran2FactMechBuildBuildOrderGoal();
		}
		else if (getCurrentStrategy() == TerranGoliathBuild)
		{
			return getTerranGoliathBuildBuildOrderGoal();
		}
		else if (getCurrentStrategy() == TerranGoliathDrop)
		{
			return getTerranGoliathDropBuildOrderGoal();
		}
		else if (getCurrentStrategy() == Terran1FastPortBuild)
		{
			return getTerran1FastPortBuildBuildOrderGoal();
		}
		else if (getCurrentStrategy() == TerranWraithRush1Port)
		{
			return getTerranWraithRush1PortBuildOrderGoal();
		}
		else if (getCurrentStrategy() == TerranWraithRush2PortsTvZ)
		{
			return getTerranWraithRush2PortsTvZBuildOrderGoal();
		}
		else if (getCurrentStrategy() == TerranWraithRush2PortsTvT)
		{
			return getTerranWraithRush2PortsTvTBuildOrderGoal();
		}
		else
		{
			return getTerranBuildOrderGoal();
		}
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
	// Default buildOrder
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	int numMedics = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Medic);
	int numFirebats = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Firebat);

	int scvsWanted;
	int marinesWanted;
	int medicsWanted;		// marine/medic ratio: 5:1
	int firebatsWanted;

	scvsWanted = numSCV + 2;
	marinesWanted = numMarines + 3;
	medicsWanted = marinesWanted / 5;
	firebatsWanted = numMarines / 3;

	if ((BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Marine) > 4) 
		&& (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Academy) < 1))
	{
		goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Terran_Academy, 1));
	}

	if ((BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Academy) > 0))
	{
		// If stimpacks are not researched and are not being researched, do it
		if (!BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Stim_Packs) &&
			!BWAPI::Broodwar->self()->isResearching(BWAPI::TechTypes::Stim_Packs))
		{
			goal.push_back(MetaPair(BWAPI::TechTypes::Stim_Packs, 1));
		}

		// If U238 Shells are not researched and are not being researched, do it
		if ((BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells) < 1))
		{
			goal.push_back(MetaPair(BWAPI::UpgradeTypes::U_238_Shells, 1));
		}
	}

	// Research weapons and armor for bio
	int currentWeaponLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons);
	int currentArmorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor);
	if ((BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Marine) > 10))
	{
		if ((currentWeaponLevel < BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons)))
		{
			goal.push_back(MetaPair(BWAPI::UpgradeTypes::Terran_Infantry_Weapons, currentWeaponLevel + 1));
		}

		if ((currentArmorLevel < BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor)))
		{
			goal.push_back(MetaPair(BWAPI::UpgradeTypes::Terran_Infantry_Armor, currentArmorLevel + 1));
		}

	}

	goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Terran_Marine, marinesWanted));
	goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Terran_Medic, medicsWanted));
	goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Terran_Firebat, firebatsWanted));

	return goal;
}

const MetaPairVector StrategyManager::getZergBuildOrderGoal() const
{
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


		if (mutasWanted > 0)
		{
			goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Zerg_Mutalisk, mutasWanted));
			goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Zerg_Flyer_Attacks, 1));
			goal.push_back(std::pair<MetaType, int>(BWAPI::UpgradeTypes::Zerg_Flyer_Carapace, 1));
		}

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

	return goal;
}

const int StrategyManager::getCurrentStrategy()
{
	return currentStrategy;
}

void StrategyManager::createZergUsableStrategies()
{
	usableStrategies.push_back(ZergCerver4PoolRush);
	usableStrategies.push_back(Zerg9PoolHatch);
	usableStrategies.push_back(Zerg7PoolRush);
	usableStrategies.push_back(Zerg9PoolSpeedlingsRush);
}

void StrategyManager::createTerranUsableStrategies()
{
	// Specific build orders
	if (enemyRace == BWAPI::Races::Protoss)
	{
		usableStrategies.push_back(TerranTriRaxMnMRush);
		//usableStrategies.push_back(TerranProxyRaxMarineRush);			// Hard to implement
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
		//usableStrategies.push_back(TerranProxyRaxMarineRush);		// 8Hard to implement
		usableStrategies.push_back(Terran1FastExpoNoDef);
		usableStrategies.push_back(Terran2FactMechBuild);
		usableStrategies.push_back(TerranGoliathBuild);
		//usableStrategies.push_back(TerranGoliathDrop);	// Hard to implement
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
		//usableStrategies.push_back(TerranProxyRaxMarineRush);		// Hard to implement
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

int StrategyManager::getStrategyIdx()
{
	int chosenStrategy = 0;

	int strategyNo;
	if (selfRace == BWAPI::Races::Protoss)
	{
		strategyNo = NumProtossStrategies;
	}

	if (selfRace == BWAPI::Races::Terran)
	{
		//chosenStrategy = generateRandomStrategy(0, usableStrategies.size());		// uncomment after testing
		//chosenStrategy = Terran3FactoryVultureRush;			// for testing purposes
		//chosenStrategy = TerranDoubleRaxMnM;
		chosenStrategy = TerranWraithRush1Port;
		//chosenStrategy = TerranTriRaxMnMRush;

	}

	if (selfRace == BWAPI::Races::Zerg)
	{
		strategyNo = usableStrategies.size();

		// choose strategy
		chosenStrategy = generateRandomStrategy(0, usableStrategies.size());

	}

	BWAPI::Broodwar->printf("                                                        Chosen strategy: %d", chosenStrategy);
	return chosenStrategy;
}

int StrategyManager::generateRandomStrategy(const int min, const int max)
{
	srand(time(NULL));
	return rand() % max + min;
}

const MetaPairVector StrategyManager::getTerranDoubleRaxMnMBuildOrderGoal() const
{	
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	//int numSupplyDepots = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);
	//int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	//int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	//int numMedics = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Medic);
	//int numGhosts = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Ghost);

	//int supplyDepotsWanted = 0;
	//int scvsWanted = 0;
	//int marinesWanted = 0;
	//int medicsWanted = 0;		// marine/medic ratio: 5:1
	//int ghostsWanted = 0;

	//
	//supplyDepotsWanted = numSupplyDepots + 1;
	//scvsWanted = numSCV + 2;
	//marinesWanted = numMarines + 4;
	//medicsWanted = marinesWanted / 4;
	//ghostsWanted = numGhosts;		// don't build Ghosts by default

	//
	//if((BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Marine) > 4) &&
	//	(BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Academy) < 1))
	//{
	//	goal.push_back(std::pair<MetaType, int>(BWAPI::UnitTypes::Terran_Academy, 1));
	//}

	//// If scanner is not researched and is not being researched, do it
	//if (!BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Scanner_Sweep) 
	//	&& (!BWAPI::Broodwar->self()->isResearching(BWAPI::TechTypes::Scanner_Sweep)))
	//{
	//	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Comsat_Station, 1));
	//	goal.push_back(MetaPair(BWAPI::TechTypes::Scanner_Sweep, 1));
	//}
	//
	//if ((BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Academy) > 0))
	//{
	//	// If stimpacks are not researched and are not being researched, do it
	//	if (!BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Stim_Packs) &&
	//		!BWAPI::Broodwar->self()->isResearching(BWAPI::TechTypes::Stim_Packs))
	//	{
	//		goal.push_back(MetaPair(BWAPI::TechTypes::Stim_Packs, 1));
	//	}

	//	// If U238 Shells are not researched and are not being researched, do it
	//	if ((BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells) < 1))
	//	{
	//		goal.push_back(MetaPair(BWAPI::UpgradeTypes::U_238_Shells, 1));
	//	}
	//}

	//// Research weapons and armor for bio
	//int currentWeaponLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons);
	//int currentArmorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor);
	//if (BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Stim_Packs))
	//{
	//	if (!(BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Infantry_Weapons))
	//		&& !(BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Infantry_Armor)))
	//	{
	//		if ((currentWeaponLevel < BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons)) 
	//			&& (currentWeaponLevel <= (currentArmorLevel + 1)))
	//		{
	//			goal.push_back(MetaPair(BWAPI::UpgradeTypes::Terran_Infantry_Weapons, currentWeaponLevel + 1));
	//		}
	//		else if ((currentArmorLevel < BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor))
	//			&& (currentArmorLevel < currentWeaponLevel + 1))
	//		{
	//			goal.push_back(MetaPair(BWAPI::UpgradeTypes::Terran_Infantry_Armor, currentArmorLevel + 1));
	//		}
	//	}

	//}

	//if ((BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor) >= 2) 
	//	&& (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons) >= 2))
	//{
	//	ghostsWanted = ghostsWanted + 3;
	//	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Ghost, ghostsWanted));
	//}
	//
	//if ((BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Refinery) < 1))
	//{
	//	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Refinery, 1));
	//}

	//// Push goals
	//if (numSCV < 72)
	//{
	//	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	//}
	//goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));
	//goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Medic, medicsWanted));
	//goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Supply_Depot, supplyDepotsWanted));













	//int buildComsatFrame = 4000;	
	//int expandOneFrame = 6000;

	//int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	//int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	//int numMedics = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Medic);
	//
	//int numCommandCentersAll = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Command_Center);
	//int numTerranAcademyAll = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy);
	//int numTerranEngineeringBay = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay);

	//int scvsWanted = 0;
	//int marinesWanted = 0;
	//int medicsWanted = 0;		// marine/medic ratio: 5:1

	//BWAPI::Player* self = BWAPI::Broodwar->self();
	//bool isAcademyCompleted = (self->completedUnitCount(BWAPI::UnitTypes::Terran_Academy) > 0);
	//bool isAcademyConstructing = (self->incompleteUnitCount(BWAPI::UnitTypes::Terran_Academy) > 1);
	//bool isAcademy = (self->allUnitCount(BWAPI::UnitTypes::Terran_Academy) > 1);

	//bool isEngineeringBayCompleted = (self->completedUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay) > 0);
	//bool isEngineeringBayConstructing = (self->incompleteUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay) > 1);
	//bool isEngineeringBay = (self->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay) > 1);

	//bool isComsatCompleted = (self->completedUnitCount(BWAPI::UnitTypes::Terran_Comsat_Station) > 1);
	//bool isComsatConstructing = (self->incompleteUnitCount(BWAPI::UnitTypes::Terran_Comsat_Station) > 1);
	//bool isComsat = (self->allUnitCount(BWAPI::UnitTypes::Terran_Comsat_Station) > 1);

	//bool isStimpackResearched = (self->hasResearched(BWAPI::TechTypes::Stim_Packs));
	//bool isStimackResearching = (self->isResearching(BWAPI::TechTypes::Stim_Packs));

	//bool isShellsUpgraded = (self->getUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells) == 1);
	//bool isShellsUpgrading = (self->isUpgrading(BWAPI::UpgradeTypes::U_238_Shells));

	//int currentInfantryWeaponsUpgrade = self->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons);
	//bool isInfantryWeaponsUpgrading = (self->isUpgrading(BWAPI::UpgradeTypes::Terran_Infantry_Weapons));
	//bool isInfantryWeaponsUpgraded = (self->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons)
	//	== BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons));

	//int currentInfantryArmorUpgrade = self->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor);
	//bool isInfantryArmorUpgrading = (self->isUpgrading(BWAPI::UpgradeTypes::Terran_Infantry_Armor));
	//bool isInfantryArmorUpgraded = (self->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor)
	//	== BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor));

	//scvsWanted = numSCV + 2;
	//marinesWanted = numMarines + 2;

	//if (marinesWanted > 5)
	//{
	//	medicsWanted = marinesWanted / 5;
	//}

	//goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, std::min(56, scvsWanted)));
	////goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	//goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	////Without this check there would be crash during goal searching
	//if (medicsWanted > 0)
	//{
	//	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Medic, medicsWanted));
	//}

	//if (!isAcademy)
	//{
	//	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Academy, numTerranAcademyAll + 1));
	//}
	//
	//// Research Stimpacks after Academy is built
	//if ((isAcademyCompleted)
	//	&& (isAcademy)
	//	&& (!isStimpackResearched)
	//	&& (!isStimackResearching))
	//{
	//	goal.push_back(MetaPair(BWAPI::TechTypes::Stim_Packs, 1));
	//}

	//// Research shells after Stimpacks are researched
	//if ((isStimpackResearched)
	//	&& (isAcademyCompleted)
	//	&& (isAcademy)
	//	&& (!isShellsUpgraded)
	//	&& (!isShellsUpgrading))
	//{
	//	goal.push_back(MetaPair(BWAPI::UpgradeTypes::U_238_Shells, 1));
	//}

	//// Build Engineering Bay after Stimpacks are researched
	//if ((isStimpackResearched)
	//	&& (!isEngineeringBay))
	//{
	//	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Engineering_Bay, numTerranEngineeringBay + 1));
	//}

	//if (isEngineeringBayCompleted
	//	&& isEngineeringBay)
	//{
	//	if (!isInfantryWeaponsUpgraded
	//		&& !isInfantryWeaponsUpgrading
	//		&& !isInfantryArmorUpgrading
	//		&& (currentInfantryWeaponsUpgrade < (currentInfantryArmorUpgrade + 1)))
	//	{
	//		goal.push_back(MetaPair(BWAPI::UpgradeTypes::Terran_Infantry_Weapons, currentInfantryWeaponsUpgrade + 1));
	//	}
	//	else if (!isInfantryArmorUpgraded
	//		&& !isInfantryArmorUpgrading)
	//	{
	//		goal.push_back(MetaPair(BWAPI::UpgradeTypes::Terran_Infantry_Armor, currentInfantryArmorUpgrade + 1));
	//	}
	//}

	// There is a risk of Dark Templar rush, build Comsat Station
	//if ((enemyRace == BWAPI::Races::Protoss)		
	//	&& (!isComsat)
	//	&& isAcademyCompleted
	//	&& isAcademy
	//	&& (numCommandCentersAll > 0))
	//{
	//	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Comsat_Station, 1));
	//}
	//else if ((BWAPI::Broodwar->getFrameCount() > buildComsatFrame)
	//	&& !isComsat
	//	&& isAcademyCompleted
	//	&& (numCommandCentersAll > 0))
	//{
	//	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Comsat_Station, 1));
	//}

	BWAPI::Player* self = BWAPI::Broodwar->self();
	//int buildComsatFrame = 4000;	
	//int expandOneFrame = 6000;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	int numMedics = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Medic);
	//
	int numCommandCentersAll = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Command_Center);
	//int numTerranAcademyAll = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy);
	//int numTerranEngineeringBay = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay);
	//int numBunkers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker);

	int scvsWanted = 0;
	int marinesWanted = 0;
	int medicsWanted = 0;		// marine/medic ratio: 5:1


	//int academiesWanted = 0;
	//int engineeringBaysWanted = 0;
	//int comsatStationsWanted = 0;
	//int bunkersWanted = 0;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 3;
	medicsWanted = numMarines / 5;		

	//academiesWanted = 1;
	//engineeringBaysWanted = 1;
	//comsatStationsWanted = numCommandCentersAll;
	//bunkersWanted = 2;

	//goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Academy, academiesWanted));
	//goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Engineering_Bay, engineeringBaysWanted));
	//goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Bunker, bunkersWanted));
	//
	// Expand
	if (expandTerranDoubleRaxMnM())
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Command_Center, numCommandCentersAll + 1));
	}


	//// Stimpacks
	//if (!(self->hasResearched(BWAPI::TechTypes::Stim_Packs))
	//	&& (numTerranAcademyAll > 0))
	//{
	//	goal.push_back(MetaPair(BWAPI::TechTypes::Stim_Packs, 1));
	//}

	//// U238 Shells
	//if ((self->getUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells) < 1)
	//	&& !(self->isUpgrading(BWAPI::UpgradeTypes::U_238_Shells))
	//	&& (self->hasResearched(BWAPI::TechTypes::Stim_Packs))
	//	&& (numTerranAcademyAll > 0))
	//{
	//	goal.push_back(MetaPair(BWAPI::UpgradeTypes::U_238_Shells, 1));
	//}
	

	//goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Comsat_Station, comsatStationsWanted));
	
	if (scvsWanted > numSCV)
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, std::min(56, scvsWanted)));
	}

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));
	if (medicsWanted > 0)
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Medic, medicsWanted));
	}


	// Caused crash, probably.
	if (!(self->getUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells) == self->getMaxUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells))
		&& (self->allUnitCount(BWAPI::UnitTypes::Terran_Academy))
		&& !(self->isUpgrading(BWAPI::UpgradeTypes::U_238_Shells))
		)
	{
		goal.push_back(MetaPair(BWAPI::UpgradeTypes::U_238_Shells, 1));
	}



	return goal;
}

const MetaPairVector StrategyManager::getTerranTriRaxMnMRushBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	int numMedics = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Medic);

	int numAcademies = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy);

	int scvsWanted;
	int marinesWanted;
	int medicsWanted;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 4;
	medicsWanted = 1 + (numMarines / 4);

	if (numAcademies < 1)
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Academy, 1));
	}

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Medic, medicsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));
	


	return goal;
}

const MetaPairVector StrategyManager::getTerranProxyRaxMarineRushBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);

	int scvsWanted;
	int marinesWanted;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 3;

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	return goal;
}

const MetaPairVector StrategyManager::getTerran3FactoryVultureRushBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	int numVultures = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Vulture);
	int numTanks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode)
		+ BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode);
	int numTurrets = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Missile_Turret);
	int numGoliaths = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Goliath);
	int numScienceVessels = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Vessel);

	int currentVehicleWeaponLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons);
	int currentVehicleArmorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Vehicle_Plating);

	int scvsWanted = 0;
	int marinesWanted = 0;
	int vulturesWanted = 0;
	int tanksWanted = 0;
	int turretsWanted = 0;
	int goliathsWanted = 0;
	int scienceVesselsWanted = 0;

	if (numSCV < 72)
	{
		scvsWanted = numSCV + 3;
	}

	int frame = BWAPI::Broodwar->getFrameCount();

	if (numTurrets < 2)
	{
		turretsWanted = numTurrets + 1;
	}

	// Since ~6min
	if (frame > 9000)
	{
		// Upgrade vehicles
		if (!(BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Tank_Siege_Mode)))
		{
			goal.push_back(MetaPair(BWAPI::TechTypes::Tank_Siege_Mode, 1));
		}
		else if ((BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Charon_Boosters) < 1))
		{
			goal.push_back(MetaPair(BWAPI::UpgradeTypes::Charon_Boosters, 1));
		}
		else
		{
			if (!(BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons))
				&& !(BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Vehicle_Plating)))
			{
				if ((currentVehicleWeaponLevel < BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons))
					&& (currentVehicleWeaponLevel <= (currentVehicleArmorLevel + 1)))
				{
					goal.push_back(MetaPair(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons, currentVehicleWeaponLevel + 1));
				}
				else if ((currentVehicleArmorLevel < BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Vehicle_Plating))
					&& (currentVehicleArmorLevel < currentVehicleWeaponLevel + 1))
				{
					goal.push_back(MetaPair(BWAPI::UpgradeTypes::Terran_Vehicle_Plating, currentVehicleArmorLevel + 1));
				}
			}
		}

		// Build science vessels for detector
		if (frame > 11000)
		{
			if (numScienceVessels < 3)
			{
				scienceVesselsWanted = numScienceVessels + 1;
			}
		}

		// Build tanks and goliaths for aa
		if ((currentVehicleArmorLevel > 0) && (currentVehicleWeaponLevel > 0))
		{
			tanksWanted = numTanks + 3;
			goliathsWanted = numGoliaths + 2;
		}
		else
		{
			tanksWanted = numTanks + 1;		
			goliathsWanted = numGoliaths + 1;			
		}		

		vulturesWanted = numVultures + 1;
	}
	else
	{
		vulturesWanted = numVultures + 6;				
	}

	// Research scanner
	if (!BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Scanner_Sweep)
		&& (!BWAPI::Broodwar->self()->isResearching(BWAPI::TechTypes::Scanner_Sweep)))
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Comsat_Station, 1));
		goal.push_back(MetaPair(BWAPI::TechTypes::Scanner_Sweep, 1));
	}

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Vulture, vulturesWanted));	
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode, tanksWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Goliath, goliathsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Science_Vessel, scienceVesselsWanted));

	return goal;
}

const MetaPairVector StrategyManager::getTerranGundamRushBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);

	int scvsWanted;
	int marinesWanted;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 3;

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	return goal;
}

const MetaPairVector StrategyManager::getTerran1FastExpoDefBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);

	int scvsWanted;
	int marinesWanted;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 3;

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	return goal;
}

const MetaPairVector StrategyManager::getTerran1FastExpoNoDefBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);

	int scvsWanted;
	int marinesWanted;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 3;

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	return goal;
}

const MetaPairVector StrategyManager::getTerran2FactMechBuildBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);

	int scvsWanted;
	int marinesWanted;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 3;

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	return goal;
}

const MetaPairVector StrategyManager::getTerranGoliathBuildBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);

	int scvsWanted;
	int marinesWanted;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 3;

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	return goal;
}

const MetaPairVector StrategyManager::getTerranGoliathDropBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);

	int scvsWanted;
	int marinesWanted;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 3;

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	return goal;
}

const MetaPairVector StrategyManager::getTerran1FastPortBuildBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	int numBattlecruisers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Battlecruiser);

	int scvsWanted;
	int marinesWanted;
	int battleCruisersWanted = 0;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 3;

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	return goal;
}
const MetaPairVector StrategyManager::getTerranWraithRush1PortBuildOrderGoal() const
{


	std::vector< std::pair<MetaType, UnitCountType> > goal;
	int frames = BWAPI::Broodwar->getFrameCount();

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	int numWraiths = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Wraith);
	int numBattlecruisers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Battlecruiser);
	int numFirebats = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Firebat);

	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	int numControlTowers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Control_Tower);
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);
	int numPhysicsLabs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Physics_Lab);
	int numBunkers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker);
	int numAcademies = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy);

	int scvsWanted = 0;
	int marinesWanted = 0;
	int wraithsWanted = 0;
	int battleCruisersWanted = 0;
	int firebatsWanted = 0;

	int bunkersWanted = 0;

	scvsWanted = numSCV + 3;
	//marinesWanted = numMarines + 3;
	wraithsWanted = numWraiths + 2;
	bunkersWanted = numBunkers + 1; // 2 + (marinesWanted / 4);


	// Battlecruiser pipeline

	// If there is no Starport, build one
	if (numStarports < 1)
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Starport, numStarports + 1));
	}
	// Make sure that every Starport has a Control Tower attached
	else if (numControlTowers < numStarports)
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Control_Tower, numControlTowers + 1));
	}
	// Build a Science Facility
	else if (numScienceFacilities < 1)
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Science_Facility, 1));
	}
	// Attach Physics Lab to the Science Facility (should be one anyway, so constraints are not needed)
	else if (numPhysicsLabs < numScienceFacilities)
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Physics_Lab, 1));
	}
	// BC requirements are met. Start building BC's - as many as there are Starports with Control Towers
	else
	{
		goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Battlecruiser, std::min(numControlTowers, numStarports)));
	}
	// eof Battlecruiser

	// Firebat pipeline
	if (numAcademies > 0)
	{
		firebatsWanted = 1 + (numMarines / 2);
		goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Firebat, firebatsWanted));
	}
	// eof Firebat

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Wraith, wraithsWanted));

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Bunker, bunkersWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Missile_Turret, numBunkers + 1));

	

	return goal;
}

const MetaPairVector StrategyManager::getTerranWraithRush2PortsTvZBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);

	int scvsWanted;
	int marinesWanted;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 3;

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	return goal;
}

const MetaPairVector StrategyManager::getTerranWraithRush2PortsTvTBuildOrderGoal() const
{
	std::vector< std::pair<MetaType, UnitCountType> > goal;

	int numSCV = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);

	int scvsWanted;
	int marinesWanted;

	scvsWanted = numSCV + 3;
	marinesWanted = numMarines + 3;

	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_SCV, scvsWanted));
	goal.push_back(MetaPair(BWAPI::UnitTypes::Terran_Marine, marinesWanted));

	return goal;
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

bool StrategyManager::doAttackTerran3FactoryVultureRush()
{
	if (isMidGame)
	{
		return true;
	}
	else if ((!isMidGame) && (BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Terran_Vulture >= 3)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool StrategyManager::doAttackTerranDoubleRaxMnM()
{
	if (!isMidGame)
	{
		return true;
	}
	else
	{
		//int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
		//int numMedics = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Medic);

		//if ((numMarines >= 10)
		//	&& (numMedics >= 3))
		//{
		//	return true;
		//}
		//else
		//{
		//	return false;
		//}
		//return true;

		if (BWAPI::Broodwar->getFrameCount() > 8000)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool StrategyManager::doAttackTerranWraithRush1Port()
{
	// Code below causes SC to crash
	//if (!isMidGame)
	//{
	//	return true;
	//}

	//int frames = BWAPI::Broodwar->getFrameCount();
	//if (frames % 1000 == 0)
	//{
	//	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	//	int numWraiths = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Wraith);
	//	int numBattlecruisers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Battlecruiser);

	//	if ((numMarines > 5)
	//		&& numWraiths > 3)
	//	{
	//		isAttackOrderGranted = true;
	//	}
	//	else
	//	{
	//		isAttackOrderGranted = false;
	//	}
	//}

	//return isAttackOrderGranted;

	//return true;





	return true;



	if (isAttackGrantedPermanently)
	{
		BWAPI::Broodwar->printf("                                           DebExt: Attack granted");
		return true;
	}

	int numWraiths = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Wraith);
	int numBCs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Battlecruiser);

	if (isAttackOrderGranted
		&& ((BWAPI::Broodwar->enemy()->allUnitCount(BWAPI::UnitTypes::Protoss_Photon_Cannon) > 1)
		|| (BWAPI::Broodwar->enemy()->allUnitCount(BWAPI::UnitTypes::Terran_Missile_Turret) > 1)
		|| (BWAPI::Broodwar->enemy()->allUnitCount(BWAPI::UnitTypes::Zerg_Spore_Colony) > 1)))
	{
		isAttackOrderGranted = false;
		BWAPI::Broodwar->printf("                                           DebExt: Attack not granted");
	}

	if ((numBCs > 4)
		&& (BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Yamato_Gun)))
	{
		BWAPI::Broodwar->printf("                                           DebExt: Attack granted");
		isAttackGrantedPermanently = true;
	}

	return isAttackOrderGranted || isAttackGrantedPermanently;
}

bool StrategyManager::doAttackTerranTriRaxMnMRush()
{
	if (!isMidGame)
	{
		return false;
	}

	int frames = BWAPI::Broodwar->getFrameCount();
	if (frames % 1000 == 0)
	{
		int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
		int numMedics = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Medic);

		if ((numMarines > 30)
			&& numMedics > 5)
		{
			isAttackOrderGranted = true;
		}
		else
		{
			isAttackOrderGranted = false;
		}
	}

	return isAttackOrderGranted;

}
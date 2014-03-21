#include "ZergBuildOrderCreator.h"


ZergBuildOrderCreator::ZergBuildOrderCreator()
{
}


ZergBuildOrderCreator::~ZergBuildOrderCreator()
{
}

std::vector<MetaType>  ZergBuildOrderCreator::GenerateQueue()
{
	//
	// Choose the right strategy against known opponent or randomize opening
	// ...
	//

	// Insert items into ProductionManager
	std::vector<MetaType> buildOrder;
	

	buildOrder = Cerver4PoolPush();

	//buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));


	//ProductionManager::Instance().setBuildOrder(buildOrder);
	return buildOrder;
}

std::vector<MetaType> ZergBuildOrderCreator::Cerver4PoolPush()
{
	// Matchups: ZvP, ZvT, ZvZ
	// Small maps, where opponent location is known

	std::vector<MetaType> buildOrder;

	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Spawning_Pool));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));

	// 3 sets of Zerglings
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Zergling));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Zergling));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Zergling));

	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));

	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Overlord));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Hatchery));

	// 3 more sets for defense
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Zergling));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Zergling));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Zergling));

	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Extractor));

	// 10/17 -> 13/17
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));

	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Lair));

	// TODO: 3 sets of Zerglings
	// TODO: Rush as soon as these come out



	return buildOrder;
}
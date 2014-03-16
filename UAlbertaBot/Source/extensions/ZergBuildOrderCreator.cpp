#include "ZergBuildOrderCreator.h"


ZergBuildOrderCreator::ZergBuildOrderCreator()
{
}


ZergBuildOrderCreator::~ZergBuildOrderCreator()
{
}

std::vector<MetaType>  ZergBuildOrderCreator::GenerateQueue()
{
	// Insert items into ProductionManager
	std::vector<MetaType> buildOrder;

	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Zerg_Drone));

	//ProductionManager::Instance().setBuildOrder(buildOrder);
	return buildOrder;
}

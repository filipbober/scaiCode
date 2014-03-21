#include "BuildOrderTest.h"


BuildOrderTest::BuildOrderTest()
{
}


BuildOrderTest::~BuildOrderTest()
{
}

std::vector<MetaType> BuildOrderTest::GenerateTestQueue()
{

	// Insert items into ProductionManager
	std::vector<MetaType> buildOrder;

	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Protoss_Probe));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Protoss_Probe));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Protoss_Pylon));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Protoss_Gateway));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Protoss_Gateway));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Protoss_Zealot));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Protoss_Zealot));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Protoss_Zealot));
	buildOrder.push_back(MetaType(BWAPI::UnitTypes::Protoss_Zealot));

	//ProductionManager::Instance().setBuildOrder(buildOrder);
	return buildOrder;

}
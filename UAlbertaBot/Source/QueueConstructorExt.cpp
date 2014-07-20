#include "QueueConstructorExt.h"
#include <math.h>


QueueConstructorExt::QueueConstructorExt()
:
_lastInvoked(BWAPI::Broodwar->getFrameCount())
, _airThreatExists(false)
, _isDetectorNeeded(false)
{
}


QueueConstructorExt::~QueueConstructorExt()
{
}

QueueConstructorExt& QueueConstructorExt::Instance()
{
	static QueueConstructorExt instance;
	return instance;
}

BuildOrderQueue QueueConstructorExt::getQueue()
{
	return _queue;
}

void QueueConstructorExt::clearQueue()
{
	_queue.clearAll();
}

void QueueConstructorExt::makeExpansion()
{
	if (BWAPI::Broodwar->self()->supplyUsed() < 20)
	{
		return;
	}

	int frame = BWAPI::Broodwar->getFrameCount();
	int numCommandCenters = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Command_Center);

	if ((numCommandCenters < 2)
		&& (frame > 10000))
	{
		queueCommandCenters(2);
	}

	if (numCommandCenters < 3
		&& frame > 16000)
	{
		queueCommandCenters(3);
	}
	//else if ((numCommandCenters < 3)
	//	&& (frame > 14000))
	//{
	//	queueCommandCenters(3);
	//}
}

void QueueConstructorExt::makeTestQueue()
{
	BWAPI::Broodwar->printf("                                           DebExt: doSomething invoked");

	//int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);

	//int numBunkers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker);
	//int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	//int numAcademies = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy);

	//if (numBunkers <= (numMarines / 4))
	//{
	//	_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Bunker), true);
	//}

	//if (numStarports < 3)
	//{
	//	_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Starport), true);
	//}

	//if ((BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV) < 56))
	//{
	//	_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_SCV), true);
	//}

	//if (numAcademies > 0)
	//{
	//	_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Firebat), true);
	//}

	//_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Marine), true);
	//_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Marine), true);

	//for (int i = 0; i < numStarports; i++)
	//{
	//	_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Wraith), true);
	//}


	//queueTerranMarines(0.5);
	//queueTerranFirebats(0.5);

	//// Working well against default Protoss
	//queueTerranWraiths(1.0);
	//queueTerranWraithUpgrades();
	//queueTerranSCVs(1.0);
	//// ---










	// Wraiths
	//int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);

	//queueTerranWraiths(1.0);

	//if (BWAPI::Broodwar->self()->supplyTotal() < BWAPI::Broodwar->self()->supplyUsed() + 5)
	//{
	//	queueTerranSupply(numSupply + 1);
	//}

	//queueTerranMarines(0.7);
	//queueTerranFirebats(0.3);
	//queueTerranSCVs(1.0);




	




	// Tanks
	int numScvs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);

	int minerals = BWAPI::Broodwar->self()->minerals();
	int gas = BWAPI::Broodwar->self()->gas();
	int frame = BWAPI::Broodwar->getFrameCount();

	//if ((BWAPI::Broodwar->getFrameCount() % 120) != 0)
	//{
	//	return;
	//}

	// To prevent performance issues
	if (BWAPI::Broodwar->self()->supplyTotal() <= BWAPI::Broodwar->self()->supplyUsed())
	{
		queueTerranSupply(numSupply + 1);
		cleanQueue();
		return;
	}
	else if (BWAPI::Broodwar->self()->supplyUsed() > (190 * 2))
	{
		// TODO tech, upgrade etc.
		// TODO - control supply while building units (ensure there is enough)
		queueTerranBunkers(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1);
		return;
	}



	// Code below does slow the game in late game! <------------------------------------------------------------------------------------
	//bool underConstruction = (BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Factory) > 0) 
	//	|| (BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Bunker));
	//if ((minerals > 300)
	//	&& (!underConstruction))
	//{
	//	queueTerranFactories(std::min((BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory) + 1), 5));
	//	queueTerranBunkers(std::min((BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1), 5));
	//}

	

	queueTerranMarines(1.0);
	queueTerranMarines(1.0);
	queueTerranMarines(1.0);

	queueTerranVultures(1.0);
	queueTerranTanks(0.3);

	if (numScvs < 48)
	{
		queueTerranSCVs(1.0);
		queueTerranSCVs(1.0);
	}

	if (frame > 10000)
	{
		queueTechVultures();
		queueTerranFactories(3);
	}

	if (frame > 12000)
	{
		queueTerranFactories(4);
	}

	if (frame > 13000)
	{
		queueTechTanks();
	}

	makeExpansion();

	// TODO: if supply is higher than 110 -> go for battlecruisers 

	if (isAirThreat())
	{
		//queueTerranGoliaths(0.3);
		//queueTechGoliaths();
		queueTerranWraiths(1.0);
	}

	// Supply MUST be at the end (highest priority). Otherwise performance issues occur (units can't be build but are inserted before supply)
	if (BWAPI::Broodwar->self()->supplyTotal() < BWAPI::Broodwar->self()->supplyUsed() + 5)
	{
		queueTerranSupply(numSupply + 3);
	}

	// eof tanks










	//// Vultures
	//int numScvs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	//int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);

	//if (BWAPI::Broodwar->self()->supplyTotal() < BWAPI::Broodwar->self()->supplyUsed() + 5)
	//{
	//	queueTerranSupply(numSupply + 1);
	//}

	//

	//queueTerranMarines(1.0);
	//queueTerranVultures(1.0);

	//if (numScvs < 48)
	//{
	//	queueTerranSCVs(1.0);
	//}

	//queueTechVultures();
	//makeExpansion();
	//
	//// eof Vultures








	//int numScvs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	//int numBcs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Battlecruiser);

	//int numRefineries = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Refinery);
	//int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);

	//if (numScvs < 10)
	//{
	//	queueTerranSCVs(1.0);
	//}
	//else
	//{
	//	if (numRefineries < 1)
	//	{
	//		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Refinery), true);
	//	}



	//	if (numBcs > 0)
	//	{
	//		queueTerranBCs(1.0);
	//		queueTerranBCUpgrades();
	//	}
	//	else
	//	{
	//		queueTerranWraiths(1.0);
	//		queueTerranBCs(0.5);
	//	}

	//	queueTerranMarines(0.7);
	//	queueTerranFirebats(0.3);
	//	queueTerranSCVs(1.0);

	//	if (BWAPI::Broodwar->self()->supplyTotal() < BWAPI::Broodwar->self()->supplyUsed() + 5)
	//	{
	//		queueTerranSupply(numSupply + 1);
	//	}
	//}

	cleanQueue();
}

void QueueConstructorExt::makeTerranVulturesAndTanksQueue()
{
	//if (_lastInvoked + 240 > BWAPI::Broodwar->getFrameCount())
	//{
	//	return;
	//}
	//else
	//{
	//	_lastInvoked = BWAPI::Broodwar->getFrameCount();
	//}


	//int numScvs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	//int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);
	//int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	//int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);
	//int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);
	//int numTurrets = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Missile_Turret);
	//int numBunkers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker);

	//int minerals = BWAPI::Broodwar->self()->minerals();
	//int gas = BWAPI::Broodwar->self()->gas();
	//int frame = BWAPI::Broodwar->getFrameCount();

	//// To prevent performance issues
	//if ((BWAPI::Broodwar->self()->supplyTotal() <= BWAPI::Broodwar->self()->supplyUsed() + 7)
	//	&& (BWAPI::Broodwar->self()->supplyUsed() <= (200 * 2))
	//	&& !(BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot)))
	//{
	//	queueTerranSupply(numSupply + 1);
	//	queueTerranBunkers(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1);
	//	cleanQueue();
	//	return;
	//}

	//queueTechTanks();
	////queueTerranVultures(1.0);

	//int numScienceVessels = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Vessel);
	//int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	//if (frame > 10000
	//	&& numScienceVessels < 1)
	//{
	//	queueTerranScienceVessels(1 / std::max(numStarports, 1));	// One is enough
	//}

	//addControlTowers();

	//if (isAirThreat() || frame > 20000)
	//{
	//	queueTerranGoliaths(0.5);
	//	queueTechGoliaths();
	//	queueTerranWraiths(1.0);
	//	queueTerranWraithUpgrades();
	//}

	//queueTerranVultures(1.0);
	//queueTerranTanks(1.0);

	//if (frame > 10000)
	//{
	//	queueTerranFactories(std::min(numFactories + 1, 4));
	//}
	////queueTerranFactories(numFactories + 1);

	//// Supply MUST be at the end (highest priority). Otherwise performance issues occur (units can't be build but are inserted before supply)	
	//int totalSupplyRequired = BWAPI::Broodwar->self()->supplyUsed() + getQueueSupply();
	//if (totalSupplyRequired >= BWAPI::Broodwar->self()->supplyTotal()
	//	&& (BWAPI::Broodwar->self()->supplyTotal() < (200*2)) )
	//{
	//	int supplyRequired = totalSupplyRequired - BWAPI::Broodwar->self()->supplyTotal();
	//	int supplyDepotsRequired = std::max(1, (int)std::ceil(supplyRequired / 8.0));

	//	supplyDepotsRequired = std::min(supplyDepotsRequired, 3);
	//	queueTerranSupply(numSupply + supplyDepotsRequired);
	//}

	//makeExpansion();

	//cleanQueue();















	if (_lastInvoked + 240 > BWAPI::Broodwar->getFrameCount())
	{
		return;
	}
	else
	{
		_lastInvoked = BWAPI::Broodwar->getFrameCount();
	}

	int numScvs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);
	int numTurrets = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Missile_Turret);
	int numBunkers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker);

	int minerals = BWAPI::Broodwar->self()->minerals();
	int gas = BWAPI::Broodwar->self()->gas();
	int frame = BWAPI::Broodwar->getFrameCount();

	queueTerranVultures(1.0);

	// To prevent performance issues
	if ((BWAPI::Broodwar->self()->supplyTotal() <= BWAPI::Broodwar->self()->supplyUsed() + 7)
		&& (BWAPI::Broodwar->self()->supplyUsed() <= (200 * 2))
		&& !(BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot)))
	{
		queueTerranSupply(numSupply + 1);
		//queueTerranBunkers(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1);
		cleanQueue();
		return;

	}
	else if (BWAPI::Broodwar->self()->supplyUsed() > (190 * 2))
	{
		queueTerranTurrets(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Missile_Turret) + 2);
		// TODO tech, upgrade etc.
		// TODO - control supply while building units (ensure there is enough)
		//queueTerranBunkers(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1);
		return;
	}

	if (minerals > 200
		&& gas > 100)
	{
		queueTerranFactories(std::min((numFactories + 1), 4));
	}	

	// Upgrades
	if (frame > 12000)
	{
		queueTechVultures();
	}

	if (frame > 14000)
	{
		queueTechMarines();
		queueTerranMarinesUpgrades();
		queueTerranTankUpgrades();
	}

	if (numTurrets < 2)
	{
		queueTerranTurrets(2);
	}

	if (numBunkers < 5)
	{
		queueTerranBunkers(std::min(numBunkers + 1, 5));
	}

	//if (minerals > 400
	//	&& BWAPI::Broodwar->self()->supplyTotal() < (200 * 2))
	//{
	//	queueTerranSupply(numSupply + 2);
	//}

	//if (minerals > 600
	//	&& BWAPI::Broodwar->self()->supplyTotal() < (200 * 2))
	//{
	//	queueTerranSupply(numSupply + 4);
	//}

	int numScienceVessels = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Vessel);
	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	if (frame > 10000
		&& numScienceVessels < 1)
	{
		queueTerranScienceVessels(1 / std::max(numStarports, 1));	// One is enough
	}


	if (isAirThreat())
	{
		queueTerranGoliaths(0.5);
		queueTechGoliaths();
		queueTerranWraiths(1.0);
		queueTerranWraithUpgrades();
	}

	if (frame > 10000)
	{
		queueTerranTanks(0.5);
		queueTerranVultures(0.5);
		queueTechTanks();
	}

	if (BWAPI::Broodwar->self()->supplyUsed() + 14 > BWAPI::Broodwar->self()->supplyTotal())
	{
		queueTerranSupply(numSupply + 1);
	}

	makeExpansion();

	cleanQueue();



























	// -------------------------------------------------------------------------------------

	//if (_lastInvoked + 240 > BWAPI::Broodwar->getFrameCount())
	//{
	//	return;
	//}
	//else
	//{
	//	_lastInvoked = BWAPI::Broodwar->getFrameCount();
	//}

	//// Tanks
	//int numScvs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	//int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);
	//int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	//int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);
	//int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);

	//int minerals = BWAPI::Broodwar->self()->minerals();
	//int gas = BWAPI::Broodwar->self()->gas();
	//int frame = BWAPI::Broodwar->getFrameCount();

	//// To prevent performance issues
	//if ((BWAPI::Broodwar->self()->supplyTotal() <= BWAPI::Broodwar->self()->supplyUsed() + 7)
	//	&& (BWAPI::Broodwar->self()->supplyUsed() <= (200 * 2))
	//	&& !(BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot)))
	//{
	//	queueTerranSupply(numSupply + 1);
	//	queueTerranBunkers(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1);
	//	cleanQueue();
	//	return;

	//}
	//else if (BWAPI::Broodwar->self()->supplyUsed() > (190 * 2))
	//{
	//	queueTerranTurrets(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Missile_Turret) + 2);
	//	// TODO tech, upgrade etc.
	//	// TODO - control supply while building units (ensure there is enough)
	//	//queueTerranBunkers(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1);
	//	return;
	//}

	//bool underConstruction = (BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Bunker));
	//if ((minerals > 300)
	//	&& (!underConstruction))
	//{
	//	queueTerranBunkers(std::min((BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1), 5));
	//}

	//if (numMarines < 20)
	//{
	//	queueTerranMarines(1.0);
	//	queueTerranMarines(1.0);
	//	queueTerranMarines(1.0);
	//}

	//if (frame < 10000)
	//{
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);
	//}
	//else if (frame < 14000)
	//{
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);
	//	queueTerranTanks(0.3);
	//}
	//else
	//{
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);
	//	queueTerranTanks(1.0);
	//}

	//if (minerals > 1000)
	//{
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);
	//	//queueTerranVultures(1.0);
	//	//queueTerranVultures(1.0);

	//	queueTerranFactories(numFactories + 1);
	//}

	//if (minerals > 1000
	//	&& gas > 500)
	//{
	//	queueTerranTanks(1.0);
	//}


	////queueTerranVultures(1.0);
	////queueTerranVultures(1.0);
	////queueTerranTanks(0.3);

	//if (numScvs < 48)
	//{
	//	queueTerranSCVs(1.0);
	//	queueTerranSCVs(1.0);
	//	queueTerranSCVs(1.0);
	//	queueTerranSCVs(1.0);
	//}

	//if (frame > 10000)
	//{
	//	//queueTechVultures();

	//	if (minerals > 250)
	//	{
	//		int factoriesWanted = std::min(numFactories + 1, 3);
	//		queueTerranFactories(factoriesWanted);
	//	}
	//	//queueTerranFactories(3);
	//}

	//if (frame > 14000)
	//{
	//	if (minerals > 250)
	//	{
	//		int factoriesWanted = std::min(numFactories + 1, 4);
	//		queueTerranFactories(factoriesWanted);
	//	}

	//	//queueTerranFactories(4);
	//}

	//if (minerals > 1500)
	//{
	//	queueTerranBarracks(std::min((numBarracks + 1), 5));
	//	queueTerranTurrets(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Missile_Turret) + 2);
	//	queueTerranMarines(1.0);
	//	queueTerranMarines(1.0);
	//	queueTerranMarinesUpgrades();
	//}

	//if (minerals > 300)
	//{
	//	//queueTerranBCUpgrades();
	//	queueTerranTankUpgrades();
	//}

	//if (frame > 10000)
	//{
	//	queueTechTanks();
	//}

	//int numScienceVessels = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Vessel);
	//int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	//if (frame > 10000
	//	&& numScienceVessels < 1)
	//{
	//	queueTerranScienceVessels(1 / std::max(numStarports, 1));	// One is enough
	//}

	//makeExpansion();

	//// TODO: if supply is higher than 110 -> go for battlecruisers 

	//if (isAirThreat())
	//{
	//	queueTerranGoliaths(0.5);
	//	queueTechGoliaths();
	//	queueTerranWraiths(1.0);
	//}

	//// Supply MUST be at the end (highest priority). Otherwise performance issues occur (units can't be build but are inserted before supply)	
	//int totalSupplyRequired = BWAPI::Broodwar->self()->supplyUsed() + getQueueSupply();
	//if (totalSupplyRequired >= BWAPI::Broodwar->self()->supplyTotal()
	//	&& (BWAPI::Broodwar->self()->supplyTotal() < (200*2)) )
	//{
	//	int supplyRequired = totalSupplyRequired - BWAPI::Broodwar->self()->supplyTotal();
	//	int supplyDepotsRequired = std::max(1, (int)std::ceil(supplyRequired / 8.0));

	//	supplyDepotsRequired = std::min(supplyDepotsRequired, 3);
	//	queueTerranSupply(numSupply + supplyDepotsRequired);
	//}
	//
	//

	//cleanQueue();


//--------------------------------------------------------------------------------------------------------------------


	//if (_lastInvoked + 240 > BWAPI::Broodwar->getFrameCount())
	//{
	//	return;
	//}
	//else
	//{
	//	_lastInvoked = BWAPI::Broodwar->getFrameCount();
	//}




	//// Tanks
	//int numScvs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	//int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);
	//int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	//int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);

	//int minerals = BWAPI::Broodwar->self()->minerals();
	//int gas = BWAPI::Broodwar->self()->gas();
	//int frame = BWAPI::Broodwar->getFrameCount();


	////if ((BWAPI::Broodwar->getFrameCount() % 120) != 0)
	////{
	////	return;
	////}

	//// To prevent performance issues
	//if ((BWAPI::Broodwar->self()->supplyTotal() <= BWAPI::Broodwar->self()->supplyUsed() + 7)
	//	&& (BWAPI::Broodwar->self()->supplyUsed() <= (200 * 2)))
	//{
	//	queueTerranSupply(numSupply + 3);
	//	queueTerranBunkers(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1);
	//	cleanQueue();
	//	return;

	//}
	//else if (BWAPI::Broodwar->self()->supplyUsed() > (190 * 2))
	//{
	//	queueTerranTurrets(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Missile_Turret) + 2);
	//	// TODO tech, upgrade etc.
	//	// TODO - control supply while building units (ensure there is enough)
	//	//queueTerranBunkers(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1);
	//	return;
	//}




	////// Test module
	////if (frame < 10000)
	////{
	////	
	////	queueTerranVultures(1.0);
	////	queueTerranMarines(1.0);
	////	queueTerranMarines(1.0);

	////	//cleanQueue();
	////	//return;
	////}
	////else if (frame < 14000)
	////{				
	////	queueTerranVultures(1.0);
	////	queueTerranTanks(0.5);
	////	queueTerranMarines(1.0);

	////	//cleanQueue();
	////	//return;
	////}
	////// eof test





	////bool underConstruction = (BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Factory) > 0) 
	////	|| (BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Bunker));
	//bool underConstruction = (BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Bunker));
	//if ((minerals > 300)
	//	&& (!underConstruction))
	//{
	//	queueTerranBunkers(std::min((BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1), 5));
	//}

	//if (minerals > 300)
	//{
	//	//queueTerranBCUpgrades();
	//	queueTerranTankUpgrades();
	//}

	//// causing crash
	////if (BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Yamato_Gun)
	////	&& BWAPI::Broodwar->self()->completedUnitCount((BWAPI::UnitTypes::Terran_Science_Facility)) > 0)
	////{
	////	queueTerranBCs(1.0);
	////}

	//if (numMarines < 20)
	//{
	//	queueTerranMarines(1.0);
	//	queueTerranMarines(1.0);
	//	queueTerranMarines(1.0);
	//}

	//if (frame < 10000)
	//{
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);
	//}
	//else if (frame < 14000)
	//{
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);
	//	queueTerranTanks(0.3);
	//}
	//else
	//{
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);
	//	queueTerranTanks(1.0);
	//}

	//if (minerals > 1000)
	//{
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);
	//	queueTerranVultures(1.0);

	//	queueTerranFactories(numFactories + 1);
	//}

	//if (minerals > 1000
	//	&& gas > 500)
	//{
	//	queueTerranTanks(1.0);
	//}


	////queueTerranVultures(1.0);
	////queueTerranVultures(1.0);
	////queueTerranTanks(0.3);

	//if (numScvs < 48)
	//{
	//	queueTerranSCVs(1.0);
	//	queueTerranSCVs(1.0);
	//	queueTerranSCVs(1.0);
	//	queueTerranSCVs(1.0);
	//}

	//if (frame > 10000)
	//{
	//	//queueTechVultures();

	//	int factoriesWanted = std::min(numFactories + 1, 3);
	//	queueTerranFactories(factoriesWanted);
	//	//queueTerranFactories(3);
	//}

	//if (frame > 14000)
	//{
	//	int factoriesWanted = std::min(numFactories + 1, 4);
	//	queueTerranFactories(factoriesWanted);

	//	//queueTerranFactories(4);
	//}

	//if (frame > 10000)
	//{
	//	queueTechTanks();
	//}

	//int numScienceVessels = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Vessel);
	//int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	//if (frame > 10000
	//	&& numScienceVessels < 1)
	//{
	//	queueTerranScienceVessels(1 / std::max(numStarports, 1));	// One is enough
	//}

	//makeExpansion();

	//// TODO: if supply is higher than 110 -> go for battlecruisers 

	//if (isAirThreat())
	//{
	//	queueTerranGoliaths(0.5);
	//	queueTechGoliaths();
	//	queueTerranWraiths(1.0);
	//}

	//// Supply MUST be at the end (highest priority). Otherwise performance issues occur (units can't be build but are inserted before supply)	
	//int totalSupplyRequired = BWAPI::Broodwar->self()->supplyUsed() + getQueueSupply();
	//if (totalSupplyRequired >= BWAPI::Broodwar->self()->supplyTotal())
	//{
	//	int supplyRequired = totalSupplyRequired - BWAPI::Broodwar->self()->supplyTotal();
	//	int supplyDepotsRequired = std::max(1, (int)std::ceil(supplyRequired / 8.0));
	//	queueTerranSupply(numSupply + supplyDepotsRequired);
	//}
	//
	//

	//cleanQueue();
}

void QueueConstructorExt::makeTerranWraithRush1PortQueue()
{
	if (_lastInvoked + 240 > BWAPI::Broodwar->getFrameCount())
	{
		return;
	}
	else
	{
		_lastInvoked = BWAPI::Broodwar->getFrameCount();
	}

	int numScvs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);
	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);

	int minerals = BWAPI::Broodwar->self()->minerals();
	int gas = BWAPI::Broodwar->self()->gas();
	int frame = BWAPI::Broodwar->getFrameCount();

	// To prevent performance issues
	if ((BWAPI::Broodwar->self()->supplyTotal() <= BWAPI::Broodwar->self()->supplyUsed() + 7)
		&& (BWAPI::Broodwar->self()->supplyUsed() <= (200 * 2)))
	{
		queueTerranSupply(numSupply + 3);
		queueTerranBunkers(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1);
		cleanQueue();
		return;

	}
	else if (BWAPI::Broodwar->self()->supplyUsed() > (190 * 2))
	{
		queueTerranTurrets(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Missile_Turret) + 2);
		queueTerranBCUpgrades();
		queueTerranWraithUpgrades();
		return;
	}

	bool underConstruction = (BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Bunker));
	if ((minerals > 300)
		&& (!underConstruction))
	{
		queueTerranBunkers(std::min((BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker) + 1), 5));
	}

	if ((minerals > 300)
		&& (frame > 10000))
	{
		queueTerranShipUpgrades();
	}

	if (frame > 12000)
	{
		queueTerranBCUpgrades();
	}

	if (numMarines < 20)
	{
		queueTerranBarracks(3);

		queueTerranMarines(1.0);
		queueTerranMarines(1.0);
		queueTerranMarines(1.0);
	}

	if (frame < 14000)
	{
		queueTerranWraiths(1.0);
		queueTerranStarports(2);
	}
	else
	{
		
		queueTerranWraiths(0.5);
		queueTerranBCs(0.5);
	}

	if (numScvs < 48)
	{
		queueTerranSCVs(1.0);
		queueTerranSCVs(1.0);
	}

	if (frame > 10000)
	{
		queueTerranWraithUpgrades();
		queueTerranStarports(3);
	}

	if (frame > 12000)
	{
		queueTerranStarports(4);
	}

	if (frame > 13000)
	{
		queueTechBattlecruisers();
	}

	int numScienceVessels = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Vessel);
	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	if (frame > 10000
		&& numScienceVessels < 1)
	{
		queueTerranScienceVessels(1 / std::max(numStarports, 1));	// One is enough
	}

	//makeExpansion();

	////if (isAirThreat())
	////{
	////	queueTerranWraiths(1.0);
	////}

	// Supply MUST be at the end (highest priority). Otherwise performance issues occur (units can't be build but are inserted before supply)	
	int totalSupplyRequired = BWAPI::Broodwar->self()->supplyUsed() + getQueueSupply();
	if (totalSupplyRequired >= BWAPI::Broodwar->self()->supplyTotal())
	{
		int supplyRequired = totalSupplyRequired - BWAPI::Broodwar->self()->supplyTotal();
		int supplyDepotsRequired = std::max(1, (int)std::ceil(supplyRequired / 8.0));
		queueTerranSupply(numSupply + supplyDepotsRequired);
	}

	cleanQueue();
}

void QueueConstructorExt::queueCommandCenters(int desiredNo)
{
	int numCommandCenters = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Command_Center);

	for (int i = numCommandCenters; i < desiredNo; i++)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Command_Center), true);
	}
}

void QueueConstructorExt::queueTerranMarines(double prodPercent)
{
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);

	int marinesWanted = std::max(1, (int)ceil(numBarracks * prodPercent));

	if (numBarracks < 1)
	{
		queueTerranBarracks(1);
	}
	else
	{
		for (int i = 0; i < marinesWanted; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Marine), true);
		}
	}
}

void QueueConstructorExt::queueTerranFirebats(double prodPercent)
{
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);
	int numAcademies = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy);

	int firebatsWanted = std::max(1, (int)ceil(numBarracks * prodPercent));

	if (numBarracks < 1)
	{
		queueTerranBarracks(1);
	}
	else if (numAcademies < 1)
	{
		queueTerranAcademies(1);
	}
	else
	{
		for (int i = 0; i < firebatsWanted; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Firebat), true);
		}
	}
}

void QueueConstructorExt::queueTerranVultures(double prodPercent)
{
	int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);

	int vulturesWanted = std::max(1, (int)ceil(numFactories * prodPercent));

	if (numFactories < 1)
	{
		queueTerranFactories(1);
	}
	else
	{
		for (int i = 0; i < vulturesWanted; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Vulture), true);
		}
	}
}

void QueueConstructorExt::queueTerranTanks(double prodPercent)
{
	int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);
	int numMachineShops = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Machine_Shop);

	int tanksWanted = std::max(1, (int)ceil( std::min(numFactories, numMachineShops) * prodPercent));

	if (numFactories < 1)
	{
		queueTerranFactories(1);
	}
	else if (numMachineShops < numFactories)
	{
		for (int i = numMachineShops; i < numFactories; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Machine_Shop), true);
		}
	}	
	else
	{
		for (int i = 0; i < tanksWanted; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode), true);
		}
	}
}

void QueueConstructorExt::queueTerranGoliaths(double prodPercent)
{
	int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);
	int numArmory = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Armory);

	int goliathsWanted = std::max(1, (int)ceil(numFactories * prodPercent));

	if (numFactories < 1)
	{
		queueTerranFactories(1);
	}
	else if (numArmory < 1)
	{
		BWAPI::Broodwar->printf("                                           DebExt: queue Armory - Goliaths may crash SC");
		queueTerranArmory(1);
	}
	else
	{
		//// This may crash the game -----------------
		for (int i = 0; i < goliathsWanted; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Goliath), true);
		}
	}
}

void QueueConstructorExt::queueTerranWraiths(double prodPercent)
{
	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	int wraithsWanted = std::max(1, (int)ceil(numStarports * prodPercent));

	if (numStarports < 1)
	{
		queueTerranStarports(1);
	}
	else
	{
		for (int i = 0; i < wraithsWanted; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Wraith), true);
		}
	}
}

void QueueConstructorExt::queueTerranSCVs(double prodPercent)
{
	int numCommandCenters = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Command_Center);

	int scvsWanted = std::max(1, (int)ceil(numCommandCenters * prodPercent));

	if (numCommandCenters > 0)
	{
		for (int i = 0; i < scvsWanted; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_SCV), true);
		}
	}

}
void QueueConstructorExt::queueTerranBCs(double prodPercent)
{
	int numBattlecruisers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Battlecruiser);
	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	int numControltowers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Control_Tower);
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);
	int numPhysicsLabs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Physics_Lab);

	int bcsWanted = std::max(1, (int)ceil(std::min(numStarports, numControltowers) * prodPercent));

	if (numStarports < 1)
	{
		queueTerranStarports(1);
	}
	else if (numControltowers < numStarports)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Control_Tower), true);
	}
	else if (numScienceFacilities < 1)
	{
		queueTerranScienceFacilities(1);
	}
	else if (numPhysicsLabs < 1)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Physics_Lab), true);
	}
	else
	{
		for (int i = 0; i < bcsWanted; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Battlecruiser), true);
		}
	}
}

void QueueConstructorExt::queueTerranScienceVessels(double prodPercent)
{
	int numScienceVessels = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Vessel);
	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);
	int numControltowers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Control_Tower);

	int scienceVesselsWanted = std::max(1, (int)ceil(std::min(numStarports, numControltowers) * prodPercent));

	if (numStarports < 1)
	{
		queueTerranStarports(1);
	}
	else if (numControltowers < numStarports)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Control_Tower), true);
	}
	else if (numScienceFacilities < 1)
	{
		queueTerranScienceFacilities(1);
	}
	else
	{
		for (int i = 0; i < scienceVesselsWanted; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Science_Vessel), true);
		}
	}
}

void QueueConstructorExt::queueTerranSupply(int desiredNo)
{
	int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);

	for (int i = numSupply; i < desiredNo; i++)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Supply_Depot), true);
	}
}

void QueueConstructorExt::queueTerranBunkers(int desiredNo)
{
	int numBunkers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Bunker);
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);

	if (numBarracks < 1)
	{
		queueTerranBarracks(1);
	}
	else
	{
		for (int i = numBunkers; i < desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Bunker), true);
		}
	}

}

void QueueConstructorExt::queueEngineeringBays(int desiredNo)
{
	int numEngineeringBays = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay);
	
	for (int i = numEngineeringBays; i < desiredNo; i++)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Engineering_Bay), true);
	}

}

void QueueConstructorExt::queueTerranTurrets(int desiredNo)
{
	int numTurrets = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Missile_Turret);
	int numEngineeringBays = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay);

	if (numEngineeringBays < 1)
	{
		queueEngineeringBays(1);
	}
	else
	{
		for (int i = numTurrets; i < desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Missile_Turret), true);
		}
	}


}

void QueueConstructorExt::queueTerranBarracks(int desiredNo)
{
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);

	for (int i = numBarracks; i < desiredNo; i++)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Barracks), true);
	}
}

void QueueConstructorExt::queueTerranAcademies(int desiredNo)
{
	int numAcademies = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy);
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);

	if (numBarracks < 1)
	{
		queueTerranBarracks(1);
	}
	else
	{
		for (int i = numAcademies; i < desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Academy), true);
		}
	}
}

void QueueConstructorExt::queueTerranFactories(int desiredNo)
{
	int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);
	int numMachineShops = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Machine_Shop);

	if (numBarracks < 1)
	{
		queueTerranBarracks(1);
	}
	if (numMachineShops < numFactories)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Machine_Shop), true);
	}
	else
	{
		for (int i = numFactories; i < desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Factory), true);
		}
	}

}

void QueueConstructorExt::queueTerranStarports(int desiredNo)
{
	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);

	if (numFactories < 1)
	{
		queueTerranFactories(1);
	}
	else
	{
		for (int i = numStarports; i < desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Starport), true);
		}
	}
}

void QueueConstructorExt::queueTerranScienceFacilities(int desiredNo)
{
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);
	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);

	if (numScienceFacilities >= desiredNo)
	{
		return;
	}

	if (numStarports < 1)
	{
		queueTerranStarports(1);
	}
	else
	{
		for (int i = numScienceFacilities; i < desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Science_Facility), true);
		}
	}
}

void QueueConstructorExt::queueTerranArmory(int desiredNo)
{
	int numArmory = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Armory);
	int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);

	if (numFactories < 1)
	{
		queueTerranFactories(1);
	}
	else
	{
		for (int i = numArmory; i < desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Armory), true);
		}
	}
}

void QueueConstructorExt::addControlTowers()
{
	BOOST_FOREACH(BWAPI::Unit* unit, BWAPI::Broodwar->self()->getUnits())
	{
		if (unit->getType() == BWAPI::UnitTypes::Terran_Starport
			&& unit->getAddon() == NULL)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Control_Tower), true);
		}
	}
}

void QueueConstructorExt::queueTerranBCUpgrades()
{
	queueTechBattlecruisers();
}

void QueueConstructorExt::queueTerranWraithUpgrades()
{
	int numArmory = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Armory);

	bool isCloakingFieldResearched = BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Cloaking_Field);
	bool isApolloReactorUpgraded = (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Apollo_Reactor)		// Do not upgrade this one
		>= BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Apollo_Reactor));

	int weaponLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Weapons);
	int armorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Plating);

	bool maxWeaponReached = (weaponLevel >= BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons));
	bool maxArmorReached = (armorLevel >= BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor));

	isApolloReactorUpgraded = true;		// to skip this upgrade
	bool allUpgradesCompleted = isCloakingFieldResearched && isApolloReactorUpgraded && maxWeaponReached && maxArmorReached;

	if (allUpgradesCompleted)
	{
		return;
	}

	if (numArmory < 1)
	{
		queueTerranArmory(1);
	}
	else
	{
		// Cloaking field first, then weapons and armor last
		if (!isCloakingFieldResearched
			&& !BWAPI::Broodwar->self()->isResearching(BWAPI::TechTypes::Cloaking_Field))
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::TechTypes::Cloaking_Field), true);
		}
		else if (weaponLevel <= armorLevel)
		{
			queueTerranAirWeapons();
		}
		else
		{
			queueTerranAirArmor();
		}
	}
}

void QueueConstructorExt::queueTerranTankUpgrades()
{
	int numFactiories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Factory);
	int numArmory = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Armory) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Armory);
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);

	if (numFactiories < 1)
	{
		queueTerranFactories(1);
	}
	else if (numArmory < 1)
	{
		queueTerranArmory(1);
	}
	else
	{
		if ((BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons))
			|| (BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Vehicle_Plating)))
		{
			return;
		}

		int weaponLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons);
		int armorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Vehicle_Plating);

		int maxWeaponLevel = BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons);
		int maxArmorLevel = BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Vehicle_Plating);

		if (weaponLevel < maxWeaponLevel)
		{
			if ((weaponLevel <= armorLevel))
			{
				if ( ((weaponLevel > 0) || (armorLevel > 0))
					&& (numScienceFacilities < 1 ))
				{
					queueTerranScienceFacilities(1);
				}
				else
				{
					_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons), true);
				}
			}
			else
			{
				_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Vehicle_Plating), true);
			}
		}
		else if (armorLevel < maxArmorLevel)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Vehicle_Plating), true);
		}
	}
}

void QueueConstructorExt::queueTerranMarinesUpgrades()
{
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);
	int numEnginneringBays = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay);
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);

	if (numBarracks < 1)
	{
		queueTerranBarracks(1);
	}
	else if (numEnginneringBays < 1)
	{
		queueEngineeringBays(1);
	}
	else
	{
		if ((BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Infantry_Weapons))
			|| (BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Infantry_Armor)))
		{
			return;
		}

		int weaponLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons);
		int armorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor);

		int maxWeaponLevel = BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons);
		int maxArmorLevel = BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor);

		if (weaponLevel < maxWeaponLevel)
		{
			if ((weaponLevel <= armorLevel))
			{
				if (((weaponLevel > 0) || (armorLevel > 0))
					&& (numScienceFacilities < 1))
				{
					queueTerranScienceFacilities(1);
				}
				else
				{
					_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Infantry_Weapons), true);
				}
			}
			else
			{
				_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Infantry_Armor), true);
			}
		}
		else if (armorLevel < maxArmorLevel)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Infantry_Armor), true);
		}
	}
}

void QueueConstructorExt::queueTerranGhostUpgrades()
{

}

void QueueConstructorExt::queueTerranBioUpgrades()
{

}

void QueueConstructorExt::queueTerranBioWeapons()
{
	int numEngineeringBays = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay);

	if (numEngineeringBays < 1)
	{
		queueEngineeringBays(1);
	}
	else
	{
		int weaponLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons);

		if (weaponLevel < BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons))
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Infantry_Weapons), true);
		}

	}
}

void QueueConstructorExt::queueTerranBioArmor()
{
	int numEngineeringBays = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay);

	if (numEngineeringBays < 1)
	{
		queueEngineeringBays(1);
	}
	else
	{
		int armorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor);

		if (armorLevel < BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor))
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Infantry_Armor), true);
		}

	}
}

void QueueConstructorExt::queueTerranShipUpgrades()
{
	int numFactiories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);
	int numArmory = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Armory);
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);

	if (numFactiories < 1)
	{
		queueTerranFactories(1);
	}
	else if (numArmory < 1)
	{
		queueTerranArmory(1);
	}
	else
	{
		if ((BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Ship_Weapons))
			|| (BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Terran_Ship_Plating)))
		{
			return;
		}

		int weaponLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Weapons);
		int armorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Plating);

		int maxWeaponLevel = BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Weapons);
		int maxArmorLevel = BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Plating);

		if (weaponLevel < maxWeaponLevel)
		{
			if ((weaponLevel <= armorLevel))
			{
				if (((weaponLevel > 0) || (armorLevel > 0))
					&& (numScienceFacilities < 1))
				{
					queueTerranScienceFacilities(1);
				}
				else
				{
					_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Ship_Weapons), true);
				}
			}
			else
			{
				_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Ship_Plating), true);
			}
		}
		else if (armorLevel < maxArmorLevel)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Ship_Plating), true);
		}
	}
}

void QueueConstructorExt::queueTerranAirWeapons()
{
	int numArmory = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Armory);

	if (numArmory < 1)
	{
		queueTerranArmory(1);
	}
	else
	{
		int armorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Weapons);

		if (armorLevel < BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Weapons))
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Ship_Weapons), true);
		}
	}
}

void QueueConstructorExt::queueTerranAirArmor()
{
	int numArmory = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Armory);

	if (numArmory < 1)
	{
		queueTerranArmory(1);
	}
	else
	{
		int armorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Plating);

		if (armorLevel < BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Plating))
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Terran_Ship_Plating), true);
		}
	}
}

void QueueConstructorExt::cleanQueue()
{
	BuildOrderQueue cleanedQueue;

	// One per game
	int numArmory = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Armory) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Armory);
	int numScienceFaciliteis = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);
	int numAcademies = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Academy);
	int numEngineeringBays = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay);

	for (int i = 0; i < _queue.size(); i++)
	{
		if (_queue[i].metaType.isUnit())
		{
			if (
				 (_queue[i].metaType.unitType == BWAPI::UnitTypes::Terran_Armory && numArmory > 1)
				|| (_queue[i].metaType.unitType == BWAPI::UnitTypes::Terran_Academy && numAcademies > 1)
				|| (_queue[i].metaType.unitType == BWAPI::UnitTypes::Terran_Engineering_Bay && numEngineeringBays > 1)
				|| (_queue[i].metaType.unitType == BWAPI::UnitTypes::Terran_Science_Facility && numScienceFaciliteis > 1))
			{
				continue;
			}
			else
			{
				cleanedQueue.queueAsHighestPriority(_queue[i].metaType, true);
			}
		}
		else
		{
			cleanedQueue.queueAsHighestPriority(_queue[i].metaType, true);
			//cleanedQueue.queueAsLowestPriority(_queue[i].metaType, true);
		}
	}

	_queue.clearAll();
	_queue = cleanedQueue;
}

void QueueConstructorExt::queueTechMarines()
{
	int numAcademies = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy);
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);

	if (numBarracks < 1)
	{
		// Ensure that there is at least one factory
		queueTerranBarracks(1);
	}
	else if (numAcademies < 1)
	{
		queueTerranAcademies(1);
	}
	else if (!(BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Stim_Packs))
		&& !(BWAPI::Broodwar->self()->isResearching(BWAPI::TechTypes::Stim_Packs)))
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::TechTypes::Spider_Mines), true);
	}
	else if ((BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells)
		< BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells))
		&& !(BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::U_238_Shells)))
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::U_238_Shells), true);
	}
}

void QueueConstructorExt::queueTechVultures()
{
	int numMachineShops = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Machine_Shop) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Machine_Shop);
	int numFactiories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Factory);

	if (numFactiories < 1)
	{
		// Ensure that there is at least one factory
		queueTerranFactories(1);
	}
	else if (numMachineShops < 1)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Machine_Shop), true);
	}
	else if ( !(BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Spider_Mines))
		&& !(BWAPI::Broodwar->self()->isResearching(BWAPI::TechTypes::Spider_Mines)) )
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::TechTypes::Spider_Mines), true);
	}
	else if ( (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Ion_Thrusters) 
				< BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Ion_Thrusters))
			&& !(BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Ion_Thrusters)))
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Ion_Thrusters), true);
	}
}

void QueueConstructorExt::queueTechTanks()
{
	int numMachineShops = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Machine_Shop) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Machine_Shop);
	int numFactiories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Factory);
	
	if (numFactiories < 1)
	{
		// Ensure that there is at least one factory
		queueTerranFactories(1);
	}
	if (numMachineShops < 1)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Machine_Shop), true);
	}
	else if ( !(BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Tank_Siege_Mode))
		&& !(BWAPI::Broodwar->self()->isResearching(BWAPI::TechTypes::Tank_Siege_Mode)))
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::TechTypes::Tank_Siege_Mode), true);
	}
}

void QueueConstructorExt::queueTechBattlecruisers()
{
	int numPhysicsLabs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Physics_Lab);
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);
	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);


	if (numFactories < 1)
	{
		queueTerranFactories(1);
	}
	else if (numStarports < 1)
	{
		queueTerranStarports(1);
	}
	else if (numScienceFacilities < 1)
	{
		queueTerranScienceFacilities(1);
	}
	else if (numPhysicsLabs < 1)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Physics_Lab), true);
	}
	else
	{
		if ((BWAPI::Broodwar->self()->isResearching(BWAPI::TechTypes::Yamato_Gun))
			|| (BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Colossus_Reactor)))
		{
			return;
		}
		else if (BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Yamato_Gun)
			&& (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Colossus_Reactor) ==
			BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Colossus_Reactor)))
		{
			return;
		}

		if (!BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Yamato_Gun))
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::TechTypes::Yamato_Gun), true);
		}
		else
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Colossus_Reactor), true);
		}
	}
}

void QueueConstructorExt::queueTechGoliaths()
{
	int numMachineShops = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Machine_Shop) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Machine_Shop);
	int numFactiories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory) + BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Factory);

	if (numFactiories < 1)
	{
		// Ensure that there is at least one factory
		queueTerranFactories(1);
	}
	else if (numMachineShops < 1)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Machine_Shop), true);
	}
	else if ((BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Charon_Boosters)
		< BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Charon_Boosters))
		&& !(BWAPI::Broodwar->self()->isUpgrading(BWAPI::UpgradeTypes::Charon_Boosters)))
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Charon_Boosters), true);
	}
}

bool QueueConstructorExt::isAirThreat()
{
	if (_airThreatExists)
	{
		return true;
	}

	int numEnemyStargates = BWAPI::Broodwar->enemy()->allUnitCount(BWAPI::UnitTypes::Protoss_Stargate);
	int numEnemyStarports = BWAPI::Broodwar->enemy()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	int numEnemySpires = BWAPI::Broodwar->enemy()->allUnitCount(BWAPI::UnitTypes::Zerg_Spire);


	if ((numEnemyStargates > 0)
		|| (numEnemyStarports > 0)
		|| (numEnemySpires > 0))
	{
		//return true;
		_airThreatExists = true;
	}
	
	BOOST_FOREACH(BWAPI::Unit* enemyUnit , BWAPI::Broodwar->enemy()->getUnits())
	{
		if (enemyUnit->getType().isFlyer())
		{
			//return true;
			_airThreatExists = true;
		}
	}

	return _airThreatExists;
}

bool QueueConstructorExt::isDetectorNeeded()
{
	if (_isDetectorNeeded)
	{
		return true;
	}

	BOOST_FOREACH(BWAPI::Unit* enemyUnit, BWAPI::Broodwar->enemy()->getUnits())
	{
		if (enemyUnit->getType().isCloakable()
			|| enemyUnit->isCloaked()
			|| enemyUnit->isBurrowed())
		{
			//return true;
			_isDetectorNeeded = true;
		}
	}

	return _isDetectorNeeded;
}

int QueueConstructorExt::getQueueSupply()
{
	int supply = 0;

	for (int i = 0; i < _queue.size(); i++)
	{
		if (_queue[i].metaType.isUnit())
		{
			supply += _queue[i].metaType.supplyRequired();
		}
	}

	return supply;
}

#include "QueueConstructorExt.h"
#include <math.h>


QueueConstructorExt::QueueConstructorExt()
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

	queueTerranMarines(0.5);
	queueTerranFirebats(0.5);
	queueTerranSCVs(1.0);
}

void QueueConstructorExt::queueTerranMarines(double prodPercent)
{
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);

	int marinesWanted = std::max(1, (int)ceil(numBarracks * prodPercent));

	if (numBarracks < 1)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Barracks), true);
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
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Barracks), true);
	}
	else if (numAcademies < 1)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Academy), true);
	}
	else
	{
		for (int i = 0; i < firebatsWanted; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Firebat), true);
		}
	}

}

void QueueConstructorExt::queueTerranWraiths(double prodPercent)
{
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);
	int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);
	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);

	int wraithsWanted = std::max(1, (int)ceil(numStarports * prodPercent));

	if (numBarracks < 1)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Barracks), true);
	}
	else if (numFactories < 1)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Factory), true);
	}
	else if (numStarports < 1)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Starport), true);
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

}

void QueueConstructorExt::queueTerranSupply(double prodPercent)
{

}

void QueueConstructorExt::queueTerranBunkers(double prodPercent)
{

}

void QueueConstructorExt::queueTerranTurrets(double prodPercent)
{

}

void QueueConstructorExt::queueTerranBCUpgrades(double prodPercent)
{

}

void QueueConstructorExt::queueTerranTankUpgrades(double prodPercent)
{

}

void QueueConstructorExt::queueTerranBioUpgrades(double prodPercent)
{

}

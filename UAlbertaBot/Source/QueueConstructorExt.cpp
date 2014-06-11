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

	
	//queueTerranMarines(0.5);
	//queueTerranFirebats(0.5);
	queueTerranWraiths(1.0);
	queueTerranMarinesUpgrades();
	queueTerranSCVs(1.0);
}

void QueueConstructorExt::queueCommandCenters(int desiredNo)
{
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Command_Center);

	for (int i = numBarracks; i <= desiredNo; i++)
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

}

void QueueConstructorExt::queueTerranSupply(int desiredNo)
{
	int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);

	for (int i = numSupply; i <= desiredNo; i++)
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
		for (int i = numBunkers; i <= desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Bunker), true);
		}
	}

}

void QueueConstructorExt::queueEngineeringBays(int desiredNo)
{
	int numEngineeringBays = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay);
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
		for (int i = numTurrets; i <= desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Missile_Turret), true);
		}
	}


}

void QueueConstructorExt::queueTerranBarracks(int desiredNo)
{
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);

	for (int i = numBarracks; i <= desiredNo; i++)
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
		for (int i = numAcademies; i <= desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Academy), true);
		}
	}
}

void QueueConstructorExt::queueTerranFactories(int desiredNo)
{
	int numFactories = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Factory);
	int numBarracks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Barracks);

	if (numBarracks < 1)
	{
		queueTerranBarracks(1);
	}
	else
	{
		for (int i = numFactories; i <= desiredNo; i++)
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
		for (int i = numStarports; i <= desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Starport), true);
		}
	}
}

void QueueConstructorExt::queueTerranScienceFacilities(int desiredNo)
{
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);
	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);

	if (numStarports < 1)
	{
		queueTerranStarports(1);
	}
	else
	{
		for (int i = numScienceFacilities; i <= desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Science_Facility), true);
		}
	}

}

void QueueConstructorExt::queueTerranBCUpgrades()
{

}

void QueueConstructorExt::queueTerranWraithUpgrades()
{

}

void QueueConstructorExt::queueTerranTankUpgrades()
{

}

void QueueConstructorExt::queueTerranMarinesUpgrades()
{
	int numAcademies = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy);

	bool isStimpackResearched = BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Stim_Packs);
	bool isShellsUpgraded = (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells)
		== BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells));

	if (numAcademies < 1)
	{
		queueTerranAcademies(1);
	}
	else
	{
		// Stimpacks and shells first
		if (!isStimpackResearched)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::TechTypes::Stim_Packs), true);
		}
		else if (!isShellsUpgraded)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::U_238_Shells), true);
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

}

void QueueConstructorExt::queueTerranBioArmor()
{

}

void QueueConstructorExt::queueTerranAirWeapons()
{

}

void QueueConstructorExt::queueTerranAirArmor()
{

}

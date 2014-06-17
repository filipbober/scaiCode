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

	//// Working well against default Protoss
	//queueTerranWraiths(1.0);
	//queueTerranWraithUpgrades();
	//queueTerranSCVs(1.0);
	//// ---











	int numSupply = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot);

	queueTerranWraiths(1.0);
	if (BWAPI::Broodwar->self()->supplyTotal() < BWAPI::Broodwar->self()->supplyUsed() + 5)
	{
		queueTerranSupply(numSupply + 1);
	}

	queueTerranMarines(0.7);
	queueTerranFirebats(0.3);
	queueTerranSCVs(1.0);



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
	int numBattlecruisers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Battlecruiser);
	int numStarports = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Starport);
	int numControltowers = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Control_Tower);
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);
	int numPhysicsLabs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Physics_Lab);

	int bcsWanted = std::max(1, (int)ceil(numBattlecruisers * prodPercent));

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
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility)
		+ BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);
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
		for (int i = numArmory; i <= desiredNo; i++)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Armory), true);
		}
	}
}

void QueueConstructorExt::queueTerranBCUpgrades()
{
	int numArmory = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Armory);
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);
	int numPhysicsLab = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Physics_Lab);

	bool isYamatoGunResearched = BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Yamato_Gun);
	bool isColossusReactorUpgraded = (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Colossus_Reactor)		// Do not upgrade this one
		>= BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Colossus_Reactor));

	int weaponLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Weapons);
	int armorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Ship_Plating);

	bool maxWeaponReached = (weaponLevel >= BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons));
	bool maxArmorReached = (armorLevel >= BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor));

	bool allUpgradesCompleted = isYamatoGunResearched && isColossusReactorUpgraded && maxWeaponReached && maxArmorReached;

	if (allUpgradesCompleted)
	{
		return;
	}

	//// First added to the queue, so the priority is lower than the Yamato Gun
	//if (numArmory < 1)
	//{
	//	queueTerranArmory(1);
	//}
	//else
	//{
	//	if ((weaponLevel <= armorLevel)
	//		&& !(maxWeaponReached))
	//	{
	//		queueTerranAirWeapons();
	//	}
	//	else if (!maxArmorReached)
	//	{
	//		queueTerranAirArmor();
	//	}
	//}

	// Queue Yamato Gun and Colossus Reactor
	if (numScienceFacilities < 1)
	{
		queueTerranScienceFacilities(1);
	}
	else if (numPhysicsLab < 1)
	{
		_queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Physics_Lab), true);
	}
	else
	{
		// Yamato Gun first
		if (!isYamatoGunResearched)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::TechTypes::Yamato_Gun), true);
		}
		else if (!isColossusReactorUpgraded)
		{
			_queue.queueAsHighestPriority(MetaType(BWAPI::UpgradeTypes::Colossus_Reactor), true);
		}


		//if (weaponLevel <= armorLevel)
		//{
		//	queueTerranAirWeapons();
		//}
		//else
		//{
		//	queueTerranAirArmor();
		//}
	}
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
		if (!isCloakingFieldResearched)
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

}

void QueueConstructorExt::queueTerranMarinesUpgrades()
{
	int numAcademies = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy);

	bool isStimpackResearched = BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Stim_Packs);
	bool isShellsUpgraded = (BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells)
		>= BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::U_238_Shells));

	int weaponLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons);
	int armorLevel = BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor);

	bool maxWeaponReached = (weaponLevel >= BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons));
	bool maxArmorReached = (armorLevel >= BWAPI::Broodwar->self()->getMaxUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor));

	bool allUpgradesCompleted = isStimpackResearched && isShellsUpgraded && maxWeaponReached && maxArmorReached;

	if (allUpgradesCompleted)
	{
		return;
	}

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
		else if (weaponLevel <= armorLevel)
		{
			queueTerranBioWeapons();
		}
		else
		{
			queueTerranBioArmor();
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
			if (_queue[i].metaType.unitType == BWAPI::UnitTypes::Terran_Armory
				|| (_queue[i].metaType.unitType == BWAPI::UnitTypes::Terran_Science_Facility && numArmory > 1)
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
	}

	_queue.clearAll();
	_queue = cleanedQueue;
}

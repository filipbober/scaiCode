#include "BattlecruiserManagerExt.h"
#include "Common.h"
#include "UnitManagerExt.h"


BattlecruiserManagerExt::BattlecruiserManagerExt()
{
}


BattlecruiserManagerExt::~BattlecruiserManagerExt()
{
}


void BattlecruiserManagerExt::executeMicro(const UnitVector & targets)
{
	const UnitVector & selectedUnits = getUnits();
	_noTurretTargetsNo = 0;

	// figure out targets
	UnitVector selectedUnitTargets;
	for (size_t i(0); i<targets.size(); i++)
	{
		// conditions for targeting
		if (targets[i]->isVisible())
		{
			selectedUnitTargets.push_back(targets[i]);

			if (!isTurret(targets[i]))
			{
				_noTurretTargetsNo++;
			}

		}
	}

	setAverageEnemyPosition(selectedUnitTargets);

	// For each unit
	BOOST_FOREACH(BWAPI::Unit * selectedUnit, selectedUnits)
	{
		// if the order is to attack or defend
		if (order.type == order.Attack || order.type == order.Defend)
		{
			// if there are targets
			if (!selectedUnitTargets.empty())
			{
				// find the best target for this unit
				BWAPI::Unit * target = getTarget(selectedUnit, selectedUnitTargets);

				// attack it				
				kiteTarget(selectedUnit, target);

			}
			// if there are no targets
			else
			{
				// if we're not near the order position
				if (selectedUnit->getDistance(order.position) > 100)
				{
					// move to it	

					// Border movement
					//BWAPI::Position movePosition;
					//if (order.type == SquadOrder::Attack)
					//{
					//	movePosition = UnitManagerExt::Instance().getMovePosition(selectedUnit);
					//}
					//else
					//{
					//	movePosition = order.position;
					//}
					// eof Border movement

					BWAPI::Position movePosition = order.position;
					smartAttackMove(selectedUnit, movePosition);
				}
			}
		}

		if (Options::Debug::DRAW_UALBERTABOT_DEBUG)
		{
			BWAPI::Broodwar->drawLineMap(selectedUnit->getPosition().x(), selectedUnit->getPosition().y(),
				selectedUnit->getTargetPosition().x(), selectedUnit->getTargetPosition().y(), Options::Debug::COLOR_LINE_TARGET);
		}
	}

}

void BattlecruiserManagerExt::executeAttack(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void BattlecruiserManagerExt::executeDefend(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void BattlecruiserManagerExt::executeAdvanceToPosition(BWAPI::Unit * terranMarine, UnitVector& marineTargets)
{

}

/// Returns target attack priority.
/// Returned value must be greater than 0
int BattlecruiserManagerExt::getAttackPriority(BWAPI::Unit * selectedUnit, BWAPI::Unit * target)
{
	BWAPI::UnitType selectedUnitType = selectedUnit->getType();
	BWAPI::UnitType targetType = target->getType();

	bool canAttackUs = targetType.airWeapon() != BWAPI::WeaponTypes::None;
	int selectedUnitWeaponRange = selectedUnitType.groundWeapon().maxRange();		// 160, Concussive
	int targetWeaponRange = targetType.groundWeapon().maxRange();


	// Detectors are top priority but Photon Cannons are too strong
	if (targetType == BWAPI::UnitTypes::Protoss_Carrier)
	{
		useYamatoGun(selectedUnit, target);
		return 99;
	}
	else if (targetType.isDetector()
		&& targetType != BWAPI::UnitTypes::Protoss_Photon_Cannon)
	{
		useYamatoGun(selectedUnit, target);
		return 100;
	}
	// Larvas are low priority targets
	else if (targetType == BWAPI::UnitTypes::Zerg_Larva
		|| targetType == BWAPI::UnitTypes::Protoss_Interceptor)
	{
		return 1;
	}
	else if (targetType == BWAPI::UnitTypes::Protoss_Pylon)
	{
		return 3;
	}
	else if ((targetType.isBuilding()) && !(targetType.canAttack()))
	{
		return 2;
	}
	// Workers are priority over ground units and buildings
	else if (targetType.isWorker())
	{
		return 4;
	}
	else if (isTurret(target))
	{
		// Attack tower if in its weapon range
		// Otherwise attack something else
		if (target->isInWeaponRange(selectedUnit))
		{
			return 5;
		}
		else
		{
			return 1;
		}
	}
	// Anti air units are top priority
	else if (canAttackUs)
	{
		return selectedUnitWeaponRange + 10;
	}
	else
	{
		return 1;
	}
}

BWAPI::Unit* BattlecruiserManagerExt::getTarget(BWAPI::Unit * selectedUnit, UnitVector & targets)
{
	int selectedUnitRange(selectedUnit->getType().groundWeapon().maxRange());

	int highestInRangePriority(0);
	int highestNotInRangePriority(0);
	int lowestInRangeHitPoints(10000);
	int lowestNotInRangeDistance(10000);

	BWAPI::Unit * inRangeTarget = NULL;
	BWAPI::Unit * notInRangeTarget = NULL;

	BOOST_FOREACH(BWAPI::Unit * unit, targets)
	{
		// Set range properely for flying units
		if (unit->getType().isFlyer())
		{
			selectedUnitRange = selectedUnit->getType().airWeapon().maxRange();
		}
		else
		{
			selectedUnitRange = selectedUnit->getType().groundWeapon().maxRange();
		}

		int priority = getAttackPriority(selectedUnit, unit);
		int distance = selectedUnit->getDistance(unit);
		int unitToughness = unit->getHitPoints() + unit->getShields();

		// if the unit is in range, update the target with the lowest hp
		if (selectedUnit->getDistance(unit) <= selectedUnitRange)
		{
			if (priority > highestInRangePriority ||
				(priority == highestInRangePriority && unitToughness < lowestInRangeHitPoints))
			{
				lowestInRangeHitPoints = unitToughness;
				highestInRangePriority = priority;
				inRangeTarget = unit;
			}
		}
		// otherwise it isn't in range so see if it's closest
		else
		{
			if (priority > highestNotInRangePriority ||
				(priority == highestNotInRangePriority && distance < lowestNotInRangeDistance))
			{
				lowestNotInRangeDistance = distance;
				highestNotInRangePriority = priority;
				notInRangeTarget = unit;
			}
		}
	}

	// if there is a highest priority unit in range, attack it first
	return (highestInRangePriority >= highestNotInRangePriority) ? inRangeTarget : notInRangeTarget;
}

void BattlecruiserManagerExt::kiteTarget(BWAPI::Unit * selectedUnit, BWAPI::Unit * target)
{
	// Battlecruiser do not kite
	int selectedUnitWeaponCooldown;
	double selectedUnitRange;
	if (target->getType().isFlyer())
	{
		selectedUnitRange = selectedUnit->getType().airWeapon().maxRange();
		selectedUnitWeaponCooldown = selectedUnit->getAirWeaponCooldown();
	}
	else
	{
		selectedUnitRange = selectedUnit->getType().groundWeapon().maxRange();
		selectedUnitWeaponCooldown = selectedUnit->getGroundWeaponCooldown();
	}

	double targetRange = getTargetWeaponRange(selectedUnit, target);

	if (target->getType() == BWAPI::UnitTypes::Protoss_Photon_Cannon
		&& (selectedUnit->getEnergy() > 150))
	{
		useYamatoGun(selectedUnit, target);
	} 
	else
	{
		smartAttackUnit(selectedUnit, target);
	}

	//useYamatoGun(selectedUnit, target);
	//smartAttackMove(selectedUnit, target->getPosition());

}

void BattlecruiserManagerExt::setAverageEnemyPosition(const UnitVector& targets)
{
	if (targets.empty())
	{
		return;
	}

	BWAPI::Position sumPos(0, 0);
	for (int i = 0; i < targets.size(); i++)
	{
		if (targets[i]->getPosition().isValid())
		{
			sumPos += targets[i]->getPosition();
		}
	}

	int xPos = (sumPos.x() / targets.size());
	int yPos = (sumPos.y() / targets.size());

	BWAPI::Position avgPos(xPos, yPos);
	_averageEnemyPosition = avgPos;

	if (!_averageEnemyPosition.isValid())
	{
		_averageEnemyPosition.makeValid();
	}
}


bool BattlecruiserManagerExt::isTurret(BWAPI::Unit* target)
{
	BWAPI::UnitType targetType = target->getType();

	if (targetType == BWAPI::UnitTypes::Protoss_Photon_Cannon
		|| targetType == BWAPI::UnitTypes::Terran_Missile_Turret
		|| targetType == BWAPI::UnitTypes::Zerg_Spore_Colony)
	{
		return true;
	}
	else
	{
		return false;
	}
}


int BattlecruiserManagerExt::getTargetWeaponRange(BWAPI::Unit* selectedUnit, BWAPI::Unit* target)
{
	if (selectedUnit->getType().isFlyer())
	{
		return target->getType().airWeapon().maxRange();
	}
	else
	{
		return target->getType().groundWeapon().maxRange();
	}
}

void BattlecruiserManagerExt::useYamatoGun(BWAPI::Unit* selectedUnit, BWAPI::Unit* target)
{
	if (BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Yamato_Gun)
		&& selectedUnit->getEnergy() > 150)
	{
		selectedUnit->useTech(BWAPI::TechTypes::Yamato_Gun, target);
	}
}
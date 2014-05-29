#include "WraithManagerExt.h"
#include "Common.h"
#include "UnitManagerExt.h"


WraithManagerExt::WraithManagerExt()
{
}


WraithManagerExt::~WraithManagerExt()
{
}

void WraithManagerExt::executeMicro(const UnitVector & targets)
{
	const UnitVector & selectedUnits = getUnits();


	// figure out targets
	UnitVector selectedUnitTargets;
	for (size_t i(0); i<targets.size(); i++)
	{
		// conditions for targeting
		if (targets[i]->isVisible())
		{
			selectedUnitTargets.push_back(targets[i]);
		}
	}

	setAverageEnemyPosition(selectedUnitTargets);

	// For each unit
	BOOST_FOREACH(BWAPI::Unit * selectedUnit, selectedUnits)
	{
		// Adjust cloak to the situation
		manageCloak(selectedUnit, selectedUnitTargets);

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
					BWAPI::Position movePosition = UnitManagerExt::Instance().getMovePosition(selectedUnit);
					// eof Border movement

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

void WraithManagerExt::executeAttack(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void WraithManagerExt::executeDefend(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void WraithManagerExt::executeAdvanceToPosition(BWAPI::Unit * terranMarine, UnitVector& marineTargets)
{

}

/// Returns target attack priority.
/// Returned value must be greater than 0
int WraithManagerExt::getAttackPriority(BWAPI::Unit * selectedUnit, BWAPI::Unit * target)
{
	BWAPI::UnitType selectedUnitType = selectedUnit->getType();
	BWAPI::UnitType targetType = target->getType();

	bool canAttackUs = targetType.airWeapon() != BWAPI::WeaponTypes::None;
	int selectedUnitWeaponRange = selectedUnitType.groundWeapon().maxRange();		// 160, Concussive
	int targetWeaponRange = targetType.groundWeapon().maxRange();


	// Larvas are low priority targets
	if (targetType == BWAPI::UnitTypes::Zerg_Larva
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
	else if (targetType == BWAPI::UnitTypes::Protoss_Photon_Cannon
		|| targetType == BWAPI::UnitTypes::Terran_Missile_Turret
		|| targetType == BWAPI::UnitTypes::Zerg_Spore_Colony)
	{
		return 5;
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

BWAPI::Unit* WraithManagerExt::getTarget(BWAPI::Unit * selectedUnit, UnitVector & targets)
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

void WraithManagerExt::kiteTarget(BWAPI::Unit * selectedUnit, BWAPI::Unit * target)
{
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

	double targetRange(target->getType().airWeapon().maxRange());

	// Determine whether the target can be kited (can attack us and have greater or rqual range)
	if ((target->getType().airWeapon() == BWAPI::WeaponTypes::None)
		|| (targetRange >= selectedUnitRange))
	{
		// if we can't kite it, there's no point to do so
		if (selectedUnitWeaponCooldown > 0)
		{						
				smartMove(selectedUnit, target->getPosition());
		}
		else
		{
			smartAttackUnit(selectedUnit, target);
		}


		return;
	}

	bool		kite(true);
	double		dist(selectedUnit->getDistance(target));
	double		speed(selectedUnit->getType().topSpeed());	

	int meleeRange = 15;

	// If we are going to be out of range (melee range added just to ensure we are still in range)
	// or if weapon is ready then attack
	if ((selectedUnitWeaponCooldown == 0)
		|| (dist >= (selectedUnitRange - meleeRange)))
	{
		smartAttackUnit(selectedUnit, target);
	}
	else
	{
		BWAPI::Position fleePosition(selectedUnit->getPosition() - _averageEnemyPosition + selectedUnit->getPosition());
		if (!fleePosition.isValid())
		{
			fleePosition.makeValid();
		}

		BWAPI::Broodwar->drawLineMap(selectedUnit->getPosition().x(), selectedUnit->getPosition().y(),
			fleePosition.x(), fleePosition.y(), BWAPI::Colors::Cyan);

		if (target->getType().canAttack())
		{
			smartMove(selectedUnit, fleePosition);
		}
		else
		{
			smartAttackMove(selectedUnit, target->getPosition());
		}
	}

}

void WraithManagerExt::setAverageEnemyPosition(const UnitVector& targets)
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

void WraithManagerExt::manageCloak(BWAPI::Unit * selectedUnit, UnitVector& targets)
{
	if (selectedUnit->isDetected()
		|| !(BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Cloaking_Field)))
	{
		selectedUnit->decloak();
		return;
	}

	BOOST_FOREACH(BWAPI::Unit* target, targets)
	{
		if (target->isInWeaponRange(selectedUnit))
		{
			selectedUnit->cloak();
			break;
		}
	}
}




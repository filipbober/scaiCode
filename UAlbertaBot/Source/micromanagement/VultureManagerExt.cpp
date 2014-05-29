#include "VultureManagerExt.h"
#include "Common.h"


VultureManagerExt::VultureManagerExt()
{
}


VultureManagerExt::~VultureManagerExt()
{
}

void VultureManagerExt::executeMicro(const UnitVector & targets)
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

	// For each Vulture
	BOOST_FOREACH(BWAPI::Unit * selectedUnit, selectedUnits)
	{
		// if the order is to attack or defend
		if (order.type == order.Attack || order.type == order.Defend)
		{
			// if there are targets
			if (!selectedUnitTargets.empty())
			{
				// find the best target for this Vulture
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
					smartAttackMove(selectedUnit, order.position);
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

void VultureManagerExt::executeAttack(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void VultureManagerExt::executeDefend(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void VultureManagerExt::executeAdvanceToPosition(BWAPI::Unit * terranMarine, UnitVector& marineTargets)
{

}

/// Returns target attack priority.
/// Returned value must be greater than 0
int VultureManagerExt::getAttackPriority(BWAPI::Unit * selectedUnit, BWAPI::Unit * target)
{
	BWAPI::UnitType selectedUnitType = selectedUnit->getType();
	BWAPI::UnitType targetType = target->getType();

	bool canAttackUs = targetType.groundWeapon() != BWAPI::WeaponTypes::None;
	int selectedUnitWeaponRange = selectedUnitType.groundWeapon().maxRange();		// 160, Concussive
	int targetWeaponRange = targetType.groundWeapon().maxRange();


	// Vulture cannot attack flyers
	if (targetType.isFlyer())
	{
		return 1;
	}
	// Faster than Vulture
	else if ((targetType.topSpeed() >= selectedUnitType.topSpeed())
		|| ((targetType == BWAPI::UnitTypes::Protoss_Zealot) 
			&& (BWAPI::Broodwar->enemy()->getUpgradeLevel(BWAPI::UpgradeTypes::Leg_Enhancements) > 0)))
	{
		return selectedUnitWeaponRange;		// return 160
	}
	// Slower than Vulture
	else
	{
		int priority = selectedUnitWeaponRange - targetWeaponRange;
		if (priority <= 0)
		{
			priority = 1;
		}

		return priority;
	}

}

BWAPI::Unit* VultureManagerExt::getTarget(BWAPI::Unit * selectedUnit, UnitVector & targets)
{
	int range(selectedUnit->getType().groundWeapon().maxRange());

	int highestInRangePriority(0);
	int highestNotInRangePriority(0);
	int lowestInRangeHitPoints(10000);
	int lowestNotInRangeDistance(10000);

	BWAPI::Unit * inRangeTarget = NULL;
	BWAPI::Unit * notInRangeTarget = NULL;

	BOOST_FOREACH(BWAPI::Unit * unit, targets)
	{
		int priority = getAttackPriority(selectedUnit, unit);
		int distance = selectedUnit->getDistance(unit);
		int unitToughness = unit->getHitPoints() + unit->getShields();

		// if the unit is in range, update the target with the lowest hp
		if (selectedUnit->getDistance(unit) <= range)
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

void VultureManagerExt::kiteTarget(BWAPI::Unit * selectedUnit, BWAPI::Unit * target)
{	
	double selectedUnitRange(selectedUnit->getType().groundWeapon().maxRange());
	double targetRange(target->getType().groundWeapon().maxRange());

	// determine whether the target can be kited
	if (selectedUnitRange <= target->getType().groundWeapon().maxRange())
	{
		// if we can't kite it, there's no point
		smartAttackUnit(selectedUnit, target);
		return;
	}

	bool		kite(true);
	double		dist(selectedUnit->getDistance(target));
	double		speed(selectedUnit->getType().topSpeed());

	// TODO: make the speed upgrade real. Currently ion thrusters speed is set to 24, which is not true.
	if ((BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Ion_Thrusters) > 0))
	{
		speed = 24;	
	}

	int vultureWeaponCooldown = selectedUnit->getGroundWeaponCooldown();
	int meleeRange = 15;

	// If we are going to be out of range (melee range added just to ensure we are still in range)
	// or if weapon is ready then attack
	if ((vultureWeaponCooldown == 0)
		|| (dist >= (selectedUnitRange - meleeRange)))
	{
		smartAttackUnit(selectedUnit, target);
		return;
	}
	else
	{
		// Run in the opposite direction to the enemy
		//BWAPI::Position fleePosition(vultureUnit->getPosition() - target->getPosition() + vultureUnit->getPosition());

		BWAPI::Position fleePosition(selectedUnit->getPosition() - _averageEnemyPosition + selectedUnit->getPosition());

		BWAPI::Broodwar->drawLineMap(selectedUnit->getPosition().x(), selectedUnit->getPosition().y(),
			fleePosition.x(), fleePosition.y(), BWAPI::Colors::Cyan);		

		putMine(selectedUnit);

		smartMove(selectedUnit, fleePosition);
	}

}

void VultureManagerExt::setAverageEnemyPosition(const UnitVector& targets)
{
	// If there are no valid targets, return opposite direction than start location,
	// so the Vulture will run straight to base
	if (targets.empty())
	{
		//_averageEnemyPosition.x = (BWAPI::Broodwar->self()->getStartLocation().x());
		//_averageEnemyPosition.y = (BWAPI::Broodwar->self()->getStartLocation().y());
		//BWAPI::Position avgPos((-BWAPI::Broodwar->self()->getStartLocation().x()), (-BWAPI::Broodwar->self()->getStartLocation().y()));
		//_averageEnemyPosition = avgPos;
		return;
	}

	BWAPI::Position sumPos(0, 0);
	for (int i = 0; i < targets.size(); i++)
	{
		if (targets[i]->getPosition().isValid()){
		sumPos += targets[i]->getPosition();
		}
	}

	int xPos = (sumPos.x() / targets.size());
	int yPos = (sumPos.y() / targets.size());

	BWAPI::Position avgPos(xPos, yPos);
	_averageEnemyPosition = avgPos;

	//_averageEnemyPosition = sumPos;

	if (!_averageEnemyPosition.isValid())
	{
		_averageEnemyPosition.makeValid();
	}
	//_averageEnemyPosition.x = (sumPos.x / targets.size());
	//_averageEnemyPosition.y = (sumPos.y / targets.size());
}

void VultureManagerExt::putMine(BWAPI::Unit * selectedUnit)
{
	if (BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Spider_Mines))
	{
		selectedUnit->useTech(BWAPI::TechTypes::Spider_Mines, selectedUnit->getPosition());
	}
	else
	{
		return;
	}
}
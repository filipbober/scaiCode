#include "MarineManagerExt.h"
#include "Common.h"
#include "StrategyManager.h"


MarineManagerExt::MarineManagerExt()
{
}


MarineManagerExt::~MarineManagerExt()
{
}

void MarineManagerExt::executeMicro(const UnitVector & targets)
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
		if ((selectedUnit->getType() == BWAPI::UnitTypes::Terran_Marine
			|| selectedUnit->getType() == BWAPI::UnitTypes::Terran_Firebat)
			&& !(selectedUnit->isLoaded())
			&& (hasBunkerSpace()))
		{

			goToBunker(selectedUnit);
			continue;
			
		}

		// if the order is to attack or defend
		if ((StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranWraithRush1Port)
			&& !isAttack())
		{
			executeTerranWraithRush1Port(selectedUnit, selectedUnitTargets);
		}
		else if ((StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranVulturesAndTanks)
			&& !isAttack())
		{
			executeTerranVulturesAndTanks(selectedUnit, selectedUnitTargets);
		}
		else if (order.type == order.Attack || order.type == order.Defend)
		{
			// if there are targets
			if (!selectedUnitTargets.empty())
			{
				// find the best target for this Marine
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

void MarineManagerExt::executeAttack(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void MarineManagerExt::executeDefend(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void MarineManagerExt::executeAdvanceToPosition(BWAPI::Unit * terranMarine, UnitVector& marineTargets)
{

}

/// Returns target attack priority.
/// Returned value must be greater than 0
int MarineManagerExt::getAttackPriority(BWAPI::Unit * selectedUnit, BWAPI::Unit * target)
{
	BWAPI::UnitType selectedUnitType = selectedUnit->getType();
	BWAPI::UnitType targetType = target->getType();

	bool canAttackUs = targetType.groundWeapon() != BWAPI::WeaponTypes::None;
	int selectedUnitWeaponRange = selectedUnitType.groundWeapon().maxRange();		// 160, Concussive
	int targetWeaponRange = targetType.groundWeapon().maxRange();


	// Larvas are low priority targets
	if (targetType == BWAPI::UnitTypes::Zerg_Larva
		|| targetType == BWAPI::UnitTypes::Protoss_Interceptor)
	{
		return 1;
	}
	else if ((targetType.isBuilding()) && !(targetType.canAttack()))
	{
		return 2;
	}
	else if (targetType == BWAPI::UnitTypes::Protoss_Photon_Cannon)
	{
		return selectedUnitWeaponRange + 5;
	}
	// Templars are extremely dangerous to bio units and should be eliminated asap.
	else if (targetType == BWAPI::UnitTypes::Protoss_High_Templar)
	{
		return selectedUnitWeaponRange + 10;
	}
	// Faster than Marine (without Stimpack)
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

BWAPI::Unit* MarineManagerExt::getTarget(BWAPI::Unit * selectedUnit, UnitVector & targets)
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

void MarineManagerExt::kiteTarget(BWAPI::Unit * selectedUnit, BWAPI::Unit * target)
{
	double selectedUnitRange(selectedUnit->getType().groundWeapon().maxRange());
	double targetRange(target->getType().groundWeapon().maxRange());

	BWAPI::UnitType targetType = target->getType();
	if (((targetType.canAttack())
		|| (targetType != BWAPI::UnitTypes::Protoss_High_Templar))
		&& (target->isInWeaponRange(selectedUnit)))
	{
		useStimpack(selectedUnit);
	}

	// determine whether the target can be kited
	if (selectedUnitRange <= targetRange)
	{
		// if we can't kite it, there's no point to do so
		smartAttackUnit(selectedUnit, target);
		return;
	}

	bool		kite(true);
	double		dist(selectedUnit->getDistance(target));
	double		speed(selectedUnit->getType().topSpeed());

	int selectedUnitWeaponCooldown = selectedUnit->getGroundWeaponCooldown();
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

void MarineManagerExt::setAverageEnemyPosition(const UnitVector& targets)
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

void MarineManagerExt::useStimpack(BWAPI::Unit * selectedUnit)
{
	if ((BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Stim_Packs))
		&& (selectedUnit->getHitPoints() > 30)
		&& (selectedUnit->getStimTimer() == 0))
	{
		selectedUnit->useTech(BWAPI::TechTypes::Stim_Packs);
	}
	else
	{
		return;
	}
}

void MarineManagerExt::goToBunker(BWAPI::Unit * selectedUnit)
{
	BOOST_FOREACH(BWAPI::Unit* unit, BWAPI::Broodwar->self()->getUnits())
	{

		if ((unit->getType() == BWAPI::UnitTypes::Terran_Bunker)
			&& (unit->getType().spaceProvided() > 1)
			&& (unit->getLoadedUnits().size() < 4)
			)
		{
			selectedUnit->load(unit);			
			break;
		}
	}
}

bool MarineManagerExt::hasBunkerSpace()
{
	BOOST_FOREACH(BWAPI::Unit* unit, BWAPI::Broodwar->self()->getUnits())
	{

		if ((unit->getType() == BWAPI::UnitTypes::Terran_Bunker)
			&& (unit->getType().spaceProvided() > 1)
			&& (unit->getLoadedUnits().size() < 4)
			)
		{
			return true;
		}
	}
	return false;
}

bool MarineManagerExt::isAttack()
{
	// Marines should only defend if Wraith Rush 1 Port is the current strategy
	if ((StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranWraithRush1Port))
	{
		return false;
	}
	else if ((StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranVulturesAndTanks))
	{
		if (BWAPI::Broodwar->getFrameCount() < 12000)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}
}

void MarineManagerExt::executeTerranWraithRush1Port(BWAPI::Unit * selectedUnit, UnitVector& selectedUnitTargets)
{
	if (order.type == order.Attack || order.type == order.Defend)
	{
		if (!selectedUnitTargets.empty())
		{
			BWAPI::Unit * target = getTarget(selectedUnit, selectedUnitTargets);

			if (selectedUnit->getDistance(target) < 300)
			{
				kiteTarget(selectedUnit, target);
			}
			else if (order.position.getDistance(selectedUnit->getPosition()) < 500)
			{
				smartAttackMove(selectedUnit, order.position);
			}
		}
	}
}

void MarineManagerExt::executeTerranVulturesAndTanks(BWAPI::Unit * selectedUnit, UnitVector& selectedUnitTargets)
{
	if (order.type == order.Attack || order.type == order.Defend)
	{
		if (!selectedUnitTargets.empty())
		{
			BWAPI::Unit * target = getTarget(selectedUnit, selectedUnitTargets);

			if (selectedUnit->getDistance(target) < 300)
			{
				kiteTarget(selectedUnit, target);
			}
			else if (order.position.getDistance(selectedUnit->getPosition()) < 500)
			{
				smartAttackMove(selectedUnit, order.position);
			}
		}
	}
}

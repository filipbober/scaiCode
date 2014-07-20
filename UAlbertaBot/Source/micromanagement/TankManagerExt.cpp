#include "TankManagerExt.h"
#include "Common.h"
#include "StrategyManager.h"
#include "UnitManagerExt.h"


TankManagerExt::TankManagerExt()
{
	_siegeModeFrame = 96;
}


TankManagerExt::~TankManagerExt()
{
}

void TankManagerExt::executeMicro(const UnitVector & targets)
{
	const UnitVector & selectedUnits = getUnits();


	// figure out targets
	UnitVector selectedUnitTargets;
	UnitVector nearbyTargets;
	for (size_t i(0); i<targets.size(); i++)
	{
		// conditions for targeting
		if (targets[i]->isVisible()
			&& !targets[i]->getType().isFlyer())
		{
			selectedUnitTargets.push_back(targets[i]);
		}
	}

	//setAverageEnemyPosition(selectedUnitTargets);

	// For each unit
	BOOST_FOREACH(BWAPI::Unit * selectedUnit, selectedUnits)
	{
		// Avg enemy pos
		for (size_t i(0); i<targets.size(); i++)
		{
			// conditions for targeting
			if (targets[i]->isVisible()
				&& !targets[i]->getType().isFlyer())
			{
				if (targets[i]->getDistance(selectedUnit) < 500)
				{
					nearbyTargets.push_back(targets[i]);
				}
			}
		}

		if (!nearbyTargets.empty())
		{
			setAverageEnemyPosition(nearbyTargets);
		}
		else
		{
			setAverageEnemyPosition(selectedUnitTargets);
		}

		// eof avg eveeny pos
		// if the order is to attack or defend
		if ((StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranWraithRush1Port)
			&& !isAttack())
		{
			executeTerranWraithRush1Port(selectedUnit, selectedUnitTargets);
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
				if (selectedUnit->getDistance(order.position) > 150)
				{
					// move to it
					siegeModeOff(selectedUnit);
					smartAttackMove(selectedUnit, order.position);
				}
				else
				{
					siegeModeOn(selectedUnit);
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

void TankManagerExt::executeAttack(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void TankManagerExt::executeDefend(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void TankManagerExt::executeAdvanceToPosition(BWAPI::Unit * terranMarine, UnitVector& marineTargets)
{

}

/// Returns target attack priority.
/// Returned value must be greater than 0
int TankManagerExt::getAttackPriority(BWAPI::Unit * selectedUnit, BWAPI::Unit * target)
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
	else if (targetType.isFlyer())
	{
		return 1;
	}
	else
	{
		return  std::max((1000 - selectedUnit->getDistance(target)), 1);
	}
}

BWAPI::Unit* TankManagerExt::getTarget(BWAPI::Unit * selectedUnit, UnitVector & targets)
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

void TankManagerExt::kiteTarget(BWAPI::Unit * selectedUnit, BWAPI::Unit * target)
{
	double selectedUnitRange(selectedUnit->getType().groundWeapon().maxRange());
	double targetRange(target->getType().groundWeapon().maxRange());

	double dist = closestEnemyDist(selectedUnit);

	int selectedUnitWeaponCooldown = selectedUnit->getGroundWeaponCooldown();
	
	int meleeRange = 15;

	int tankModeMaxRange = BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode.groundWeapon().maxRange();
	int siegeModeMinRange = BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode.groundWeapon().minRange();
	int siegeModeMaxRange = BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode.groundWeapon().maxRange();

	bool isSiegeModeOn = selectedUnit->isSieged();
	bool isTargetApproaching = !target->getType().isBuilding() && !target->getType().isFlyer() && !target->getType().isWorker();

	int keepDistance = 200;

	// If an enemy if farther than normal mode range but within siege mode + 10 (if not building)

	// If target is farther than normal mode range
	if (dist > tankModeMaxRange)
	{
		// If target is farther than siege mode range but is approaching
		if (((dist <= (siegeModeMaxRange + 50))
			&& isTargetApproaching)
			|| (target->getDistance(selectedUnit) <= siegeModeMaxRange))
		{
			siegeModeOn(selectedUnit);
		}
		// Target too far away
		else
		{
			siegeModeOff(selectedUnit);
		}
	}
	else if (dist < siegeModeMinRange)
	{
		siegeModeOff(selectedUnit);
	}


	// If an order was issued this frame (like siege off), return
	if (selectedUnit->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount())
	{
		return;
	}
	
	if ((dist > keepDistance)
		|| (selectedUnitWeaponCooldown == 0))
	{
		smartAttackUnit(selectedUnit, target);
	}
	else if (!isSiegeModeOn)
	{
		BWAPI::Position fleePosition(selectedUnit->getPosition() - _averageEnemyPosition + selectedUnit->getPosition());
		if (!fleePosition.isValid())
		{
			fleePosition.makeValid();
		}

		BWAPI::Broodwar->drawLineMap(selectedUnit->getPosition().x(), selectedUnit->getPosition().y(),
			fleePosition.x(), fleePosition.y(), BWAPI::Colors::Cyan);

		//fleeOrMine(selectedUnit, fleePosition);
		smartMove(selectedUnit, fleePosition);
	}











	

	//// If we are going to be out of range (melee range added just to ensure we are still in range)
	//// or if weapon is ready then attack
	//int keepDistance = 100;
	//if ((selectedUnitWeaponCooldown == 0)
	//	&& (dist > keepDistance)
	//	&& target->getDistance(selectedUnit) > keepDistance)
	//{
	//	//attackOrMine(selectedUnit, target);
	//	smartAttackUnit(selectedUnit, target);
	//}
	//else
	//{
	//	BWAPI::Position fleePosition(selectedUnit->getPosition() - _averageEnemyPosition + selectedUnit->getPosition());
	//	if (!fleePosition.isValid())
	//	{
	//		fleePosition.makeValid();
	//	}

	//	BWAPI::Broodwar->drawLineMap(selectedUnit->getPosition().x(), selectedUnit->getPosition().y(),
	//		fleePosition.x(), fleePosition.y(), BWAPI::Colors::Cyan);

	//	//fleeOrMine(selectedUnit, fleePosition);
	//	smartMove(selectedUnit, fleePosition);
	//}

}

void TankManagerExt::setAverageEnemyPosition(const UnitVector& targets)
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

bool TankManagerExt::isAttack()
{
	// Tanks attack no matter what the strategy is
	if ((StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranWraithRush1Port))
	{
		return true;
	}
	else
	{
		return true;
	}
}

void TankManagerExt::executeTerranWraithRush1Port(BWAPI::Unit * selectedUnit, UnitVector& selectedUnitTargets)
{
	// Code below is used for units (like marines) if they are not supposed to attack and should defend the base only
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

double TankManagerExt::closestEnemyDist(BWAPI::Unit* selectedUnit)
{
	double dist = 1000;

	BOOST_FOREACH(BWAPI::Unit* enemyUnit, BWAPI::Broodwar->enemy()->getUnits())
	{
		int unitDist = enemyUnit->getDistance(selectedUnit);

		if ((unitDist <= dist)
			&& enemyUnit->getType().canAttack()
			&& enemyUnit->isVisible()
			&& !enemyUnit->getType().isFlyer() // ignore Flyers - Vultures cant attack them anyway
			&& !enemyUnit->isCloaked())		
		{
			dist = unitDist;
		}
	}

	return dist;
}

void TankManagerExt::siegeModeOn(BWAPI::Unit* selectedUnit)
{
	if (!selectedUnit->isSieged()
		&& BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Tank_Siege_Mode))
	{
		selectedUnit->siege();
	}
}

void TankManagerExt::siegeModeOff(BWAPI::Unit* selectedUnit)
{
	if (selectedUnit->isSieged()
		&& BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Tank_Siege_Mode))
	{
		selectedUnit->unsiege();
	}
}

#include "VultureManagerExt.h"
#include "Common.h"
#include "StrategyManager.h"
#include "UnitManagerExt.h"


VultureManagerExt::VultureManagerExt()
{
	_putMineFrame = 96;
}


VultureManagerExt::~VultureManagerExt()
{
}

void VultureManagerExt::executeMicro(const UnitVector & targets)
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
	else if (targetType == BWAPI::UnitTypes::Protoss_High_Templar
		|| targetType == BWAPI::UnitTypes::Terran_Medic)
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
	// If mine is being put or we have issued a command this frame, return
	if (BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Spider_Mines)
		&& (UnitManagerExt::Instance().isPuttingMine(selectedUnit)
		|| selectedUnit->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount()))
	{
		return;
	}

	double selectedUnitRange(selectedUnit->getType().groundWeapon().maxRange());
	double targetRange(target->getType().groundWeapon().maxRange());	

	double dist = closestEnemyDist(selectedUnit);

	int selectedUnitWeaponCooldown = selectedUnit->getGroundWeaponCooldown();

	// If we are going to be out of range (melee range added just to ensure we are still in range)
	// or if weapon is ready then attack
	int keepDistance = 100;
	if (target->getType().isBuilding()
		&& !target->getType().canAttack())
	{
		attackOrMine(selectedUnit, target);
	}	
	if ((selectedUnitWeaponCooldown == 0)
		&& (dist > keepDistance)
		&& (target->getDistance(selectedUnit) > keepDistance))
	{				
		attackOrMine(selectedUnit, target);		
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

		fleeOrMine(selectedUnit, fleePosition);		
	}

}

void VultureManagerExt::setAverageEnemyPosition(const UnitVector& targets)
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

bool VultureManagerExt::isAttack()
{
	// Vultures attack no matter what strategy is
	if ((StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranWraithRush1Port))
	{
		return true;
	}
	else
	{
		return true;
	}
}

void VultureManagerExt::executeTerranWraithRush1Port(BWAPI::Unit * selectedUnit, UnitVector& selectedUnitTargets)
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


void VultureManagerExt::putMine(BWAPI::Unit * selectedUnit, BWAPI::Unit* target)
{
	if ((BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Spider_Mines)
		&& (selectedUnit->getSpiderMineCount() > 0)))
	{
		

		int proximity = 18;
		BWAPI::Position minePosition = getMinePosition(selectedUnit, target, proximity);

		bool isValid = selectedUnit->useTech(BWAPI::TechTypes::Spider_Mines, minePosition);
	}
}

void VultureManagerExt::putMine(BWAPI::Unit * selectedUnit, BWAPI::Position targetPosition)
{
	if ((BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Spider_Mines)
		&& (selectedUnit->getSpiderMineCount() > 0)))
	{
		bool isValid = selectedUnit->useTech(BWAPI::TechTypes::Spider_Mines, targetPosition);
	}
}

BWAPI::Position VultureManagerExt::getMinePosition(BWAPI::Unit* selectedUnit, BWAPI::Unit* target, int proximity)
{
	int posX = selectedUnit->getPosition().x();
	int posY = selectedUnit->getPosition().y();

	BWAPI::Position selectedUnitPosition = selectedUnit->getPosition();
	BWAPI::Position targetPosition = target->getPosition();

	// Set X
	if (selectedUnitPosition.x() < targetPosition.x())
	{
		posX -= proximity;
	}
	else if (selectedUnitPosition.x() > targetPosition.x())
	{
		posX += proximity;
	}
	else
	{
		posX = selectedUnit->getPosition().x();
	}

	// Set Y
	if (selectedUnitPosition.y() < targetPosition.y())
	{
		posY -= proximity;
	}
	else if (selectedUnitPosition.y() > targetPosition.y())
	{
		posY += proximity;
	}
	else
	{
		posY = selectedUnit->getPosition().y();
	}

	BWAPI::Position minePos = BWAPI::Position(posX, posY);
	if (!minePos.isValid())
	{
		minePos.makeValid();
	}

	return minePos;
}

void VultureManagerExt::attackOrMine(BWAPI::Unit* selectedUnit, BWAPI::Unit* target)
{
	int minDistToTarget = 200;

	// Put mine
	if ((selectedUnit->getSpiderMineCount() > 0)
		&& (selectedUnit->getDistance(target) <= 200)
		&& (BWAPI::Broodwar->getFrameCount() % _putMineFrame ==  0
		&& (selectedUnit->getSpellCooldown() == 0))
		&& !isMineProximity(selectedUnit))
	{
		BWAPI::Broodwar->printf("                                           DebExt: Vulture attackOrMine: putMine");
		UnitManagerExt::Instance().putMineFlagOn(selectedUnit);
		putMine(selectedUnit, target);
		return;
	}
	else
	{
		smartAttackUnit(selectedUnit, target);
		return;
	}
}

void VultureManagerExt::fleeOrMine(BWAPI::Unit * selectedUnit, BWAPI::Position fleePosition)
{
	if ((selectedUnit->getSpiderMineCount() > 0)
		&& (selectedUnit->getSpellCooldown() == 0)
		&& (BWAPI::Broodwar->getFrameCount() % _putMineFrame == 0))
	{
		BWAPI::Broodwar->printf("                                           DebExt: Vulture fleeOrMine: putMine");
		UnitManagerExt::Instance().putMineFlagOn(selectedUnit);
		putMine(selectedUnit, selectedUnit->getPosition());
	}
	else
	{
		//BWAPI::Broodwar->printf("                                           DebExt: Vulture fleeOrMine: smartMove");
		smartMove(selectedUnit, fleePosition);
	}
}

bool VultureManagerExt::isMineProximity(BWAPI::Unit* selectedUnit)
{
	int proximity = 100;

	BOOST_FOREACH(BWAPI::Unit* mine, BWAPI::Broodwar->self()->getUnits())
	{
		if (mine->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine)
		{
			if (mine->getDistance(selectedUnit) <= proximity)
			{
				return true;
			}
		}
	}

	return false;
}

double VultureManagerExt::closestEnemyDist(BWAPI::Unit* selectedUnit)
{
	double dist = 1000;

	BOOST_FOREACH(BWAPI::Unit* enemyUnit, BWAPI::Broodwar->enemy()->getUnits())
	{
		int unitDist = enemyUnit->getDistance(selectedUnit);

		if ((unitDist <= dist)
			&& enemyUnit->getType().canAttack()
			&& enemyUnit->isVisible()
			&& !enemyUnit->getType().isFlyer())		// ignore Flyers - Vultures cant attack them anyway
		{
			dist = unitDist;
		}
	}

	return dist;
}

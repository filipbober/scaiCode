#include "WraithManagerExt.h"
#include "Common.h"
#include "UnitManagerExt.h"
#include "StrategyManager.h"


WraithManagerExt::WraithManagerExt()
{
}


WraithManagerExt::~WraithManagerExt()
{
}

void WraithManagerExt::executeMicro(const UnitVector & targets)
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
					BWAPI::Position movePosition;
					if (order.type == SquadOrder::Attack
						&& (StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranWraithRush1Port))
					{
						movePosition = UnitManagerExt::Instance().getMovePosition(selectedUnit);
					}
					else
					{
						movePosition = order.position;
					}
					// eof Border movement
					if (!movePosition.isValid())
					{
						movePosition.makeValid();
					}

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


	// Detectors are top priority but Photon Cannons are too strong
	if (targetType == BWAPI::UnitTypes::Protoss_Carrier ||
		targetType == BWAPI::UnitTypes::Terran_Battlecruiser)
	{
		return selectedUnitWeaponRange + 20;
	}
	else if (targetType.isFlyer())
	{
		return selectedUnitWeaponRange + 10;
	}
	else if (targetType.isDetector()
		&& targetType != BWAPI::UnitTypes::Protoss_Photon_Cannon)
	{
		return selectedUnitWeaponRange + 5;
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
	else if (targetType == BWAPI::UnitTypes::Terran_Medic
		|| targetType == BWAPI::UnitTypes::Protoss_High_Templar
		|| targetType == BWAPI::UnitTypes::Protoss_Dark_Templar)
	{
		return selectedUnitWeaponRange + 15;
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
	
	double targetRange = getTargetWeaponRange(selectedUnit, target);

	// Avoid

	// Determine whether the target can be kited (can attack us and have greater or equal range)
	if ((target->getType().airWeapon() == BWAPI::WeaponTypes::None)
		|| (targetRange >= selectedUnitRange))
	{
		// If target is a turret and there are other viable targets then
		// move closer but stay out of the turret range
		int numWraiths = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Wraith);
		if (isTurret(target)
			&& (_noTurretTargetsNo > 0)
			&& numWraiths < 3)
		{
			BWAPI::Position safePos = getSafeTurretPosition(selectedUnit, target, 30);
			if (!safePos.isValid())
			{
				safePos.makeValid();
			}

			smartMove(selectedUnit, safePos);
		}
		// if we can't kite it, there's no point to do so
		else if (selectedUnitWeaponCooldown > 0)
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

double WraithManagerExt::closestEnemyDist(BWAPI::Unit* selectedUnit)
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

void WraithManagerExt::manageCloak(BWAPI::Unit * selectedUnit, UnitVector& targets)
{
	if (!BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Cloaking_Field))
	{
		return;
	}
	else
	{
		double dist = closestEnemyDist(selectedUnit);
		int energy = selectedUnit->getEnergy();

		if (selectedUnit->isCloaked()
			&& selectedUnit->isDetected()
			&& energy > 40)
		{
			BWAPI::Broodwar->printf("                                           DebExt: Wraith decloak");
			selectedUnit->decloak();
		}
		else if ((dist < 380)
			&& energy > 50)
		{
			selectedUnit->cloak();
		}
	}

	//if ((BWAPI::Broodwar->getFrameCount() % 240 == 0)
	//	&& (selectedUnit->isDetected())
	//	&& (selectedUnit->getEnergy() > 40)
	//	&& !(selectedUnit->isCloaked()))
	//{
	//	BWAPI::Broodwar->printf("                                           DebExt: Wraith decloak");
	//	selectedUnit->decloak();
	//	return;
	//}

	//BOOST_FOREACH(BWAPI::Unit* target, targets)
	//{
	//	if (BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Cloaking_Field)
	//		&& target->isInWeaponRange(selectedUnit)
	//		&& !selectedUnit->isCloaked()
	//		&& selectedUnit->getEnergy() > 50)			
	//	{
	//		BWAPI::Broodwar->printf("                                           DebExt: Wraith cloak");
	//		selectedUnit->cloak();
	//		break;
	//	}
	//}
}

bool WraithManagerExt::isInTurretRange(BWAPI::Position position, UnitVector & targets, BWAPI::Unit* selectedUnit)
{
	BWAPI::UnitType targetType;

	if (targets.empty())
	{
		return false;
	}

	BOOST_FOREACH(BWAPI::Unit* target, targets)
	{
		targetType = target->getType();

		// If target is a turret
		if (isTurret(target))
		{
			// If our position is in turret range
			if (getTargetWeaponRange(selectedUnit, target) >= target->getDistance(position))
			{
				return true;
			}
		}
	}
	
	return false;
}

bool WraithManagerExt::isTurret(BWAPI::Unit* target)
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

BWAPI::Position WraithManagerExt::getSafeTurretPosition(BWAPI::Unit* selectedUnit, BWAPI::Unit* target, int proximity)
{
	int safeX = target->getPosition().x();
	int safeY = target->getPosition().y();

	BWAPI::Position selectedUnitPosition = selectedUnit->getPosition();
	BWAPI::Position targetPosition = target->getPosition();
	int targetRange = getTargetWeaponRange(selectedUnit, target);

	// Set X
	if (selectedUnitPosition.x() < targetPosition.x())
	{
		safeX -= targetRange;
	}	
	else if (selectedUnitPosition.x() > targetPosition.x())
	{
		safeX += targetRange;
	}
	else
	{
		safeX = target->getPosition().x();
	}

	// Set Y
	if (selectedUnitPosition.y() < targetPosition.y())
	{
		safeY -= targetRange;
	}
	else if (selectedUnitPosition.y() > targetPosition.y())
	{
		safeY += targetRange;
	}
	else
	{
		safeY = target->getPosition().y();
	}

	BWAPI::Position safePos = BWAPI::Position(safeX, safeY);
	if (!safePos.isValid())
	{
		safePos.makeValid();
	}

	return safePos;
}

int WraithManagerExt::getTargetWeaponRange(BWAPI::Unit* selectedUnit, BWAPI::Unit* target)
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




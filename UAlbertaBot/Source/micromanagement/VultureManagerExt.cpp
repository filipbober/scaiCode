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
	const UnitVector & vultureUnits = getUnits();

	// figure out targets
	UnitVector vultureUnitTargets;
	for (size_t i(0); i<targets.size(); i++)
	{
		// conditions for targeting
		if (targets[i]->isVisible())
		{
			vultureUnitTargets.push_back(targets[i]);
		}
	}

	setAverageEnemyPosition(vultureUnitTargets);


	// For each Vulture
	BOOST_FOREACH(BWAPI::Unit * vultureUnit, vultureUnits)
	{
		// if the order is to attack or defend
		if (order.type == order.Attack || order.type == order.Defend)
		{
			// if there are targets
			if (!vultureUnitTargets.empty())
			{
				// find the best target for this Vulture
				BWAPI::Unit * target = getTarget(vultureUnit, vultureUnitTargets);

				// attack it
				kiteTarget(vultureUnit, target);
			}
			// if there are no targets
			else
			{
				// if we're not near the order position
				if (vultureUnit->getDistance(order.position) > 100)
				{
					// move to it
					smartAttackMove(vultureUnit, order.position);
				}
			}
		}

		if (Options::Debug::DRAW_UALBERTABOT_DEBUG)
		{
			BWAPI::Broodwar->drawLineMap(vultureUnit->getPosition().x(), vultureUnit->getPosition().y(),
				vultureUnit->getTargetPosition().x(), vultureUnit->getTargetPosition().y(), Options::Debug::COLOR_LINE_TARGET);
		}
	}

}

void VultureManagerExt::vultureAttack(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void VultureManagerExt::vultureDefend(BWAPI::Unit* terranMarine, UnitVector& marineTargets)
{

}

void VultureManagerExt::vultureAdvanceToPosition(BWAPI::Unit * terranMarine, UnitVector& marineTargets)
{

}

int VultureManagerExt::getAttackPriority(BWAPI::Unit * rangedUnit, BWAPI::Unit * target)
{
	BWAPI::UnitType vultureUnitType = rangedUnit->getType();
	BWAPI::UnitType targetType = target->getType();

	bool canAttackUs = targetType.groundWeapon() != BWAPI::WeaponTypes::None;
	int vultureWeaponRange = vultureUnitType.groundWeapon().maxRange();		// 160, Concussive
	int targetWeaponRange = targetType.groundWeapon().maxRange();

	// Vulture cannot attack flyers
	if (targetType.isFlyer())
	{
		return 0;
	}
	// Faster than Vulture
	else if ((targetType.topSpeed() >= vultureUnitType.topSpeed())
		|| ((targetType == BWAPI::UnitTypes::Protoss_Zealot) 
			&& (BWAPI::Broodwar->enemy()->getUpgradeLevel(BWAPI::UpgradeTypes::Leg_Enhancements) > 0)))
	{
		return vultureWeaponRange;		// return 160
	}
	// Slower than Vulture
	else
	{
		int priority = vultureWeaponRange - targetWeaponRange;
		if (priority < 0)
		{
			priority = 0;
		}

		return priority;
	}

}

BWAPI::Unit* VultureManagerExt::getTarget(BWAPI::Unit * vultureUnit, UnitVector & targets)
{
	int range(vultureUnit->getType().groundWeapon().maxRange());

	int highestInRangePriority(0);
	int highestNotInRangePriority(0);
	int lowestInRangeHitPoints(10000);
	int lowestNotInRangeDistance(10000);

	BWAPI::Unit * inRangeTarget = NULL;
	BWAPI::Unit * notInRangeTarget = NULL;

	BOOST_FOREACH(BWAPI::Unit * unit, targets)
	{
		int priority = getAttackPriority(vultureUnit, unit);
		int distance = vultureUnit->getDistance(unit);

		// if the unit is in range, update the target with the lowest hp
		if (vultureUnit->getDistance(unit) <= range)
		{
			if (priority > highestInRangePriority ||
				(priority == highestInRangePriority && unit->getHitPoints() < lowestInRangeHitPoints))
			{
				lowestInRangeHitPoints = unit->getHitPoints();
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

void VultureManagerExt::kiteTarget(BWAPI::Unit * vultureUnit, BWAPI::Unit * target)
{	
	double vultureRange(vultureUnit->getType().groundWeapon().maxRange());
	double targetRange(target->getType().groundWeapon().maxRange());

	// determine whether the target can be kited
	if (vultureRange <= target->getType().groundWeapon().maxRange())
	{
		// if we can't kite it, there's no point
		smartAttackUnit(vultureUnit, target);
		return;
	}

	bool		kite(true);
	double		dist(vultureUnit->getDistance(target));
	double		speed(vultureUnit->getType().topSpeed());

	// TODO: make the speed upgrade real. Currently ion thrusters speed is set to 24, which is not true.
	if ((BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Ion_Thrusters) > 0))
	{
		speed = 24;	
	}

	int vultureWeaponCooldown = vultureUnit->getGroundWeaponCooldown();
	int meleeRange = 15;

	// If we are going to be out of range (melee range added just to ensure we are still in range)
	// or if weapon is ready then attack
	if ((vultureWeaponCooldown == 0)
		|| (dist >= (vultureRange - meleeRange)))
	{
		smartAttackUnit(vultureUnit, target);
		return;
	}
	else
	{
		// Run in the opposite direction to the enemy
		//BWAPI::Position fleePosition(vultureUnit->getPosition() - target->getPosition() + vultureUnit->getPosition());

		BWAPI::Broodwar->printf("                                           DebExt: kiteTarget 1");
		BWAPI::Position fleePosition(vultureUnit->getPosition() - _averageEnemyPosition + vultureUnit->getPosition());
		BWAPI::Broodwar->printf("                                           DebExt: kiteTarget 2");

		BWAPI::Broodwar->drawLineMap(vultureUnit->getPosition().x(), vultureUnit->getPosition().y(),
			fleePosition.x(), fleePosition.y(), BWAPI::Colors::Cyan);		

		putMine(vultureUnit);

		BWAPI::Broodwar->printf("                                           DebExt: kiteTarget 3");
		smartMove(vultureUnit, fleePosition);
		BWAPI::Broodwar->printf("                                           DebExt: kiteTarget 4");
	}

}

void VultureManagerExt::setAverageEnemyPosition(const UnitVector& targets)
{
	// If there are no valid targets, return opposite direction than start location,
	// so the Vulture will run straight to base
	BWAPI::Broodwar->printf("                                           DebExt: setAverageEnemyPos()");
	if (targets.empty())
	{
		//_averageEnemyPosition.x = (BWAPI::Broodwar->self()->getStartLocation().x());
		//_averageEnemyPosition.y = (BWAPI::Broodwar->self()->getStartLocation().y());
		BWAPI::Broodwar->printf("                                           DebExt: setAverageEnemyPos 1");
		BWAPI::Position avgPos((-BWAPI::Broodwar->self()->getStartLocation().x()), (-BWAPI::Broodwar->self()->getStartLocation().y()));
		_averageEnemyPosition = avgPos;
		BWAPI::Broodwar->printf("                                           DebExt: setAverageEnemyPos 2");
		return;
	}

	//BWAPI::Broodwar->printf("                                           DebExt: setAverageEnemyPos 3");
	BWAPI::Position sumPos(targets[0]->getPosition());
	for (int i = 1; i < targets.size(); i++)
	{
		sumPos += targets[i]->getPosition();
	}

	////int xPos = (sumPos.x() / targets.size());
	////int yPos = (sumPos.y() / targets.size());

	////BWAPI::Position avgPos(xPos, yPos);
	////_averageEnemyPosition = avgPos;

	_averageEnemyPosition = sumPos;
	//BWAPI::Broodwar->printf("                                           DebExt: setAverageEnemyPos 4");
	//_averageEnemyPosition.x = (sumPos.x / targets.size());
	//_averageEnemyPosition.y = (sumPos.y / targets.size());
}

void VultureManagerExt::putMine(BWAPI::Unit * vultureUnit)
{
	if (BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Spider_Mines))
	{
		vultureUnit->useTech(BWAPI::TechTypes::Spider_Mines, vultureUnit->getPosition());
	}
	else
	{
		return;
	}
}
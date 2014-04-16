#include "Common.h"
#include "MarineManagerExt.h"
#include "MicroManager.h"


MarineManagerExt::MarineManagerExt()
{
}


MarineManagerExt::~MarineManagerExt()
{
}

void MarineManagerExt::executeMicro(const UnitVector & targets)
{
	BWAPI::Broodwar->printf("                                           DebExt: Extension execute()");
	// Units vector initialized by MicroManager::setUnits, called by Squad::setManagerUnits
	const UnitVector & terranMarines = getUnits();
	
	UnitVector marineTargets;							
	// Set targets only to visible units
	for (size_t i(0); i<targets.size(); i++)
	{		
		// conditions for targeting
		if (targets[i]->isVisible())
		{
			marineTargets.push_back(targets[i]);
		}
	}

	// For each Terran Marine
	BOOST_FOREACH(BWAPI::Unit * terranMarine, terranMarines)
	{
		if (order.type == order.Attack)
		{
			marinesAttack(terranMarine, marineTargets);
		}
		else if (order.type == order.Defend)
		{
			marinesDefend(terranMarine, marineTargets);
		}
		else
		{
			marinesAdvanceToPosition(terranMarine, marineTargets); 
		}
	
		// Draw debug info
		if (Options::Debug::DRAW_UALBERTABOT_DEBUG)
		{
			BWAPI::Broodwar->drawLineMap(terranMarine->getPosition().x(), terranMarine->getPosition().y(),
				terranMarine->getTargetPosition().x(), terranMarine->getTargetPosition().y(), Options::Debug::COLOR_LINE_TARGET);
		}
	}	
}

void MarineManagerExt::marinesAttack(BWAPI::Unit * terranMarine, UnitVector& marineTargets)
{
	// if there are targets
	if (!marineTargets.empty())
	{
		// find the best target for this Marine
		BWAPI::Unit * target = getTarget(terranMarine, marineTargets);

		// attack it
		kiteTarget(terranMarine, target);
	}
	// if there are no targets
	else
	{
		// if we're not near the order position
		if (terranMarine->getDistance(order.position) > 100)
		{
			// move to it
			smartAttackMove(terranMarine, order.position);
		}
	}
}

// Temporarily just invokes marinesAttack method
void MarineManagerExt::marinesDefend(BWAPI::Unit * terranMarine, UnitVector& marineTargets)
{
	marinesAttack(terranMarine, marineTargets);
}

void MarineManagerExt::marinesAdvanceToPosition(BWAPI::Unit * terranMarine, UnitVector& marineTargets)
{
	if (terranMarine->getDistance(order.position) > 100)
	{
		smartAttackMove(terranMarine, order.position);
	}
}

// Method copied from RangedManager::kiteTarget
void MarineManagerExt::kiteTarget(BWAPI::Unit * rangedUnit, BWAPI::Unit * target)
{

}

// Method copied from RangedManager::getTarget
BWAPI::Unit* MarineManagerExt::getTarget(BWAPI::Unit * terranMarine, UnitVector & targets)
{
	int range(terranMarine->getType().groundWeapon().maxRange());

	int highestInRangePriority(0);
	int highestNotInRangePriority(0);
	int lowestInRangeHitPoints(10000);
	int lowestNotInRangeDistance(10000);

	BWAPI::Unit * inRangeTarget = NULL;
	BWAPI::Unit * notInRangeTarget = NULL;

	BOOST_FOREACH(BWAPI::Unit * unit, targets)
	{
		int priority = getAttackPriority(terranMarine, unit);
		int distance = terranMarine->getDistance(unit);

		// if the unit is in range, update the target with the lowest hp
		if (terranMarine->getDistance(unit) <= range)
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

// Method copied from RangedManager::getAttackPriority
int MarineManagerExt::getAttackPriority(BWAPI::Unit * terranMarine, BWAPI::Unit * target)
{
	BWAPI::UnitType rangedUnitType = terranMarine->getType();
	BWAPI::UnitType targetType = target->getType();

	bool canAttackUs = rangedUnitType.isFlyer() ? targetType.airWeapon() != BWAPI::WeaponTypes::None : targetType.groundWeapon() != BWAPI::WeaponTypes::None;



	// highest priority is something that can attack us or aid in combat
	if (targetType == BWAPI::UnitTypes::Terran_Medic || canAttackUs ||
		targetType == BWAPI::UnitTypes::Terran_Bunker)
	{
		return 3;
	}
	// next priority is worker
	else if (targetType.isWorker())
	{
		return 2;
	}
	// then everything else
	else
	{
		return 1;
	}
}

double MarineManagerExt::getGroundRange()
{
	return 0;
}

double MarineManagerExt::getAirRange()
{
	return 0;
}

double MarineManagerExt::getSpeed()
{
	return 0;
}

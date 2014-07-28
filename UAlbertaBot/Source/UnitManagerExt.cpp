#include "UnitManagerExt.h"


UnitManagerExt::UnitManagerExt()
{
	//_unitsData.reserve(200);
}


UnitManagerExt::~UnitManagerExt()
{
}

void UnitManagerExt::update()
{
	// Erase not-existing units from the vector
	for (UnitDataVector::iterator it = _unitsData.begin(); it != _unitsData.end(); /*++it*/)
	{
		
		if (!it->getUnit()->exists())
		{
			_unitsData.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void UnitManagerExt::addUnit(BWAPI::Unit* unitToAdd)
{
	update();

	int unitToAddId = unitToAdd->getID();

	// Check if unit is already added
	BOOST_FOREACH(UnitDataExt unitData, _unitsData)
	{
		if (unitToAddId == unitData.getUnit()->getID())
		{
			return;
		}
	}

	// if it's not then add it
	UnitDataExt unitData = UnitDataExt(unitToAdd);
	_unitsData.push_back(unitData);

}

UnitDataExt* UnitManagerExt::getUnitData(BWAPI::Unit* unit)
{
	for (int i = 0; i < _unitsData.size(); i++)
	{
		if (unit->getID() == _unitsData.at(i).getUnitId())
		{
			return &_unitsData.at(i);
		}
	}

	// If unit was not found
	BWAPI::Broodwar->printf("                                           DebExt: getUnitData error: Unit not found!");
	return NULL;
}

BWAPI::Position UnitManagerExt::getMovePosition(BWAPI::Unit* unit)
{
	UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(unit);

	if (!unitData->isWaypointReached())
	{
		return unitData->getWaypoint();
	}
	else
	{
		unitData->popWaypoint();
		return unitData->getWaypoint();
	}

}

void UnitManagerExt::setWaypoints(BWAPI::Unit* attacker, BWAPI::Position targetPosition, WaypointMovementType movementType)
{
	if (movementType == WaypointMovementType_BorderMovement)
	{
		UnitManagerExt::Instance().addUnit(attacker);
		UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(attacker);

		// Recompute destination if order position has changed
		if (targetPosition != unitData->getDestination())
		{
			unitData->isDestinationSet = false;
			unitData->eraseWaypoints();
		}

		if (!unitData->isDestinationSet)
		{
			WaypointCreatorExt::setBorderMoveWaypoints(attacker, targetPosition);
		}
	}
}

void UnitManagerExt::setUnitStateToAttack(BWAPI::Unit* unit)
{
	UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(unit);
	unitData->setState(UnitDataExt::State_Attacking);
}

bool UnitManagerExt::isAttacking(BWAPI::Unit* unit)
{
	UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(unit);
	if (unitData->getState() == UnitDataExt::State_Attacking)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UnitManagerExt::setLandingPosition(BWAPI::Unit* unit, BWAPI::TilePosition landingPos)
{
	UnitManagerExt::Instance().addUnit(unit);

	UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(unit);

	unitData->setLandingPosition(landingPos);
}

BWAPI::TilePosition UnitManagerExt::getLandingPosition(BWAPI::Unit* unit)
{
	UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(unit);

	if (unitData != NULL)
	{
		return unitData->getLandingPosition();
	}
	else
	{
		return BWAPI::Broodwar->self()->getStartLocation().makeValid();
	}
}

UnitManagerExt& UnitManagerExt::Instance()
{	
	static UnitManagerExt instance;
	return instance;
}

bool UnitManagerExt::isPerformingAction(BWAPI::Unit* unit)
{
	UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(unit);

	if (unitData != NULL)
	{
		return unitData->isPerformingAction();
	}
}

void UnitManagerExt::setIsPerformingAction(BWAPI::Unit* unit, bool isPerforming)
{
	UnitManagerExt::Instance().addUnit(unit);

	UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(unit);

	unitData->setIsPerformingAction(isPerforming);
}

bool UnitManagerExt::isPuttingMine(BWAPI::Unit* unit)
{
	if (unit->getType() != BWAPI::UnitTypes::Terran_Vulture)
	{
		return false;
	}
	else
	{
		UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(unit);

		if (unitData != NULL)
		{
			return unitData->isPuttingMine();
		}
		else
		{
			addUnit(unit);
			return false;
		}
	}
}

void UnitManagerExt::putMineFlagOn(BWAPI::Unit* unit)
{
	UnitManagerExt::Instance().addUnit(unit); 
	if (unit->getType() != BWAPI::UnitTypes::Terran_Vulture)
	{
		return;
	}
	else
	{
		UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(unit);

		if (unitData != NULL)
		{			
			unitData->refreshStartingMines();
		}
	}
}

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
			BWAPI::Broodwar->printf("                                           DebExt: UnitManagerExt update unit it = %d", it->getUnit()->getID());
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

		if (!unitData->isDestinationSet)
		{
			WaypointCreatorExt::setBorderMoveWaypoints(attacker, targetPosition);
		}
	}
}

UnitManagerExt& UnitManagerExt::Instance()
{	
	static UnitManagerExt instance;
	return instance;
}

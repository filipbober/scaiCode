#include "UnitDataExt.h"

UnitDataExt::UnitDataExt(const BWAPI::Unit* unit)
: _unit(unit)
{
	_state = State_Idle;
}

UnitDataExt::UnitDataExt(BWAPI::Position destination, const BWAPI::Unit* unit)
: _unit(unit)
{
	_destination = destination;
	_state = State_Idle;
}


UnitDataExt::~UnitDataExt()
{
}

int UnitDataExt::getUnitId() const
{
	return _unit->getID();
}

const BWAPI::Unit* UnitDataExt::getUnit() const
{
	return _unit;
}


UnitDataExt::State UnitDataExt::getState()
{
	return _state;
}

void UnitDataExt::setState(State state)
{
	_state = state;
}

void UnitDataExt::setDestination(BWAPI::Position destination)
{
	_destination = destination;
}

void UnitDataExt::addWaypoint(BWAPI::Position waypoint)
{
	_waypoints.push_back(waypoint);
}

BWAPI::Position UnitDataExt::nextWaypoint()
{
	if (!_waypoints.empty())
	{
		BWAPI::Position pos;
		pos = _waypoints.back();
		_waypoints.pop_back();
		return pos;
	}
	else
	{
		return _destination;
	}
}

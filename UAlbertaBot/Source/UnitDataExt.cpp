#include "UnitDataExt.h"

UnitDataExt::UnitDataExt(const BWAPI::Unit* unit)
: _unit(unit),
isDestinationSet(false),
_waypoints(),
_waypointProximity(30)
{
	_state = State_Idle;
	//_waypoints.reserve(10);
}

UnitDataExt::UnitDataExt(BWAPI::Position destination, const BWAPI::Unit* unit)
: _unit(unit),
_waypointProximity(30)
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
	isDestinationSet = true;
}

BWAPI::Position UnitDataExt::getDestination()
{
	return _destination;
}

void UnitDataExt::pushWaypoint(BWAPI::Position waypoint)
{
	//BWAPI::Broodwar->printf("                                           DebExt: pushWaypoint");
	//BWAPI::Broodwar->printf("                                           DebExt: waypoint x = %d", waypoint.y());
	//BWAPI::Broodwar->printf("                                           DebExt: waypoint y = %d", waypoint.y());
	_waypoints.push_back(waypoint);
	BWAPI::Broodwar->printf("                                           DebExt: waypoints size = %d", _waypoints.size());
}

BWAPI::Position UnitDataExt::popWaypoint()
{
	BWAPI::Broodwar->printf("                                           DebExt: popWaypoint");	

	if (!_waypoints.empty())
	{
		BWAPI::Position pos = _waypoints.back();
		_waypoints.pop_back();

		return pos;
	}
	else
	{
		return _destination;
	}

	return _destination;
}

BWAPI::Position UnitDataExt::getWaypoint() const
{
	if (!_waypoints.empty())
	{
		return _waypoints.back();
	}
	else
	{
		return _destination;
	}
}

void UnitDataExt::eraseWaypoints()
{
	_waypoints.clear();
}

bool UnitDataExt::isWaypointReached()
{
	int distance = _unit->getDistance(_waypoints.back());
	
	if (distance <= _waypointProximity)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#include "UnitDataExt.h"

UnitDataExt::UnitDataExt(const BWAPI::Unit* unit)
: _unit(unit),
isDestinationSet(false),
_waypoints()
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
	isDestinationSet = true;
}

void UnitDataExt::pushWaypoint(BWAPI::Position waypoint)
{
	BWAPI::Broodwar->printf("                                           DebExt: pushWaypoint");
	BWAPI::Broodwar->printf("                                           DebExt: waypoint x = %d", waypoint.y());
	BWAPI::Broodwar->printf("                                           DebExt: waypoint y = %d", waypoint.y());
	_waypoints.push_back(waypoint);
	BWAPI::Broodwar->printf("                                           DebExt: waypoints size = %d", _waypoints.size());
}

BWAPI::Position UnitDataExt::popWaypoint()
{
	BWAPI::Broodwar->printf("                                           DebExt: popWaypoint");	

	if (!_waypoints.empty())
	{
		//int i = 0;
		//BOOST_FOREACH(BWAPI::Position waypoint, _waypoints)
		//{
		//	BWAPI::Broodwar->printf("                                           DebExt: Waypint %d", i);
		//	BWAPI::Broodwar->printf("                                           DebExt: x = ", waypoint.x());
		//	BWAPI::Broodwar->printf("                                           DebExt: y = ", waypoint.y());
		//	i++;
		//}


		// Bug below
		//BWAPI::Position pos = _waypoints.back();
		//_waypoints.pop_back();

		//BWAPI::Broodwar->printf("                                           DebExt: pos x = %d", pos.x());
		//BWAPI::Broodwar->printf("                                           DebExt: pos y = %d", pos.y());

		//return pos;
	}
	else
	{
		BWAPI::Broodwar->printf("                                           DebExt: destination x = %d", _destination.x());
		BWAPI::Broodwar->printf("                                           DebExt: destination y = %d", _destination.y());
		return _destination;
	}

	return _destination;
}

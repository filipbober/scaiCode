#include "UnitDataExt.h"


UnitDataExt::UnitDataExt(BWAPI::Position destination, int unitId)
: _unitId(unitId)
{
	_destination = destination;
}


UnitDataExt::~UnitDataExt()
{
}

int UnitDataExt::getUnitId() const
{
	return _unitId;
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
		return &_destination;
	}
}

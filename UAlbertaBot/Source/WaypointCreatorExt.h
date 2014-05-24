#pragma once
#include "Common.h"
class WaypointCreatorExt
{
public:
	WaypointCreatorExt();
	~WaypointCreatorExt();

	std::vector<BWAPI::Position> WaypointCreatorExt::createBorderMoveWaypoints(BWAPI::Unit * attacker, BWAPI::Position targetPosition) const;
};


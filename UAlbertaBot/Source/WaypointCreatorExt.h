#pragma once
#include "Common.h"
class WaypointCreatorExt
{
public:
	WaypointCreatorExt();
	~WaypointCreatorExt();

	void WaypointCreatorExt::createBorderMoveWaypoints(BWAPI::Unit * attacker, BWAPI::Position targetPosition) const;
};


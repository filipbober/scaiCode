#pragma once
#include "Common.h"
#include "UnitManagerExt.h"

class WaypointCreatorExt
{
public:
	WaypointCreatorExt();
	~WaypointCreatorExt();

	static void WaypointCreatorExt::setBorderMoveWaypoints(BWAPI::Unit* attacker, BWAPI::Position targetPosition);
};


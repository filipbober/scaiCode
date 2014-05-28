#pragma once
#include "Common.h"
#include "UnitManagerExt.h"

class WaypointCreatorExt
{
public:
	WaypointCreatorExt();
	~WaypointCreatorExt();

	static void setBorderMoveWaypoints(BWAPI::Unit* attacker, BWAPI::Position targetPosition);

private:
	static int closestBorderId(BWAPI::Position mapPostition, BWAPI::Position borders[], int size = 4);
	static int distanceClockwise(BWAPI::Position fromPos, BWAPI::Position toPos, int distFromBorder);
	static int nextClockwiseId(int borderId);
	static int nextCounterclockwiseId(int borderId);
};


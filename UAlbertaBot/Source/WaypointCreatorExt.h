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
	static int nextClockwiseId(int borderId);
	static int nextCounterclockwiseId(int borderId);

	static void setMapBorderWaypoints(BWAPI::Position borderWaypoints[], int distFromBorder, int size = 8);
	static int getClosestBorderWaypointId(BWAPI::Position mapPostition, BWAPI::Position borderWaypoints[], int size = 8);
};


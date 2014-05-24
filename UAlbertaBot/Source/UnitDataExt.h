#pragma once
#include "Common.h"

//typedef std::vector<BWAPI::Position*> PositionVector;

class UnitDataExt
{
public:
	UnitDataExt(BWAPI::Position destination);
	~UnitDataExt();

	void addWaypoint(BWAPI::Position* waypoint);
	BWAPI::Position* nextWaypoint();

private:
	BWAPI::Position _destination;
	std::vector<BWAPI::Position*>  _waypoints;
	

};


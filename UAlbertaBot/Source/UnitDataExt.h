#pragma once
#include "Common.h"

//typedef std::vector<BWAPI::Position*> PositionVector;

class UnitDataExt
{
public:
	enum State { State_Moving, State_Attacking, State_Idle };

	UnitDataExt(const BWAPI::Unit* unit);
	UnitDataExt(BWAPI::Position destination, const BWAPI::Unit* unit);
	~UnitDataExt();

	bool isDestinationSet;

	int getUnitId() const;
	const BWAPI::Unit* getUnit() const;

	State getState();
	void setState(State state);

	void setDestination(BWAPI::Position destination);
	void pushWaypoint(BWAPI::Position waypoint);
	BWAPI::Position popWaypoint();	
	BWAPI::Position getWaypoint() const;

	bool isWaypointReached();						// True if current waypoint is reached

private:
	BWAPI::Position _destination;
	std::vector<BWAPI::Position>  _waypoints;
	const BWAPI::Unit* _unit;
	State _state;	
	int _waypointProximity;

};


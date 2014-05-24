#pragma once
#include "Common.h"

//typedef std::vector<BWAPI::Position*> PositionVector;

class UnitDataExt
{
public:
	enum State { State_Moving, State_Attacking, State_Idle };

	UnitDataExt(BWAPI::Position destination, const BWAPI::Unit* unit);
	~UnitDataExt();

	int getUnitId() const;
	const BWAPI::Unit* getUnit() const;

	State getState();
	void setState(State state);

	void addWaypoint(BWAPI::Position waypoint);
	BWAPI::Position nextWaypoint();	

private:
	BWAPI::Position _destination;
	std::vector<BWAPI::Position>  _waypoints;
	const BWAPI::Unit* _unit;
	State _state;
	

};


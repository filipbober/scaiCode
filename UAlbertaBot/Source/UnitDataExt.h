#pragma once
#include "Common.h"

//typedef std::vector<BWAPI::Position*> PositionVector;

class UnitDataExt
{
public:
	enum State { State_Moving, State_Attacking, State_Idle, State_Putting_Mine };

	UnitDataExt(const BWAPI::Unit* unit);
	UnitDataExt(BWAPI::Position destination, const BWAPI::Unit* unit);
	~UnitDataExt();

	bool isDestinationSet;

	int getUnitId() const;
	const BWAPI::Unit* getUnit() const;

	State getState();
	void setState(State state);

	void setDestination(BWAPI::Position destination);
	BWAPI::Position getDestination();
	void pushWaypoint(BWAPI::Position waypoint);
	BWAPI::Position popWaypoint();	
	BWAPI::Position getWaypoint() const;
	void eraseWaypoints();

	bool isWaypointReached();						// True if current waypoint is reached

	void setLandingPosition(BWAPI::TilePosition landingPos);
	BWAPI::TilePosition getLandingPosition();

	bool isPerformingAction();
	bool setIsPerformingAction(bool isPerforming);

	bool isPuttingMine();
	int getStartingMines();
	void refreshStartingMines();
	

private:
	BWAPI::Position _destination;
	std::vector<BWAPI::Position>  _waypoints;
	const BWAPI::Unit* _unit;
	State _state;	
	int _waypointProximity;
	BWAPI::TilePosition _landingPosition;
	bool _isPerformingAction;				// Like putting a mine
	int _startingMines;

};


#pragma once
#include "Common.h"
#include "UnitDataExt.h"
#include "WaypointCreatorExt.h"

typedef std::vector<UnitDataExt> UnitDataVector;

class UnitManagerExt
{
public:
	UnitManagerExt();
	~UnitManagerExt();

	enum WaypointMovementType { WaypointMovementType_BorderMovement };

	// Singleton
	static UnitManagerExt& Instance();

	void update();
	void addUnit(BWAPI::Unit* unitToAdd);
	UnitDataExt* getUnitData(BWAPI::Unit* unit);
	BWAPI::Position getMovePosition(BWAPI::Unit* unit);
	void setWaypoints(BWAPI::Unit* attacker, BWAPI::Position targetPosition, WaypointMovementType movementType);
	

private:
	//UnitVector _units;
	UnitDataVector _unitsData;
	

};


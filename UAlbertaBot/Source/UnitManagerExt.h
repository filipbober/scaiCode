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

	// Singleton
	static UnitManagerExt& Instance();

	void update();
	void addUnit(BWAPI::Unit* unitToAdd);
	UnitDataExt* getUnitData(BWAPI::Unit* unit);
	BWAPI::Position getMovePosition(BWAPI::Unit* unit);
	

private:
	//UnitVector _units;
	UnitDataVector _unitsData;
	

};


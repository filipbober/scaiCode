#pragma once
#include "MicroManager.h"
class ScienceVesselManagerExt :
	public MicroManager
{
	std::map<BWAPI::Unit *, bool>	cloakedUnitMap;

	bool isAssigned(BWAPI::Unit * unit);

	BWAPI::Unit * unitClosestToEnemy;

public:
	ScienceVesselManagerExt();
	~ScienceVesselManagerExt() {}

	void setUnitClosestToEnemy(BWAPI::Unit * unit) { unitClosestToEnemy = unit; }
	void executeMicro(const UnitVector & targets);

	BWAPI::Unit * closestCloakedUnit(const UnitVector & cloakedUnits, BWAPI::Unit * detectorUnit);
};


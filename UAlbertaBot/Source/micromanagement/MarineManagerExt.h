#pragma once
#include "Common.h"
#include "MicroManager.h"

class MarineManagerExt :
	public MicroManager
{
public:
	MarineManagerExt();
	~MarineManagerExt();

	void executeMicro(const UnitVector & targets);

private:
	void marinesAttack(BWAPI::Unit* terranMarine, UnitVector& marineTargets);
	void marinesDefend(BWAPI::Unit* terranMarine, UnitVector& marineTargets);
	void marinesAdvanceToPosition(BWAPI::Unit * terranMarine, UnitVector& marineTargets);

	void kiteTarget(BWAPI::Unit * rangedUnit, BWAPI::Unit * target);
	BWAPI::Unit* getTarget(BWAPI::Unit * terranMarine, UnitVector & targets);
	int getAttackPriority(BWAPI::Unit * terranMarine, BWAPI::Unit * target);

	double getGroundRange();
	double getAirRange();
	double getSpeed();
};


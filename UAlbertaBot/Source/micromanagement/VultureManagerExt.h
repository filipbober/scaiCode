#pragma once
#include "MicroManager.h"
#include "Common.h"

class VultureManagerExt :
	public MicroManager
{
public:
	VultureManagerExt();
	~VultureManagerExt();

	void executeMicro(const UnitVector & targets);

private:
	void vultureAttack(BWAPI::Unit* terranMarine, UnitVector& marineTargets);
	void vultureDefend(BWAPI::Unit* terranMarine, UnitVector& marineTargets);
	void vultureAdvanceToPosition(BWAPI::Unit * terranMarine, UnitVector& marineTargets);

	int getAttackPriority(BWAPI::Unit * rangedUnit, BWAPI::Unit * target);
	BWAPI::Unit * getTarget(BWAPI::Unit * rangedUnit, UnitVector & targets);
	void kiteTarget(BWAPI::Unit * rangedUnit, BWAPI::Unit * target);

};


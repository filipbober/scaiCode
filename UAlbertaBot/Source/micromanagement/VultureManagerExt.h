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
	void vultureAttack(BWAPI::Unit* vultureUnit, UnitVector& vultureTargets);
	void vultureDefend(BWAPI::Unit* vultureUnit, UnitVector& vultureTargets);
	void vultureAdvanceToPosition(BWAPI::Unit * terranMarine, UnitVector& marineTargets);

	int getAttackPriority(BWAPI::Unit * vultureUnit, BWAPI::Unit * target);
	BWAPI::Unit * getTarget(BWAPI::Unit * vultureUnit, UnitVector & targets);
	void kiteTarget(BWAPI::Unit * rangedUnit, BWAPI::Unit * target);

	void setAverageEnemyPosition(const UnitVector& targets);
	void putMine(BWAPI::Unit * vultureUnit);

	//Fields
	BWAPI::Position _averageEnemyPosition;

};


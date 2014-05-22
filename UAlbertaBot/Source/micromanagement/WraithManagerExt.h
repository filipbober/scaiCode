#pragma once
#include "MicroManager.h"
class WraithManagerExt :
	public MicroManager
{
public:
	WraithManagerExt();
	~WraithManagerExt();

	void executeMicro(const UnitVector & targets);

private:
	void executeAttack(BWAPI::Unit* selectedUnit, UnitVector& targets);
	void executeDefend(BWAPI::Unit* vultureUnit, UnitVector& targets);
	void executeAdvanceToPosition(BWAPI::Unit* selectedUnit, UnitVector& targets);

	int getAttackPriority(BWAPI::Unit * selectedUnit, BWAPI::Unit * target);
	BWAPI::Unit * getTarget(BWAPI::Unit * selectedUnit, UnitVector& targets);
	void kiteTarget(BWAPI::Unit * selectedUnit, BWAPI::Unit * target);

	void setAverageEnemyPosition(const UnitVector& targets);
	void manageCloak(BWAPI::Unit * selectedUnit, UnitVector& targets);

	//Fields
	BWAPI::Position _averageEnemyPosition;
};


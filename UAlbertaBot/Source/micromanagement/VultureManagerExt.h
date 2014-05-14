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
	void executeAttack(BWAPI::Unit* selectedUnit, UnitVector& targets);
	void executeDefend(BWAPI::Unit* vultureUnit, UnitVector& targets);
	void executeAdvanceToPosition(BWAPI::Unit* selectedUnit, UnitVector& targets);

	int getAttackPriority(BWAPI::Unit * selectedUnit, BWAPI::Unit * target);
	BWAPI::Unit * getTarget(BWAPI::Unit * selectedUnit, UnitVector& targets);
	void kiteTarget(BWAPI::Unit * selectedUnit, BWAPI::Unit * target);

	void setAverageEnemyPosition(const UnitVector& targets);
	void putMine(BWAPI::Unit * selectedUnit);

	//Fields
	BWAPI::Position _averageEnemyPosition;

};


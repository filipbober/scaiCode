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
	void executeAttack(BWAPI::Unit* selectedUnit, UnitVector& targets);
	void executeDefend(BWAPI::Unit* vultureUnit, UnitVector& targets);
	void executeAdvanceToPosition(BWAPI::Unit* selectedUnit, UnitVector& targets);

	int getAttackPriority(BWAPI::Unit * selectedUnit, BWAPI::Unit * target);
	BWAPI::Unit * getTarget(BWAPI::Unit * selectedUnit, UnitVector& targets);
	void kiteTarget(BWAPI::Unit * selectedUnit, BWAPI::Unit * target);

	void setAverageEnemyPosition(const UnitVector& targets);
	void useStimpack(BWAPI::Unit * selectedUnit);
	void goToBunker(BWAPI::Unit * selectedUnit);
	bool hasBunkerSpace();

	//Fields
	BWAPI::Position _averageEnemyPosition;
};


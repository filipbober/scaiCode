#pragma once
#include "MicroManager.h"
#include "Common.h"

class TankManagerExt :
	public MicroManager
{
public:
	TankManagerExt();
	~TankManagerExt();

	void executeMicro(const UnitVector & targets);

private:
	void executeAttack(BWAPI::Unit* selectedUnit, UnitVector& targets);
	void executeDefend(BWAPI::Unit* vultureUnit, UnitVector& targets);
	void executeAdvanceToPosition(BWAPI::Unit* selectedUnit, UnitVector& targets);

	int getAttackPriority(BWAPI::Unit * selectedUnit, BWAPI::Unit * target);
	BWAPI::Unit * getTarget(BWAPI::Unit * selectedUnit, UnitVector& targets);
	void kiteTarget(BWAPI::Unit * selectedUnit, BWAPI::Unit * target);

	void setAverageEnemyPosition(const UnitVector& targets);

	bool isAttack();
	void executeTerranWraithRush1Port(BWAPI::Unit * selectedUnit, UnitVector& selectedUnitTargets);

	double closestEnemyDist(BWAPI::Unit* selectedUnit);

	void siegeModeOn(BWAPI::Unit* selectedUnit);
	void siegeModeOff(BWAPI::Unit* selectedUnit);

	//Fields
	BWAPI::Position _averageEnemyPosition;
	int _siegeModeFrame;
};


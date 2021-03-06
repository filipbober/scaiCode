#pragma once
#include "MicroManager.h"
class GoliathManagerExt :
	public MicroManager
{
public:
	GoliathManagerExt();
	~GoliathManagerExt();

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
	void executeTerranVulturesAndTanks(BWAPI::Unit * selectedUnit, UnitVector& selectedUnitTargets);

	//Fields
	BWAPI::Position _averageEnemyPosition;
};


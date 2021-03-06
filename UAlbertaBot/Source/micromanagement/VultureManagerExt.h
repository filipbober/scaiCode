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

	bool isAttack();
	void executeTerranWraithRush1Port(BWAPI::Unit * selectedUnit, UnitVector& selectedUnitTargets);

	void putMine(BWAPI::Unit * selectedUnit, BWAPI::Unit* target);
	void putMine(BWAPI::Unit * selectedUnit, BWAPI::Position targetPosition);
	BWAPI::Position getMinePosition(BWAPI::Unit* selectedUnit, BWAPI::Position targetPosition, int proximity);

	void attackOrMine(BWAPI::Unit * selectedUnit, BWAPI::Unit* target);
	void fleeOrMine(BWAPI::Unit * selectedUnit, BWAPI::Position fleePosition);
	bool isMineProximity(BWAPI::Unit* selectedUnit);

	double closestEnemyDist(BWAPI::Unit* selectedUnit);

	//Fields
	BWAPI::Position _averageEnemyPosition;

	bool _isPuttingMine;
	int _putMineFrame;
	

};


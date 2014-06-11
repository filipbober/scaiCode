#pragma once
#include "MicroManager.h"
class BattlecruiserManagerExt :
	public MicroManager
{
public:
	BattlecruiserManagerExt();
	~BattlecruiserManagerExt();

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
	bool isInTurretRange(BWAPI::Position position, UnitVector & targets, BWAPI::Unit* selectedUnit);
	bool isTurret(BWAPI::Unit* target);
	BWAPI::Position getSafeTurretPosition(BWAPI::Unit* selectedUnit, BWAPI::Unit* target, int proximity);
	int getTargetWeaponRange(BWAPI::Unit* selectedUnit, BWAPI::Unit* target);

	//Fields
	BWAPI::Position _averageEnemyPosition;
	int _noTurretTargetsNo;
};


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

	bool isTurret(BWAPI::Unit* target);
	int getTargetWeaponRange(BWAPI::Unit* selectedUnit, BWAPI::Unit* target);

	void useYamatoGun(BWAPI::Unit* selectedUnit, BWAPI::Unit* target);

	//Fields
	BWAPI::Position _averageEnemyPosition;
	int _noTurretTargetsNo;
};


#pragma once

#include "Common.h"
#include "micromanagement/MeleeManager.h"
#include "micromanagement/RangedManager.h"
#include "micromanagement/DetectorManager.h"
#include "micromanagement/TransportManager.h"
#include "SquadOrder.h"
#include "DistanceMap.hpp"
#include "StrategyManager.h"
#include "CombatSimulation.h"

#include "micromanagement/MarineManagerExt.h"
#include "micromanagement/VultureManagerExt.h"
#include "micromanagement/WraithManagerExt.h"
#include "micromanagement/BattlecruiserManagerExt.h"
#include"micromanagement/TankManagerExt.h"
#include "micromanagement/GoliathManagerExt.h"
#include "micromanagement/ScienceVesselManagerExt.h"
#include "UnitManagerExt.h"

class ZealotManager;
class DarkTemplarManager;
class DragoonManager;
class ObserverManager;

class MeleeManager;
class RangedManager;
class DetectorManager;

class Squad
{
	UnitVector			units;
	std::string			regroupStatus;
	int					lastFrameRegroup;
	bool				squadObserverNear(BWAPI::Position p);
	
	SquadOrder			order;
	MeleeManager		meleeManager;
	RangedManager		rangedManager;
	DetectorManager		detectorManager;
	TransportManager	transportManager;

	//Extensions
	//MarineManagerExt marineManager;
	MarineManagerExt marineManager;	// For testing
	VultureManagerExt vultureManager;
	WraithManagerExt wraithManager;
	BattlecruiserManagerExt bcManager;
	TankManagerExt tankManager;
	GoliathManagerExt goliathManager;
	ScienceVesselManagerExt scienceVesselManager;
	// eof ext

	std::map<BWAPI::Unit *, bool>	nearEnemy;

	void				updateUnits();
	void				setManagerUnits();
	void				setNearEnemyUnits();
	void				setAllUnits();
	
	void				setUnits(const UnitVector & u)	{ units = u; }
	
	bool				unitNearEnemy(BWAPI::Unit * unit);
	bool				needsToRegroup();
	BWAPI::Unit *		getRegroupUnit();
	int					squadUnitsNear(BWAPI::Position p);

	BWAPI::Unit *		unitClosestToEnemy();

    static int          lastRetreatSwitch;
    static bool         lastRetreatSwitchVal;

public:


	Squad(const UnitVector & units, SquadOrder order);
	Squad() {}
	~Squad() {}

	BWAPI::Position		calcCenter();
	BWAPI::Position		calcRegroupPosition();

	void				update();

	const UnitVector &	getUnits() const;
	const SquadOrder &	getSquadOrder()	const;

	void				setSquadOrder(const SquadOrder & so);
};

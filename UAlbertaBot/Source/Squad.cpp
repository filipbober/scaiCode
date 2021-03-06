#include "Common.h"
#include "Squad.h"
#include <boost\lexical_cast.hpp>

int  Squad::lastRetreatSwitch = 0;
bool Squad::lastRetreatSwitchVal = false;

Squad::Squad(const UnitVector & units, SquadOrder order) 
	: units(units)
	, order(order)
{
}

void Squad::update()
{
	// update all necessary unit information within this squad
	updateUnits();

	// determine whether or not we should regroup
	const bool needToRegroup(needsToRegroup());
	
	// draw some debug info
	if (Options::Debug::DRAW_UALBERTABOT_DEBUG && order.type == SquadOrder::Attack) 
	{
		BWAPI::Broodwar->drawTextScreen(200, 330, "%s", regroupStatus.c_str());

		BWAPI::Unit * closest = unitClosestToEnemy();
		if (closest && (BWAPI::Broodwar->getFrameCount() % 24 == 0))
		{
			//BWAPI::Broodwar->setScreenPosition(closest->getPosition().x() - 320, closest->getPosition().y() - 200);
		}
	}

	// if we do need to regroup, do it
	if (needToRegroup)
	{
		InformationManager::Instance().lastFrameRegroup = 1;

		const BWAPI::Position regroupPosition(calcRegroupPosition());
		BWAPI::Broodwar->drawTextScreen(200, 150, "REGROUP");

		BWAPI::Broodwar->drawCircleMap(regroupPosition.x(), regroupPosition.y(), 30, BWAPI::Colors::Purple, true);

		meleeManager.regroup(regroupPosition);
		rangedManager.regroup(regroupPosition);
		
		// Extensions
		marineManager.regroup(regroupPosition);
		vultureManager.regroup(regroupPosition);
		wraithManager.regroup(regroupPosition);
		bcManager.regroup(regroupPosition);
		tankManager.regroup(regroupPosition);
		goliathManager.regroup(regroupPosition);
		scienceVesselManager.regroup(regroupPosition);
		// eof ext
	}
	else // otherwise, execute micro
	{
		InformationManager::Instance().lastFrameRegroup = 1;

		meleeManager.execute(order);
		rangedManager.execute(order);
		transportManager.execute(order);

		// Extensions
		marineManager.execute(order);
		vultureManager.execute(order);
		wraithManager.execute(order);
		bcManager.execute(order);
		tankManager.execute(order);
		goliathManager.execute(order);
		scienceVesselManager.execute(order);
		// eof ext

		detectorManager.setUnitClosestToEnemy(unitClosestToEnemy());
		detectorManager.execute(order);
	}
}

void Squad::updateUnits()
{
	setAllUnits();
	setNearEnemyUnits();
	setManagerUnits();
}

void Squad::setAllUnits()
{
	// clean up the units vector just in case one of them died
	UnitVector goodUnits;
	BOOST_FOREACH(BWAPI::Unit * unit, units)
	{
		if( unit->isCompleted() && 
			unit->getHitPoints() > 0 && 
			unit->exists() &&
			unit->getPosition().isValid() &&
			unit->getType() != BWAPI::UnitTypes::Unknown)
		{
			goodUnits.push_back(unit);
		}
	}
	units = goodUnits;
}

void Squad::setNearEnemyUnits()
{
	nearEnemy.clear();
	BOOST_FOREACH(BWAPI::Unit * unit, units)
	{
		int x = unit->getPosition().x();
		int y = unit->getPosition().y();

		int left = unit->getType().dimensionLeft();
		int right = unit->getType().dimensionRight();
		int top = unit->getType().dimensionUp();
		int bottom = unit->getType().dimensionDown();

		nearEnemy[unit] = unitNearEnemy(unit);
		if (nearEnemy[unit])
		{
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawBoxMap(x-left, y - top, x + right, y + bottom, Options::Debug::COLOR_UNIT_NEAR_ENEMY);
		}
		else
		{
			if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawBoxMap(x-left, y - top, x + right, y + bottom, Options::Debug::COLOR_UNIT_NOTNEAR_ENEMY);
		}
	}
}

void Squad::setManagerUnits()
{
	UnitVector meleeUnits;
	UnitVector rangedUnits;
	UnitVector detectorUnits;
	UnitVector transportUnits;

	// Extensions
	UnitVector terranMarines;
	UnitVector terranVultures;
	UnitVector terranComsatStations;
	UnitVector terranWraiths;
	UnitVector terranBCs;
	UnitVector terranTanks;
	UnitVector terranGoliaths;
	UnitVector terranScienceVessels;
	// eof ext

	// add units to micro managers
	BOOST_FOREACH(BWAPI::Unit * unit, units)
	{
		if(unit->isCompleted() && unit->getHitPoints() > 0 && unit->exists())
		{
			// Select Terran Marines
			if (unit->getType() == BWAPI::UnitTypes::Terran_Marine)
			{				
				terranMarines.push_back(unit);
			}
			else if (unit->getType() == BWAPI::UnitTypes::Terran_Firebat)
			{
				terranMarines.push_back(unit);
			}
			// Select Terran Vultures
			else if (unit->getType() == BWAPI::UnitTypes::Terran_Vulture)
			{
				terranVultures.push_back(unit);
				UnitManagerExt::Instance().addUnit(unit);
			}
			// Do not add mines to the manager
			else if (unit->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine)
			{
				continue;
			}
			// Select Terran Wraiths
			else if (unit->getType() == BWAPI::UnitTypes::Terran_Wraith)
			{

				// Unit data & border movement
				//UnitManagerExt::Instance().addUnit(unit);
				//UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(unit);

				//if (!unitData->isDestinationSet)
				//{		
				//	WaypointCreatorExt::setBorderMoveWaypoints(unit, order.position);
				//}
				
				if (order.type == SquadOrder::Attack
					&& (StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranWraithRush1Port))
				{
					UnitManagerExt::Instance().setWaypoints(unit, order.position, UnitManagerExt::WaypointMovementType_BorderMovement);

					// Setting waypoints sets unit state to attack
					UnitManagerExt::Instance().setUnitStateToAttack(unit);
				}

				// eof unit data

				terranWraiths.push_back(unit);
			}
			else if (unit->getType() == BWAPI::UnitTypes::Terran_Battlecruiser)
			{
				terranBCs.push_back(unit);
			}
			// Set tanks
			else if (unit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode
				|| unit->getType() == BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode)
			{
				terranTanks.push_back(unit);
				UnitManagerExt::Instance().addUnit(unit);
			}
			else if (unit->getType() == BWAPI::UnitTypes::Terran_Goliath)
			{
				terranGoliaths.push_back(unit);
				UnitManagerExt::Instance().addUnit(unit);
			}
			else if (unit->getType() == BWAPI::UnitTypes::Terran_Science_Vessel)
			{
				terranScienceVessels.push_back(unit);
				UnitManagerExt::Instance().addUnit(unit);
			}
			// select detector units
			else if (unit->getType().isDetector() && !unit->getType().isBuilding())
			{
				detectorUnits.push_back(unit);
			}
			// select transport units
			else if (unit->getType() == BWAPI::UnitTypes::Protoss_Shuttle || unit->getType() == BWAPI::UnitTypes::Terran_Dropship)
			{
				transportUnits.push_back(unit);
			}
			// select ranged units
			else if ((unit->getType().groundWeapon().maxRange() > 32) || (unit->getType() == BWAPI::UnitTypes::Protoss_Reaver))
			{
				rangedUnits.push_back(unit);
			}
			// select melee units
			else if (unit->getType().groundWeapon().maxRange() <= 32)
			{
				meleeUnits.push_back(unit);
			}
		}
	}

	meleeManager.setUnits(meleeUnits);
	rangedManager.setUnits(rangedUnits);
	detectorManager.setUnits(detectorUnits);
	transportManager.setUnits(detectorUnits);

	// Extensions
	marineManager.setUnits(terranMarines);
	vultureManager.setUnits(terranVultures);
	wraithManager.setUnits(terranWraiths);
	bcManager.setUnits(terranBCs);
	tankManager.setUnits(terranTanks);
	goliathManager.setUnits(terranGoliaths);
	scienceVesselManager.setUnits(terranScienceVessels);
	// eof ext

}

// calculates whether or not to regroup
bool Squad::needsToRegroup()
{
	// Extension
	//BOOST_FOREACH(BWAPI::Unit* unit, units)
	//{
	//	if (unit->getType().isFlyer())
	//		return false;
	//}
	// eof Ext


	// if we are not attacking, never regroup
	if (units.empty() || (order.type != SquadOrder::Attack))
	{
		regroupStatus = std::string("\x04 No combat units available");
		return false;
	}

	// if we are DT rushing and we haven't lost a DT yet, no retreat!
	if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Protoss &&
		StrategyManager::Instance().getCurrentStrategy() == StrategyManager::ProtossDarkTemplar &&
		(BWAPI::Broodwar->self()->deadUnitCount(BWAPI::UnitTypes::Protoss_Dark_Templar) == 0))
	{
		regroupStatus = std::string("\x04 DARK TEMPLAR HOOOOO!");
		return false;
	}

	BWAPI::Unit * unitClosest = unitClosestToEnemy();

	if (!unitClosest)
	{
		regroupStatus = std::string("\x04 No closest unit");
		return false;
	}

	// Ext comment v
	//CombatSimulation sim;
	//sim.setCombatUnits(unitClosest->getPosition(), Options::Micro::COMBAT_REGROUP_RADIUS + InformationManager::Instance().lastFrameRegroup*300);
	//ScoreType score = sim.simulateCombat();
	ScoreType score = 0;

    bool retreat = score < 0;
    int switchTime = 100;
    bool waiting = false;

    // we should not attack unless 5 seconds have passed since a retreat
    if (retreat != lastRetreatSwitchVal)
    {
        if (retreat == false && (BWAPI::Broodwar->getFrameCount() - lastRetreatSwitch < switchTime))
        {
            waiting = true;
            retreat = lastRetreatSwitchVal;
        }
        else
        {
            waiting = false;
            lastRetreatSwitch = BWAPI::Broodwar->getFrameCount();
            lastRetreatSwitchVal = retreat;
        }
    }
	
	if (retreat)
	{
		regroupStatus = std::string("\x04 Retreat - simulation predicts defeat, score: " + boost::lexical_cast<std::string>(score));
	}
	else
	{
		regroupStatus = std::string("\x04 Attack - simulation predicts success, score:  " + boost::lexical_cast<std::string>(score));
	}


	// Extension
	if ((score > -500)
		&& (score < 0))
	{
		retreat = true;
	}
	else
	{
		retreat = false;
	}
	// eof ext

	return retreat;
}

void Squad::setSquadOrder(const SquadOrder & so)
{
	order = so;
}

bool Squad::unitNearEnemy(BWAPI::Unit * unit)
{
	assert(unit);

	UnitVector enemyNear;

	MapGrid::Instance().GetUnits(enemyNear, unit->getPosition(), 400, false, true);

	return enemyNear.size() > 0;
}

BWAPI::Position Squad::calcCenter()
{


	BWAPI::Position accum(0,0);
	BOOST_FOREACH(BWAPI::Unit * unit, units)
	{
		accum += unit->getPosition();
	}
	return BWAPI::Position(accum.x() / units.size(), accum.y() / units.size());
}

BWAPI::Position Squad::calcRegroupPosition()
{
	BWAPI::Position regroup(0,0);

	int minDist(100000);

	BOOST_FOREACH(BWAPI::Unit * unit, units)
	{
		if (!nearEnemy[unit])
		{
			int dist = unit->getDistance(order.position);
			if (dist < minDist)
			{
				minDist = dist;
				regroup = unit->getPosition();
			}
		}
	}

	if (regroup == BWAPI::Position(0,0))
	{
		return BWTA::getRegion(BWTA::getStartLocation(BWAPI::Broodwar->self())->getTilePosition())->getCenter();
	}
	else
	{
		return regroup;
	}
}

BWAPI::Unit * Squad::unitClosestToEnemy()
{
	BWAPI::Unit * closest = NULL;
	int closestDist = 100000;

	BOOST_FOREACH (BWAPI::Unit * unit, units)
	{
		if (unit->getType() == BWAPI::UnitTypes::Protoss_Observer)
		{
			continue;
		}

		// the distance to the order position
		int dist = MapTools::Instance().getGroundDistance(unit->getPosition(), order.position);

		if (dist != -1 && (!closest || dist < closestDist))
		{
			closest = unit;
			closestDist = dist;
		}
	}

	if (!closest)
	{
		BOOST_FOREACH (BWAPI::Unit * unit, units)
		{
			if (unit->getType() == BWAPI::UnitTypes::Protoss_Observer)
			{
				continue;
			}

			// the distance to the order position
			int dist = unit->getDistance(BWAPI::Position(BWAPI::Broodwar->enemy()->getStartLocation()));

			if (dist != -1 && (!closest || dist < closestDist))
			{
				closest = unit;
				closestDist = dist;
			}
		}
	}

	return closest;
}

int Squad::squadUnitsNear(BWAPI::Position p)
{
	int numUnits = 0;

	BOOST_FOREACH (BWAPI::Unit * unit, units)
	{
		if (unit->getDistance(p) < 600)
		{
			numUnits++;
		}
	}

	return numUnits;
}

bool Squad::squadObserverNear(BWAPI::Position p)
{
	BOOST_FOREACH (BWAPI::Unit * unit, units)
	{
		if (unit->getType().isDetector() && unit->getDistance(p) < 300)
		{
			return true;
		}
	}

	return false;
}

const UnitVector &Squad::getUnits() const	
{ 
	return units; 
} 

const SquadOrder & Squad::getSquadOrder()	const			
{ 
	return order; 
}
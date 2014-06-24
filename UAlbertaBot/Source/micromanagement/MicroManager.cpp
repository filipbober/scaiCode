#include "Common.h"
#include "MicroManager.h"

void MicroManager::setUnits(const UnitVector & u) 
{ 
	units = u; 
}

BWAPI::Position MicroManager::calcCenter() const
{
	assert(!units.empty());

	BWAPI::Position accum(0,0);
	BOOST_FOREACH(BWAPI::Unit * unit, units)
	{
		accum += unit->getPosition();
	}
	return BWAPI::Position(accum.x() / units.size(), accum.y() / units.size());
}

void MicroManager::execute(const SquadOrder & inputOrder)
{
	// Nothing to do if we have no units
	if(units.empty() || !(inputOrder.type == SquadOrder::Attack || inputOrder.type == SquadOrder::Defend))
	{
		//BWAPI::Broodwar->printf("Gots no units, fix shit up (%d)", order.type);
		return;
	}
	order = inputOrder;
	drawOrderText();

	// Discover enemies within region of interest
	UnitVector nearbyEnemies;

	// if the order is to defend, we only care about units in the radius of the defense
	if (order.type == order.Defend)
	{
		MapGrid::Instance().GetUnits(nearbyEnemies, order.position, 800, false, true);
	
	} // otherwise we want to see everything on the way
	else if (order.type == order.Attack) 
	{
		MapGrid::Instance().GetUnits(nearbyEnemies, order.position, 800, false, true);
		BOOST_FOREACH (BWAPI::Unit * unit, units) 
		{
			BWAPI::Unit * u = unit;
			BWAPI::UnitType t = u->getType();
			MapGrid::Instance().GetUnits(nearbyEnemies, unit->getPosition(), 800, false, true);
		}
	}

	// the following block of code attacks all units on the way to the order position
	// we want to do this if the order is attack, defend, or harass
	if (order.type == order.Attack || order.type == order.Defend) 
	{
        // if this is a worker defense force
        if (units.size() == 1 && units[0]->getType().isWorker())
        {
            executeMicro(nearbyEnemies);
        }
        // otherwise it is a normal attack force
        else
        {
             // remove enemy worker units unless they are in one of their occupied regions
            UnitVector workersRemoved;

            BOOST_FOREACH (BWAPI::Unit * enemyUnit, nearbyEnemies) 
		    {
                // if its not a worker add it to the targets
			    if (!enemyUnit->getType().isWorker())
                {
                    workersRemoved.push_back(enemyUnit);
                }
                // if it is a worker
                else
                {
                    BOOST_FOREACH(BWTA::Region * enemyRegion, InformationManager::Instance().getOccupiedRegions(BWAPI::Broodwar->enemy()))
                    {
                        // only add it if it's in their region
                        if (BWTA::getRegion(BWAPI::TilePosition(enemyUnit->getPosition())) == enemyRegion)
                        {
                            workersRemoved.push_back(enemyUnit);
                        }
                    }
                }
		    }

		    // Allow micromanager to handle enemies
		    executeMicro(workersRemoved);
        }
	}	
}

void MicroManager::regroup(const BWAPI::Position & regroupPosition) const
{
	// for each of the units we have
	BOOST_FOREACH (BWAPI::Unit * unit, units)
	{
		// if the unit is outside the regroup area
		if (unit->getDistance(regroupPosition) > 100)
		{
			// regroup it
			BWAPI::Broodwar->drawCircleMap(unit->getPosition().x(), unit->getPosition().y(), 20, BWAPI::Colors::Yellow);
			smartMove(unit, regroupPosition);
		}
		else
		{
			smartAttackMove(unit, unit->getPosition());
		}
	}
}

bool MicroManager::unitNearEnemy(BWAPI::Unit * unit)
{
	assert(unit);

	UnitVector enemyNear;

	MapGrid::Instance().GetUnits(enemyNear, unit->getPosition(), 800, false, true);

	return enemyNear.size() > 0;
}

// returns true if position:
// a) is walkable
// b) doesn't have buildings on it
// c) doesn't have a unit on it that can attack ground
bool MicroManager::checkPositionWalkable(BWAPI::Position pos) {

	// get x and y from the position
	int x(pos.x()), y(pos.y());

	// walkable tiles exist every 8 pixels
	bool good = BWAPI::Broodwar->isWalkable(x/8, y/8);
	
	// if it's not walkable throw it out
	if (!good) return false;
	
	// for each of those units, if it's a building or an attacking enemy unit we don't want to go there
	BOOST_FOREACH (BWAPI::Unit * unit, BWAPI::Broodwar->getUnitsOnTile(x/32, y/32)) 
	{
		if	(unit->getType().isBuilding() || unit->getType().isResourceContainer() || 
			(unit->getPlayer() != BWAPI::Broodwar->self() && unit->getType().groundWeapon() != BWAPI::WeaponTypes::None)) 
		{		
				return false;
		}
	}

	// otherwise it's okay
	return true;
}

void MicroManager::smartAttackUnit(BWAPI::Unit * attacker, BWAPI::Unit * target) const
{
	assert(attacker && target);

	// if we have issued a command to this unit already this frame, ignore this one
	if (attacker->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount() || attacker->isAttackFrame())
	{
		return;
	}

	// get the unit's current command
	BWAPI::UnitCommand currentCommand(attacker->getLastCommand());

	// if we've already told this unit to attack this target, ignore this command
	if (currentCommand.getType() == BWAPI::UnitCommandTypes::Attack_Unit &&	currentCommand.getTarget() == target)
	{
	

		return;
	}

	// if nothing prevents it, attack the target
	attacker->attack(target);

	if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawLineMap(	attacker->getPosition().x(), attacker->getPosition().y(),
									target->getPosition().x(), target->getPosition().y(),
									BWAPI::Colors::Red );

}

void MicroManager::smartAttackMove(BWAPI::Unit * attacker, BWAPI::Position targetPosition) const
{
	assert(attacker);

	// if we have issued a command to this unit already this frame, ignore this one
	if (attacker->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount() || attacker->isAttackFrame())
	{
		return;
	}

	// get the unit's current command
	BWAPI::UnitCommand currentCommand(attacker->getLastCommand());

	// if we've already told this unit to attack this target, ignore this command
	if (currentCommand.getType() == BWAPI::UnitCommandTypes::Attack_Move &&	currentCommand.getTargetPosition() == targetPosition)
	{
		return;
	}

	// if nothing prevents it, attack the target
	attacker->attack(targetPosition);

	if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawLineMap(	attacker->getPosition().x(), attacker->getPosition().y(),
									targetPosition.x(), targetPosition.y(),
									BWAPI::Colors::Orange );
}


// Legacy code. WaypointCreatorExt is responsible for border movement
void MicroManager::smartBorderMove(BWAPI::Unit * attacker, BWAPI::Position targetPosition) const
{
	assert(attacker);
	 
	// if we have issued a command to this unit already this frame, ignore this one
	if (attacker->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount() || attacker->isAttackFrame())
	{
		return;
	}

	// 1. Go to the closest border
	// 2. Go either clockwise or counter clockwise
	int distFromBorder = 50;

	int mapWidth = BWAPI::Broodwar->mapWidth() * TILE_SIZE;		// Map width in Position units
	int mapHeight = BWAPI::Broodwar->mapHeight() * TILE_SIZE;	// Map height in Position units

	

	//BWAPI::Position borderN = BWAPI::Position(attacker->getPosition().x, mapHeight);
	//BWAPI::Position borderE = BWAPI::Position(0, attacker->getPosition().y);
	//BWAPI::Position borderS = BWAPI::Position(attacker->getPosition().x, 0);
	//BWAPI::Position borderW = BWAPI::Position(mapWidth, attacker->getPosition().y);

	BWAPI::Position borders[4]; // NESW
	borders[0] = BWAPI::Position(attacker->getPosition().x(), mapHeight);
	borders[1] = BWAPI::Position(0, attacker->getPosition().y());
	borders[2] = BWAPI::Position(attacker->getPosition().x(), 0);
	borders[3] = BWAPI::Position(mapWidth, attacker->getPosition().y());

	BWAPI::Position enemyBorders[4]; // NESW
	enemyBorders[0] = BWAPI::Position(targetPosition.x(), mapHeight);
	enemyBorders[1] = BWAPI::Position(0, targetPosition.y());
	enemyBorders[2] = BWAPI::Position(targetPosition.x(), 0);
	enemyBorders[3] = BWAPI::Position(mapWidth, targetPosition.y());

	int closestAttackerBorderDist = INT_MAX;
	int closestEnemyBorderDist = INT_MAX;

	int attackerBorderId = 0;
	int enemyBorderId = 0;

	bool isClosestBorderReached = false;
	// Check distance to every border and choose the closest
	// for the attacker and enemy.
	// Ignore borders that are already close enough
	for (int i = 0; i < 4; i++)
	{
		int currentBorderDistance = attacker->getDistance(borders[i]);
		if ((currentBorderDistance < closestAttackerBorderDist))
		{
			closestAttackerBorderDist = currentBorderDistance;
			attackerBorderId = i;
			if (currentBorderDistance < distFromBorder)
			{
				isClosestBorderReached = true;
			}

			//closestCurrentAttackerBorder = borders[i];
			//attackerBorderId = i;
			//if ((attacker->getDistance(borders[i]) < distFromBorder))
			//{
			//	closestNextAttackerBorder = borders[i];
			//}
		}

		int currentEnemyBorderDistance = targetPosition.getDistance(enemyBorders[i]);
		if ((currentEnemyBorderDistance < closestEnemyBorderDist))
		{
			closestEnemyBorderDist = currentEnemyBorderDistance;
			enemyBorderId = i;
		}
	}



	bool isClockwise = true;
	int clockwiseDist = clockwiseSteps(attackerBorderId, enemyBorderId);
	int counterclockwiseDist = counterclockwiseSteps(attackerBorderId, enemyBorderId);
	int stepDistance = 0;

	if (clockwiseSteps(attackerBorderId, enemyBorderId) <= counterclockwiseSteps(attackerBorderId, enemyBorderId))
	{
		isClockwise = true;
		stepDistance = clockwiseSteps(attackerBorderId, enemyBorderId);
	}
	else
	{
		isClockwise = false;
		stepDistance = counterclockwiseSteps(attackerBorderId, enemyBorderId);
	}

	// If we are near closest enemy border
	BWAPI::Position nextPosition;
	if (stepDistance == 0)
	{
		//nextPosition = enemyBorders[enemyBorderId];
		nextPosition = targetPosition;
	}
	else
	{
		if (isClosestBorderReached)
		{
			if (isClockwise)
			{
				nextPosition = borders[nextClockwiseBorderIdx(attackerBorderId)];
			}
			else
			{
				nextPosition = borders[nextCounterclockwiseBorderIdx(attackerBorderId)];
			}
		}
		else
		{
			nextPosition = borders[attackerBorderId];
		}
		
	}

	BWAPI::Broodwar->setFrameSkip(-1);
	if (BWAPI::Broodwar->getFrameCount() % 240 == 0)
	{
		BWAPI::Broodwar->setFrameSkip(-1);
		BWAPI::Broodwar->printf("                                           DebExt: isClosestBorderReached = %d", isClosestBorderReached);
		BWAPI::Broodwar->printf("                                           DebExt: isClockwise = %d", isClockwise);
		BWAPI::Broodwar->printf("                                           DebExt: nextClockwise = %d", nextClockwiseBorderIdx(attackerBorderId));
		BWAPI::Broodwar->printf("                                           DebExt: nextCounterclockwise = %d", nextCounterclockwiseBorderIdx(attackerBorderId));
		BWAPI::Broodwar->printf("                                           DebExt: attackerBorderId = %d", attackerBorderId);
		BWAPI::Broodwar->printf("                                           DebExt: enemyBorderId = %d", enemyBorderId);
		BWAPI::Broodwar->printf("                                           DebExt: clockwiseDist = %d", clockwiseDist);
		BWAPI::Broodwar->printf("                                           DebExt: coutnerclockwiseDist = %d", counterclockwiseDist);
	}

	if (!nextPosition.isValid())
	{
		nextPosition.makeValid();
	}

	smartMove(attacker, nextPosition);










	//BWAPI::Position nextPosition;
	//// If we reached enemy border
	//if (enemyBorderId == attackerBorderId)
	//{
	//	if (attacker->getDistance(enemyBorders[enemyBorderId]) < distFromBorder)
	//	{
	//		smartAttackMove(attacker, targetPosition);
	//	}
	//}	
	//else if (isClockwise)
	//{
	//	int nextBorderId = nextClockwiseBorderIdx(attackerBorderId);
	//	nextPosition = borders[nextBorderId];
	//
	//}
	//else
	//{
	//	int nextBorderId = nextCounterclockwiseBorderIdx(attackerBorderId);
	//	nextPosition = borders[nextBorderId];
	//}


	//smartMove(attacker, nextPosition);
	
	//BWAPI::Position movePosition = closestAttackerBorder;


}

int MicroManager::nextClockwiseBorderIdx(int borderId) const
{
	if (borderId == 3)
		return 0;
	else
		return borderId + 1;
}

int MicroManager::nextCounterclockwiseBorderIdx(int borderId) const
{
	if (borderId == 0)
		return 3;
	else
		return borderId - 1;
}

int MicroManager::clockwiseSteps(int attackerBorder, int enemyBorder) const
{
	int steps = 0;

	while (attackerBorder != enemyBorder)
	{
		steps++;
		attackerBorder = nextClockwiseBorderIdx(attackerBorder);
	}
	return steps;
}

int MicroManager::counterclockwiseSteps(int attackerBorder, int enemyBorder) const
{
	int steps = 0;

	while (attackerBorder != enemyBorder)
	{
		steps++;
		attackerBorder = nextCounterclockwiseBorderIdx(attackerBorder);
	}
	return steps;
}


void MicroManager::smartMove(BWAPI::Unit * attacker, BWAPI::Position targetPosition) const
{
	assert(attacker);

	// if we have issued a command to this unit already this frame, ignore this one
	if (attacker->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount() || attacker->isAttackFrame())
	{
		if (attacker->isSelected())
		{
			BWAPI::Broodwar->printf("Attack Frame");
		}
		return;
	}

	// get the unit's current command
	BWAPI::UnitCommand currentCommand(attacker->getLastCommand());

	// if we've already told this unit to attack this target, ignore this command
	if (   (currentCommand.getType() == BWAPI::UnitCommandTypes::Move)
		&& (currentCommand.getTargetPosition() == targetPosition) 
		&& (BWAPI::Broodwar->getFrameCount() - attacker->getLastCommandFrame() < 5)
		&& attacker->isMoving())
	{
		if (attacker->isSelected())
		{
			BWAPI::Broodwar->printf("Previous Command Frame=%d Pos=(%d, %d)", attacker->getLastCommandFrame(), currentCommand.getTargetPosition().x(), currentCommand.getTargetPosition().y());
		}
		return;
	}

	// if nothing prevents it, attack the target
	attacker->move(targetPosition);

	if (Options::Debug::DRAW_UALBERTABOT_DEBUG) 
	{
		BWAPI::Broodwar->drawLineMap(attacker->getPosition().x(), attacker->getPosition().y(),
									 targetPosition.x(), targetPosition.y(), BWAPI::Colors::Orange);
	}
}

void MicroManager::smartGroupMove(BWAPI::Unit * attacker, BWAPI::Position targetPosition, int proximity, UnitVector& unitsGroup) const
{
	assert(attacker);

	// if we have issued a command to this unit already this frame, ignore this one
	if (attacker->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount() || attacker->isAttackFrame())
	{
		if (attacker->isSelected())
		{
			BWAPI::Broodwar->printf("Attack Frame");
		}
		return;
	}

	// get the unit's current command
	BWAPI::UnitCommand currentCommand(attacker->getLastCommand());

	// if we've already told this unit to attack this target, ignore this command
	if ((currentCommand.getType() == BWAPI::UnitCommandTypes::Move)
		&& (currentCommand.getTargetPosition() == targetPosition)
		&& (BWAPI::Broodwar->getFrameCount() - attacker->getLastCommandFrame() < 5)
		&& attacker->isMoving())
	{
		if (attacker->isSelected())
		{
			BWAPI::Broodwar->printf("Previous Command Frame=%d Pos=(%d, %d)", attacker->getLastCommandFrame(), currentCommand.getTargetPosition().x(), currentCommand.getTargetPosition().y());
		}
		return;
	}

	// if nothing prevents it, attack the target

	// Group movement



	attacker->move(targetPosition);
	// eof group movement

	if (Options::Debug::DRAW_UALBERTABOT_DEBUG)
	{
		BWAPI::Broodwar->drawLineMap(attacker->getPosition().x(), attacker->getPosition().y(),
			targetPosition.x(), targetPosition.y(), BWAPI::Colors::Orange);
	}
}

void MicroManager::trainSubUnits(BWAPI::Unit * unit) const
{
	if (unit->getType() == BWAPI::UnitTypes::Protoss_Reaver)
	{
		unit->train(BWAPI::UnitTypes::Protoss_Scarab);
	}
	else if (unit->getType() == BWAPI::UnitTypes::Protoss_Carrier)
	{
		unit->train(BWAPI::UnitTypes::Protoss_Interceptor);
	}
}

bool MicroManager::unitNearChokepoint(BWAPI::Unit * unit) const
{
	BOOST_FOREACH (BWTA::Chokepoint * choke, BWTA::getChokepoints())
	{
		if (unit->getDistance(choke->getCenter()) < 80)
		{
			return true;
		}
	}

	return false;
}

void MicroManager::drawOrderText() {

	BOOST_FOREACH (BWAPI::Unit * unit, units) {
		if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextMap(unit->getPosition().x(), unit->getPosition().y(), "%s", order.getStatus().c_str());
	}
}

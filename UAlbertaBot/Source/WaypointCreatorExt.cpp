#include "WaypointCreatorExt.h"




WaypointCreatorExt::WaypointCreatorExt()
{
}


WaypointCreatorExt::~WaypointCreatorExt()
{
}

void WaypointCreatorExt::setBorderMoveWaypoints(BWAPI::Unit* attacker, BWAPI::Position targetPosition)
{
	// for testing purposes:
	//std::vector<BWAPI::Position> waypoints;
	//waypoints.push_back(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation().x(), BWAPI::Broodwar->self()->getStartLocation().y()));
	//waypoints.push_back(targetPosition);




	UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(attacker);

	BWAPI::Position pos = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation().x(), BWAPI::Broodwar->self()->getStartLocation().y());
	if (!pos.isValid())
	{
		pos.makeValid();
	}
	//unitData->pushWaypoint(pos);
	unitData->pushWaypoint(targetPosition);
	unitData->setDestination(targetPosition);

	// 

	//assert(attacker);

	//// if we have issued a command to this unit already this frame, ignore this one
	//if (attacker->getLastCommandFrame() >= BWAPI::Broodwar->getFrameCount() || attacker->isAttackFrame())
	//{
	//	return;
	//}

	//// 1. Go to the closest border
	//// 2. Go either clockwise or counter clockwise
	//int distFromBorder = 50;

	//int mapWidth = BWAPI::Broodwar->mapWidth() * TILE_SIZE;		// Map width in Position units
	//int mapHeight = BWAPI::Broodwar->mapHeight() * TILE_SIZE;	// Map height in Position units



	////BWAPI::Position borderN = BWAPI::Position(attacker->getPosition().x, mapHeight);
	////BWAPI::Position borderE = BWAPI::Position(0, attacker->getPosition().y);
	////BWAPI::Position borderS = BWAPI::Position(attacker->getPosition().x, 0);
	////BWAPI::Position borderW = BWAPI::Position(mapWidth, attacker->getPosition().y);

	//BWAPI::Position borders[4]; // NESW
	//borders[0] = BWAPI::Position(attacker->getPosition().x(), mapHeight);
	//borders[1] = BWAPI::Position(0, attacker->getPosition().y());
	//borders[2] = BWAPI::Position(attacker->getPosition().x(), 0);
	//borders[3] = BWAPI::Position(mapWidth, attacker->getPosition().y());

	//BWAPI::Position enemyBorders[4]; // NESW
	//enemyBorders[0] = BWAPI::Position(targetPosition.x(), mapHeight);
	//enemyBorders[1] = BWAPI::Position(0, targetPosition.y());
	//enemyBorders[2] = BWAPI::Position(targetPosition.x(), 0);
	//enemyBorders[3] = BWAPI::Position(mapWidth, targetPosition.y());

	//int closestAttackerBorderDist = INT_MAX;
	//int closestEnemyBorderDist = INT_MAX;

	//int attackerBorderId = 0;
	//int enemyBorderId = 0;

	//bool isClosestBorderReached = false;
	//// Check distance to every border and choose the closest
	//// for the attacker and enemy.
	//// Ignore borders that are already close enough
	//for (int i = 0; i < 4; i++)
	//{
	//	int currentBorderDistance = attacker->getDistance(borders[i]);
	//	if ((currentBorderDistance < closestAttackerBorderDist))
	//	{
	//		closestAttackerBorderDist = currentBorderDistance;
	//		attackerBorderId = i;
	//		if (currentBorderDistance < distFromBorder)
	//		{
	//			isClosestBorderReached = true;
	//		}

	//		//closestCurrentAttackerBorder = borders[i];
	//		//attackerBorderId = i;
	//		//if ((attacker->getDistance(borders[i]) < distFromBorder))
	//		//{
	//		//	closestNextAttackerBorder = borders[i];
	//		//}
	//	}

	//	int currentEnemyBorderDistance = targetPosition.getDistance(enemyBorders[i]);
	//	if ((currentEnemyBorderDistance < closestEnemyBorderDist))
	//	{
	//		closestEnemyBorderDist = currentEnemyBorderDistance;
	//		enemyBorderId = i;
	//	}
	//}



	//bool isClockwise = true;
	//int clockwiseDist = clockwiseSteps(attackerBorderId, enemyBorderId);
	//int counterclockwiseDist = counterclockwiseSteps(attackerBorderId, enemyBorderId);
	//int stepDistance = 0;

	//if (clockwiseSteps(attackerBorderId, enemyBorderId) <= counterclockwiseSteps(attackerBorderId, enemyBorderId))
	//{
	//	isClockwise = true;
	//	stepDistance = clockwiseSteps(attackerBorderId, enemyBorderId);
	//}
	//else
	//{
	//	isClockwise = false;
	//	stepDistance = counterclockwiseSteps(attackerBorderId, enemyBorderId);
	//}

	//// If we are near closest enemy border
	//BWAPI::Position nextPosition;
	//if (stepDistance == 0)
	//{
	//	//nextPosition = enemyBorders[enemyBorderId];
	//	nextPosition = targetPosition;
	//}
	//else
	//{
	//	if (isClosestBorderReached)
	//	{
	//		if (isClockwise)
	//		{
	//			nextPosition = borders[nextClockwiseBorderIdx(attackerBorderId)];
	//		}
	//		else
	//		{
	//			nextPosition = borders[nextCounterclockwiseBorderIdx(attackerBorderId)];
	//		}
	//	}
	//	else
	//	{
	//		nextPosition = borders[attackerBorderId];
	//	}

	//}

	//BWAPI::Broodwar->setFrameSkip(-1);
	//if (BWAPI::Broodwar->getFrameCount() % 240 == 0)
	//{
	//	BWAPI::Broodwar->setFrameSkip(-1);
	//	BWAPI::Broodwar->printf("                                           DebExt: isClosestBorderReached = %d", isClosestBorderReached);
	//	BWAPI::Broodwar->printf("                                           DebExt: isClockwise = %d", isClockwise);
	//	BWAPI::Broodwar->printf("                                           DebExt: nextClockwise = %d", nextClockwiseBorderIdx(attackerBorderId));
	//	BWAPI::Broodwar->printf("                                           DebExt: nextCounterclockwise = %d", nextCounterclockwiseBorderIdx(attackerBorderId));
	//	BWAPI::Broodwar->printf("                                           DebExt: attackerBorderId = %d", attackerBorderId);
	//	BWAPI::Broodwar->printf("                                           DebExt: enemyBorderId = %d", enemyBorderId);
	//	BWAPI::Broodwar->printf("                                           DebExt: clockwiseDist = %d", clockwiseDist);
	//	BWAPI::Broodwar->printf("                                           DebExt: coutnerclockwiseDist = %d", counterclockwiseDist);
	//}

	//if (!nextPosition.isValid())
	//{
	//	nextPosition.makeValid();
	//}

	//smartMove(attacker, nextPosition);
}

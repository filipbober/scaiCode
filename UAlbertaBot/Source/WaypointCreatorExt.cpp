#include "WaypointCreatorExt.h"




WaypointCreatorExt::WaypointCreatorExt()
{
}


WaypointCreatorExt::~WaypointCreatorExt()
{
}

void WaypointCreatorExt::setBorderMoveWaypoints(BWAPI::Unit* attacker, BWAPI::Position targetPosition)
{
	UnitDataExt* unitData = UnitManagerExt::Instance().getUnitData(attacker);
	
	// Set the destination
	unitData->setDestination(targetPosition);

	int distFromBorder = 10;
	BWAPI::Position attackerPosition = attacker->getPosition();
	BWAPI::Position attackerBorders[4];			// NESW
	BWAPI::Position targetPositionBorders[4];			// NESW

	attackerBorders[0] = BWAPI::Position(attackerPosition.x(), BWAPI::Broodwar->mapHeight() - distFromBorder);		// N
	attackerBorders[1] = BWAPI::Position(distFromBorder, attackerPosition.y());										// E
	attackerBorders[2] = BWAPI::Position(attackerPosition.x(), distFromBorder);										// S
	attackerBorders[3] = BWAPI::Position(BWAPI::Broodwar->mapWidth() - distFromBorder, attackerPosition.y());		// W

	targetPositionBorders[0] = BWAPI::Position(targetPosition.x(), BWAPI::Broodwar->mapHeight() - distFromBorder);			// N
	targetPositionBorders[1] = BWAPI::Position(distFromBorder, targetPosition.y());											// E
	targetPositionBorders[2] = BWAPI::Position(targetPosition.x(), distFromBorder);											// S
	targetPositionBorders[3] = BWAPI::Position(BWAPI::Broodwar->mapWidth() - distFromBorder, targetPosition.y());			// W

	BWAPI::Position borderWaypoints[8];
	setMapBorderWaypoints(borderWaypoints, distFromBorder);

	// Nearest border from the target
	BWAPI::Position targetBorderPosition = targetPositionBorders[closestBorderId(targetPosition, targetPositionBorders)];

	// Nearest waypoint from the attacker position
	int attackerClosestWaypointId = getClosestBorderWaypointId(attackerPosition, borderWaypoints);

	int attackerNextWaypointId = getNextBorderWaypointId(attackerClosestWaypointId);
	int attackerPreviousWaypointId = getPreviousBorderWaypointId(attackerClosestWaypointId);

	// Measure if its better to go clockwise or counterclockwise
	double clockwiseDistance = borderWaypoints[attackerNextWaypointId].getDistance(targetBorderPosition);
	double counterclockwiseDistance = borderWaypoints[attackerPreviousWaypointId].getDistance(targetBorderPosition);

	bool isClockwise = (clockwiseDistance < counterclockwiseDistance) ? true : false;

	// Push waypoints

	// Push closest enemy border
	unitData->pushWaypoint(targetBorderPosition);

	// Push either clockwise or counterclockwise until closest attacker waypoint is reached
	BWAPI::Position currentWaypoint;
	int currentWaypointId = getClosestBorderWaypointId(targetBorderPosition, borderWaypoints);

	// Push closest enemy waypoint
	currentWaypoint = borderWaypoints[currentWaypointId];
	unitData->pushWaypoint(currentWaypoint);

	if (isClockwise)
	{
		while (currentWaypointId != attackerClosestWaypointId)
		{
			currentWaypointId = getNextBorderWaypointId(currentWaypointId);
			currentWaypoint = borderWaypoints[currentWaypointId];
		}
	}
	else
	{
		while (currentWaypointId != attackerClosestWaypointId)
		{
			currentWaypointId = getPreviousBorderWaypointId(currentWaypointId);
			currentWaypoint = borderWaypoints[currentWaypointId];
		}
	}

	// Push closest attacker waypoint
	unitData->pushWaypoint(borderWaypoints[attackerClosestWaypointId]);
}

int WaypointCreatorExt::closestBorderId(BWAPI::Position mapPostition, BWAPI::Position borders[], int size)
{
	int id = 0;
	double closestDistance = DBL_MAX;	

	for (int i = 0; i < size; i++)
	{
		int currentDistance = mapPostition.getDistance(borders[i]);
		if (currentDistance < closestDistance)
		{
			closestDistance = currentDistance;
			id = i;
		}
	}

	return id;

}

int WaypointCreatorExt::nextClockwiseId(int borderId)
{
	if (borderId == 3)
		return 0;
	else
		return borderId + 1;
}

int WaypointCreatorExt::nextCounterclockwiseId(int borderId)
{
	if (borderId == 0)
		return 3;
	else
		return borderId - 1;
}

void WaypointCreatorExt::setMapBorderWaypoints(BWAPI::Position borderWaypoints[], int distFromBorder, int size)
{
	// From top-left clockwise

	borderWaypoints[0] = BWAPI::Position(distFromBorder, BWAPI::Broodwar->mapHeight() - distFromBorder);
	borderWaypoints[1] = BWAPI::Position(BWAPI::Broodwar->mapWidth() / 2, BWAPI::Broodwar->mapHeight() - distFromBorder);
	borderWaypoints[2] = BWAPI::Position(BWAPI::Broodwar->mapWidth() - distFromBorder, BWAPI::Broodwar->mapHeight() - distFromBorder);
	borderWaypoints[3] = BWAPI::Position(BWAPI::Broodwar->mapWidth() - distFromBorder, BWAPI::Broodwar->mapHeight() / 2);
	borderWaypoints[4] = BWAPI::Position(BWAPI::Broodwar->mapWidth() - distFromBorder, distFromBorder);
	borderWaypoints[5] = BWAPI::Position(BWAPI::Broodwar->mapWidth()/2, distFromBorder);
	borderWaypoints[6] = BWAPI::Position(distFromBorder, distFromBorder);
	borderWaypoints[7] = BWAPI::Position(distFromBorder, BWAPI::Broodwar->mapHeight() / 2);
}

int WaypointCreatorExt::getClosestBorderWaypointId(BWAPI::Position mapPostition, BWAPI::Position borderWaypoints[], int size)
{
	double minDistance = DBL_MAX;
	int id;

	for (int i = 0; i < size; i++)
	{
		double currentDistance = mapPostition.getDistance(borderWaypoints[i]);
		if (currentDistance < minDistance)
		{
			minDistance = currentDistance;
			id = i;
		}
	}

	return id;
}

int WaypointCreatorExt::getNextBorderWaypointId(int currentWaypointId)
{
	if (currentWaypointId == 7)
		return 0;
	else
		return currentWaypointId + 1;
}

int WaypointCreatorExt::getPreviousBorderWaypointId(int currentWaypointId)
{
	if (currentWaypointId == 0)
		return 7;
	else
		return currentWaypointId - 1;
}
#include "Common.h"
#include "ProductionManager.h"

#include "..\..\StarcraftBuildOrderSearch\Source\starcraftsearch\ActionSet.hpp"
#include "..\..\StarcraftBuildOrderSearch\Source\starcraftsearch\DFBBStarcraftSearch.hpp"
#include "..\..\StarcraftBuildOrderSearch\Source\starcraftsearch\StarcraftData.hpp"

#define BOADD(N, T, B) for (int i=0; i<N; ++i) { queue.queueAsLowestPriority(MetaType(T), B); }

#define GOAL_ADD(G, M, N) G.push_back(std::pair<MetaType, int>(M, N))

#include "extensions\BuildOrderTest.h"
#include "extensions\BuildOrderCreator.h"
#include "extensions\ZergBuildOrderCreator.h"
#include "..\QueueConstructorExt.h"


ProductionManager::ProductionManager()
: initialBuildSet(false)
, reservedMinerals(0)
, reservedGas(0)
, assignedWorkerForThisBuilding(false)
, haveLocationForThisBuilding(false)
, enemyCloakedDetected(false)
, rushDetected(false)
, isBuildOrderSearchOn(false)		// ext
, _lastType(MetaType(BWAPI::UnitTypes::Unknown), 1, true)
{
	populateTypeCharMap();

	if (!Options::Modules::USING_BUILD_LEARNER && !Options::Modules::USING_BUILD_ORDER_DEMO)
	{
		setBuildOrder(StarcraftBuildOrderSearchManager::Instance().getOpeningBuildOrder());

		// Extension

		
		//BuildOrderTest buildOrder;
		//setBuildOrder(buildOrder.GenerateTestQueue());

		// Create base build order class
		//BuildOrderCreator* buildOrder = new ZergBuildOrderCreator();

		// Set build order to active build
		//setBuildOrder(buildOrder->GenerateQueue());

		// Free memory
		//delete buildOrder;

		// eof Extension
	}
}

void ProductionManager::setBuildOrder(const std::vector<MetaType> & buildOrder)
{
	// clear the current build order
	queue.clearAll();

	// for each item in the results build order, add it
	for (size_t i(0); i<buildOrder.size(); ++i)
	{
		// queue the item
		queue.queueAsLowestPriority(buildOrder[i], true);
	}
}

void ProductionManager::performBuildOrderSearch(const std::vector< std::pair<MetaType, UnitCountType> > & goal)
{	
	std::vector<MetaType> buildOrder = StarcraftBuildOrderSearchManager::Instance().findBuildOrder(goal);

	// Ext
	// Terran specific constraints
	for (int i = 0; i < buildOrder.size(); i++)
	{
		if ((buildOrder[i].isBuilding()))
		{
			if ((buildOrder[i].type == (buildOrder[i].type == BWAPI::UnitTypes::Terran_Barracks))
				|| (buildOrder[i].type == (buildOrder[i].type == BWAPI::UnitTypes::Terran_Factory))
				|| (buildOrder[i].type == (buildOrder[i].type == BWAPI::UnitTypes::Terran_Starport))
				)
			{

				//// If there are already 4 buildings of the same type
				//// remove them from queue
				//if ((BWAPI::Broodwar->self()->allUnitCount(buildOrder[i].type) > 3))
				//{
				//	buildOrder.erase(buildOrder.begin() + i);
				//}
			}
		}
	}

	// eof Ext

	// set the build order
	setBuildOrder(buildOrder);
}

void ProductionManager::setSearchGoal(MetaPairVector & goal)
{
	searchGoal = goal;
}

void ProductionManager::update() 
{
	// check the queue for stuff we can build
	manageBuildOrderQueue();

	if (Options::Modules::USING_BUILD_LEARNER)
	{
		if (queue.size() == 0)
		{
			queue.queueAsHighestPriority(buildLearner.getRandomLegalAction(), true);
		}

		return;
	}

	if ((queue.size() == 0) && Options::Modules::USING_BUILD_ORDER_DEMO)
	{
		performBuildOrderSearch(searchGoal);
	}

	// if nothing is currently building, get a new goal from the strategy manager
	if ((queue.size() == 0) && (BWAPI::Broodwar->getFrameCount() > 10) && !Options::Modules::USING_BUILD_ORDER_DEMO)
	{
		// Extension
		if (!StrategyManager::Instance().isMidGame)
		{
			// If it is not mid game yet (just finished build order), set midgame falg to true
			StrategyManager::Instance().isMidGame = true;
			BWAPI::Broodwar->printf("Midgame started!");
		}		
		// eof Extension

		BWAPI::Broodwar->drawTextScreen(150, 10, "Nothing left to build, new search!");

		// Ext - commented
		if (isBuildOrderSearchOn)
		{
			const std::vector< std::pair<MetaType, UnitCountType> > newGoal = StrategyManager::Instance().getBuildOrderGoal();	// ext
			performBuildOrderSearch(newGoal);																					// ext
		}
		// eof ext comment
	}

	//// detect if there's a build order deadlock once per second
	if ((BWAPI::Broodwar->getFrameCount() % 24 == 0) && detectBuildOrderDeadlock())
	{
		BWAPI::Broodwar->printf("Supply deadlock detected, building pylon!");
		queue.queueAsHighestPriority(MetaType(BWAPI::Broodwar->self()->getRace().getSupplyProvider()), true);
	}

	// if they have cloaked units get a new goal asap (for Protoss)
	if (!enemyCloakedDetected && InformationManager::Instance().enemyHasCloakedUnits())
	{
		if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Protoss)
		{
			if (BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Photon_Cannon) < 2)
			{
				queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Protoss_Photon_Cannon), true);
				queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Protoss_Photon_Cannon), true);
			}

			if (BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Protoss_Forge) == 0)
			{
				queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Protoss_Forge), true);
			}
		}
		else if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Terran)
		{
			if (BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Missile_Turret) < 2)
			{
				queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Missile_Turret), true);
				queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Missile_Turret), true);
			}

			if (BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay) == 0)
			{
				queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Engineering_Bay), true);
			}

		}
		else if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Zerg)
		{
			// TODO: Implement
		}
		BWAPI::Broodwar->printf("Enemy Cloaked Unit Detected!");
		enemyCloakedDetected = true;
	}


//	if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextScreen(447, 17, "\x07 %d", BuildingManager::Instance().getReservedMinerals());
}

// on unit destroy
void ProductionManager::onUnitDestroy(BWAPI::Unit * unit)
{
	// we don't care if it's not our unit
	if (!unit || unit->getPlayer() != BWAPI::Broodwar->self())
	{
		return;
	}
		
	if (Options::Modules::USING_MACRO_SEARCH && isBuildOrderSearchOn)
	{
		// if it's a worker or a building, we need to re-search for the current goal
		if ((unit->getType().isWorker() && !WorkerManager::Instance().isWorkerScout(unit)) || unit->getType().isBuilding())
		{
			BWAPI::Broodwar->printf("Critical unit died, re-searching build order");

			if (unit->getType() != BWAPI::UnitTypes::Zerg_Drone)
			{
				performBuildOrderSearch(StrategyManager::Instance().getBuildOrderGoal());
			}
		}
	}
}

void ProductionManager::manageBuildOrderQueue() 
{
	// if there is nothing in the queue, oh well
	// Extension
	static int lastSupply;// = BWAPI::Broodwar->self()->supplyUsed();
	static int lastMineralPrice;
	if (StrategyManager::Instance().isMidGame
		&& (BWAPI::Broodwar->getFrameCount() % 3000 == 0)
		//&& (BWAPI::Broodwar->self()->supplyUsed() <= lastSupply)
		&& !queue.isEmpty()
		&& queue.getHighestPriorityItem().metaType.mineralPrice() == lastMineralPrice)
	{
		BWAPI::Broodwar->printf("                                           DebExt: last supply = %d", lastSupply);
		queue.removeCurrentHighestPriorityItem();
	}
	else if (!queue.isEmpty())
	{
		lastSupply = BWAPI::Broodwar->self()->supplyUsed();
		lastMineralPrice = queue.getHighestPriorityItem().metaType.mineralPrice();
	}

	//if ((BWAPI::Broodwar->self()->minerals() > 1500)
	//	&& (BWAPI::Broodwar->getFrameCount() % 3000 == 0))
	//{
	//	queue.clearAll();
	//}

	if //((BWAPI::Broodwar->getFrameCount() > 10000)
		((BWAPI::Broodwar->getFrameCount() > 6000)
		&& (StrategyManager::Instance().isMidGame)
		&& (BWAPI::Broodwar->getFrameCount() % 48 == 0))
	{
		manageIdleProduction();
	}


	if (queue.isEmpty()) 
	{		
		queueDoSomething();
		return;

		//if (BWAPI::Broodwar->getFrameCount() % 240 == 0)
		//{
		//	queueDoSomething();
		//	return;
		//}
		//else
		//{
		//	return;
		//}
	}

	//if ((BWAPI::Broodwar->getFrameCount() > 10000)
	//	&& (StrategyManager::Instance().isMidGame)
	//	&& (BWAPI::Broodwar->getFrameCount() % 48 == 0))
	//{
	//	manageIdleProduction();
	//}

	//// Detect if a unit is not blocking the queue
	BuildOrderItem<PRIORITY_TYPE>& highestQueueItem = queue.getHighestPriorityItem();
	//if ( ((BWAPI::Broodwar->getFrameCount() % 1000) == 0)
	//	&& !queue.isEmpty()
	//	&& ((BWAPI::Broodwar->self()->allUnitCount(highestQueueItem.metaType.whatBuilds()) < 1)
	//		|| ((highestQueueItem.metaType.mineralPrice() > BWAPI::Broodwar->self()->minerals())
	//			&& (highestQueueItem.metaType.gasPrice() > BWAPI::Broodwar->self()->gas()))))
	//{
	//	BWAPI::Broodwar->printf("                                           DebExt: Something is blocking the queue");
	//	queue.clearAll();
	//	queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_SCV), true);
	//	//queueDoSomething();
	//}
	
	// Remove upgrades and research from the queue (otherwise they would block)
	if (
		(highestQueueItem.metaType.isTech() && (BWAPI::Broodwar->self()->isResearching(highestQueueItem.metaType.techType)))
		|| (highestQueueItem.metaType.isUpgrade() && (BWAPI::Broodwar->self()->isUpgrading(highestQueueItem.metaType.upgradeType)))
		)
	{
		queue.removeCurrentHighestPriorityItem();
	}

	// eof ext

	// the current item to be used
	BuildOrderItem<PRIORITY_TYPE> & currentItem = queue.getHighestPriorityItem();

	// while there is still something left in the queue
	while (!queue.isEmpty()) 
	{
		// this is the unit which can produce the currentItem
		BWAPI::Unit * producer = selectUnitOfType(currentItem.metaType.whatBuilds());

		// check to see if we can make it right now
		bool canMake = canMakeNow(producer, currentItem.metaType);

		// if we try to build too many refineries manually remove it
		if (currentItem.metaType.isRefinery() && (BWAPI::Broodwar->self()->allUnitCount(BWAPI::Broodwar->self()->getRace().getRefinery() >= 3)))
		{
			queue.removeCurrentHighestPriorityItem();
			break;
		}
		
		// Ext
		//if ((currentItem.metaType.unitType == BWAPI::UnitTypes::Terran_Academy) &&
		//	(BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy) > 0) )
		//{
		//	queue.removeCurrentHighestPriorityItem();
		//}

		if (currentItem.metaType.isUnit()
			&& isDuplicate(currentItem.metaType.unitType))
		{
			BWAPI::Broodwar->printf("                                           DebExt: Removing duplicates");
			queue.removeCurrentHighestPriorityItem();
			break;
		}
		// eof ext

		// if the next item in the list is a building and we can't yet make it
		if (currentItem.metaType.isBuilding() && !(producer && canMake))
		{
			// construct a temporary building object
			Building b(currentItem.metaType.unitType, BWAPI::Broodwar->self()->getStartLocation());

			// set the producer as the closest worker, but do not set its job yet
			producer = WorkerManager::Instance().getBuilder(b, false);

			// predict the worker movement to that building location
			predictWorkerMovement(b);
		}

		// if we can make the current item
		if (producer && canMake) 
		{
			// create it
			createMetaType(producer, currentItem.metaType);
			assignedWorkerForThisBuilding = false;
			haveLocationForThisBuilding = false;

			// and remove it from the queue
			queue.removeCurrentHighestPriorityItem();

			// don't actually loop around in here
			break;
		}
		// otherwise, if we can skip the current item
		else if (queue.canSkipItem())
		{
			// skip it
			queue.skipItem();

			// and get the next one
			currentItem = queue.getNextHighestPriorityItem();				
		}
		else 
		{
			// so break out
			break;
		}
	}
}

bool ProductionManager::canMakeNow(BWAPI::Unit * producer, MetaType t)
{
	bool canMake = meetsReservedResources(t);
	if (canMake)
	{
		if (t.isUnit())
		{
			canMake = BWAPI::Broodwar->canMake(producer, t.unitType);
		}
		else if (t.isTech())
		{
			canMake = BWAPI::Broodwar->canResearch(producer, t.techType);
		}
		else if (t.isUpgrade())
		{
			canMake = BWAPI::Broodwar->canUpgrade(producer, t.upgradeType);
		}
		else
		{	
			assert(false);
		}
	}

	return canMake;
}

bool ProductionManager::detectBuildOrderDeadlock()
{
	// if the queue is empty there is no deadlock
	if (queue.size() == 0 || BWAPI::Broodwar->self()->supplyTotal() >= 390)
	{
		return false;
	}

	// are any supply providers being built currently
	bool supplyInProgress =		BuildingManager::Instance().isBeingBuilt(BWAPI::Broodwar->self()->getRace().getCenter()) || 
								BuildingManager::Instance().isBeingBuilt(BWAPI::Broodwar->self()->getRace().getSupplyProvider());

	// does the current item being built require more supply
	int supplyCost			= queue.getHighestPriorityItem().metaType.supplyRequired();
	int supplyAvailable		= std::max(0, BWAPI::Broodwar->self()->supplyTotal() - BWAPI::Broodwar->self()->supplyUsed());

	// if we don't have enough supply and none is being built, there's a deadlock
	if ((supplyAvailable < supplyCost) && !supplyInProgress)
	{
		return true;
	}

	return false;
}

// When the next item in the queue is a building, this checks to see if we should move to it
// This function is here as it needs to access prodction manager's reserved resources info
void ProductionManager::predictWorkerMovement(const Building & b)
{
	if (b.type.isAddon())
	{
		return;
	}

	// get a possible building location for the building
	if (!haveLocationForThisBuilding)
	{
		predictedTilePosition			= BuildingManager::Instance().getBuildingLocation(b);
	}

	if (predictedTilePosition != BWAPI::TilePositions::None)
	{
		haveLocationForThisBuilding		= true;
	}
	else
	{
		return;
	}
	
	// draw a box where the building will be placed
	int x1 = predictedTilePosition.x() * 32;
	int x2 = x1 + (b.type.tileWidth()) * 32;
	int y1 = predictedTilePosition.y() * 32;
	int y2 = y1 + (b.type.tileHeight()) * 32;
	if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawBoxMap(x1, y1, x2, y2, BWAPI::Colors::Blue, false);

	// where we want the worker to walk to
	BWAPI::Position walkToPosition		= BWAPI::Position(x1 + (b.type.tileWidth()/2)*32, y1 + (b.type.tileHeight()/2)*32);

	// compute how many resources we need to construct this building
	int mineralsRequired				= std::max(0, b.type.mineralPrice() - getFreeMinerals());
	int gasRequired						= std::max(0, b.type.gasPrice() - getFreeGas());

	// get a candidate worker to move to this location
	BWAPI::Unit * moveWorker			= WorkerManager::Instance().getMoveWorker(walkToPosition);

	// Conditions under which to move the worker: 
	//		- there's a valid worker to move
	//		- we haven't yet assigned a worker to move to this location
	//		- the build position is valid
	//		- we will have the required resources by the time the worker gets there
	if (moveWorker && haveLocationForThisBuilding && !assignedWorkerForThisBuilding && (predictedTilePosition != BWAPI::TilePositions::None) &&
		WorkerManager::Instance().willHaveResources(mineralsRequired, gasRequired, moveWorker->getDistance(walkToPosition)) )
	{
		// we have assigned a worker
		assignedWorkerForThisBuilding = true;

		// tell the worker manager to move this worker
		WorkerManager::Instance().setMoveWorker(mineralsRequired, gasRequired, walkToPosition);
	}
}

void ProductionManager::performCommand(BWAPI::UnitCommandType t) {

	// if it is a cancel construction, it is probably the extractor trick
	if (t == BWAPI::UnitCommandTypes::Cancel_Construction)
	{
		BWAPI::Unit * extractor = NULL;
		BOOST_FOREACH(BWAPI::Unit * unit, BWAPI::Broodwar->self()->getUnits())
		{
			if (unit->getType() == BWAPI::UnitTypes::Zerg_Extractor)
			{
				extractor = unit;
			}
		}

		if (extractor)
		{
			extractor->cancelConstruction();
		}
	}
}

int ProductionManager::getFreeMinerals()
{
	return BWAPI::Broodwar->self()->minerals() - BuildingManager::Instance().getReservedMinerals();
}

int ProductionManager::getFreeGas()
{
	return BWAPI::Broodwar->self()->gas() - BuildingManager::Instance().getReservedGas();
}

// return whether or not we meet resources, including building reserves
bool ProductionManager::meetsReservedResources(MetaType type) 
{
	// return whether or not we meet the resources
	return (type.mineralPrice() <= getFreeMinerals()) && (type.gasPrice() <= getFreeGas());
}

// this function will check to see if all preconditions are met and then create a unit
void ProductionManager::createMetaType(BWAPI::Unit * producer, MetaType t) 
{
	if (!producer)
	{
		return;
	}

	buildLearner.addAction(t);

	// TODO: special case of evolved zerg buildings needs to be handled

	// if we're dealing with a building
	if (t.isUnit() && t.unitType.isBuilding() 
		&& t.unitType != BWAPI::UnitTypes::Zerg_Lair 
		&& t.unitType != BWAPI::UnitTypes::Zerg_Hive
		&& t.unitType != BWAPI::UnitTypes::Zerg_Greater_Spire)
	{
		// send the building task to the building manager
		BuildingManager::Instance().addBuildingTask(t.unitType, BWAPI::Broodwar->self()->getStartLocation());
	}
	// if we're dealing with a non-building unit
	else if (t.isUnit()) 
	{
		// if the race is zerg, morph the unit
		if (t.unitType.getRace() == BWAPI::Races::Zerg) {
			producer->morph(t.unitType);

		// if not, train the unit
		} else {
			producer->train(t.unitType);
		}
	}
	// if we're dealing with a tech research
	else if (t.isTech())
	{
		producer->research(t.techType);
	}
	else if (t.isUpgrade())
	{
		//Logger::Instance().log("Produce Upgrade: " + t.getName() + "\n");
		producer->upgrade(t.upgradeType);
	}
	else
	{	
		// critical error check
//		assert(false);

		//Logger::Instance().log("createMetaType error: " + t.getName() + "\n");
	}
	
}

// selects a unit of a given type
BWAPI::Unit * ProductionManager::selectUnitOfType(BWAPI::UnitType type, bool leastTrainingTimeRemaining, BWAPI::Position closestTo) {

	// if we have none of the unit type, return NULL right away
	if (BWAPI::Broodwar->self()->completedUnitCount(type) == 0) 
	{
		return NULL;
	}

	BWAPI::Unit * unit = NULL;

	// if we are concerned about the position of the unit, that takes priority
	if (closestTo != BWAPI::Position(0,0)) {

		double minDist(1000000);

		BOOST_FOREACH (BWAPI::Unit * u, BWAPI::Broodwar->self()->getUnits()) {

			if (u->getType() == type) {

				double distance = u->getDistance(closestTo);
				if (!unit || distance < minDist) {
					unit = u;
					minDist = distance;
				}
			}
		}

		// if it is a building and we are worried about selecting the unit with the least
		// amount of training time remaining
	} else if (type.isBuilding() && leastTrainingTimeRemaining) {

		BOOST_FOREACH (BWAPI::Unit * u, BWAPI::Broodwar->self()->getUnits()) {

			if (u->getType() == type && u->isCompleted() && !u->isTraining() && !u->isLifted() &&!u->isUnpowered()) {

				return u;
			}
		}
		// otherwise just return the first unit we come across
	} else {

		BOOST_FOREACH(BWAPI::Unit * u, BWAPI::Broodwar->self()->getUnits()) 
		{
			if (u->getType() == type && u->isCompleted() && u->getHitPoints() > 0 && !u->isLifted() &&!u->isUnpowered()) 
			{
				return u;
			}
		}
	}

	// return what we've found so far
	return NULL;
}

void ProductionManager::populateTypeCharMap()
{
	typeCharMap['p'] = MetaType(BWAPI::UnitTypes::Protoss_Probe);
	typeCharMap['z'] = MetaType(BWAPI::UnitTypes::Protoss_Zealot);
	typeCharMap['d'] = MetaType(BWAPI::UnitTypes::Protoss_Dragoon);
	typeCharMap['t'] = MetaType(BWAPI::UnitTypes::Protoss_Dark_Templar);
	typeCharMap['c'] = MetaType(BWAPI::UnitTypes::Protoss_Corsair);
	typeCharMap['e'] = MetaType(BWAPI::UnitTypes::Protoss_Carrier);
	typeCharMap['h'] = MetaType(BWAPI::UnitTypes::Protoss_High_Templar);
	typeCharMap['n'] = MetaType(BWAPI::UnitTypes::Protoss_Photon_Cannon);
	typeCharMap['a'] = MetaType(BWAPI::UnitTypes::Protoss_Arbiter);
	typeCharMap['r'] = MetaType(BWAPI::UnitTypes::Protoss_Reaver);
	typeCharMap['o'] = MetaType(BWAPI::UnitTypes::Protoss_Observer);
	typeCharMap['s'] = MetaType(BWAPI::UnitTypes::Protoss_Scout);
	typeCharMap['l'] = MetaType(BWAPI::UpgradeTypes::Leg_Enhancements);
	typeCharMap['v'] = MetaType(BWAPI::UpgradeTypes::Singularity_Charge);
}

void ProductionManager::drawProductionInformation(int x, int y)
{
	// fill prod with each unit which is under construction
	std::vector<BWAPI::Unit *> prod;
	BOOST_FOREACH (BWAPI::Unit * unit, BWAPI::Broodwar->self()->getUnits())
	{
		if (unit->isBeingConstructed())
		{
			prod.push_back(unit);
		}
	}
	
	// sort it based on the time it was started
	std::sort(prod.begin(), prod.end(), CompareWhenStarted());

	if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextScreen(x, y, "\x04 Build Order Info:");
	if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextScreen(x, y+20, "\x04UNIT NAME");

	size_t reps = prod.size() < 10 ? prod.size() : 10;

	y += 40;
	int yy = y;

	// for each unit in the queue
	for (size_t i(0); i<reps; i++) {

		std::string prefix = "\x07";

		yy += 10;

		BWAPI::UnitType t = prod[i]->getType();

		if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextScreen(x, yy, "%s%s", prefix.c_str(), t.getName().c_str());
		if (Options::Debug::DRAW_UALBERTABOT_DEBUG) BWAPI::Broodwar->drawTextScreen(x+150, yy, "%s%6d", prefix.c_str(), prod[i]->getRemainingBuildTime());
	}

	queue.drawQueueInformation(x, yy+10);
}

ProductionManager & ProductionManager::Instance() {

	static ProductionManager instance;
	return instance;
}

void ProductionManager::onGameEnd()
{
	buildLearner.onGameEnd();
}

void ProductionManager::queueDoSomething()
{

	if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Terran)
	{
		if (StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranWraithRush1Port)
		{
			queueDoSomethingTerranWraithRush1Port();
		}
		else if (StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranVulturesAndTanks)
		{
			queueDoSomethingTerranVulturesAndTanks();
		}
		else
		{
			queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_SCV), true);

			queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Supply_Depot), true);

			queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Marine), true);
			queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Marine), true);
			queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Marine), true);
			queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Medic), true);
		}
		
	}
}

void ProductionManager::queueDoSomethingTerranWraithRush1Port()
{
	QueueConstructorExt::Instance().clearQueue();
	QueueConstructorExt::Instance().makeTerranWraithRush1PortQueue();
	queue = QueueConstructorExt::Instance().getQueue();
}

void ProductionManager::queueDoSomethingTerranVulturesAndTanks()
{
	QueueConstructorExt::Instance().clearQueue();
	QueueConstructorExt::Instance().makeTerranVulturesAndTanksQueue();
	queue = QueueConstructorExt::Instance().getQueue();
}

bool ProductionManager::isDuplicate(BWAPI::UnitType unitType)
{
	int numAcademies = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Academy);
	int numArmory = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Armory);
	int numEngineeringBay = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Engineering_Bay);
	int numScienceFacilities = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Science_Facility);



	// Remove additional Academies
	if (numAcademies > 0
		&& unitType == BWAPI::UnitTypes::Terran_Academy)
	{
		return true;
	}

	// Remove additional Science Facilities
	if (numScienceFacilities > 0
		&& unitType == BWAPI::UnitTypes::Terran_Science_Facility)
	{
		return true;
	}

	// Remove additional Armory
	if (numArmory > 0
		&& unitType == BWAPI::UnitTypes::Terran_Armory)
	{
		return true;
	}

	// Remove additional Enginnering Bays
	if (numEngineeringBay > 0
		&& unitType == BWAPI::UnitTypes::Terran_Engineering_Bay)
	{
		return true;
	}

	return false;

}

void ProductionManager::resetQueue()
{
	queue.clearAll();
}

void ProductionManager::manageIdleProduction()
{
	if (StrategyManager::Instance().getCurrentStrategy() == StrategyManager::Instance().TerranVulturesAndTanks)
	{
		manageIdleProductionVulturesAndTanks();
	}
	else
	{
		return;
	}
}

void ProductionManager::manageIdleProductionVulturesAndTanks()
{
	int supplyLeft = BWAPI::Broodwar->self()->supplyTotal() - BWAPI::Broodwar->self()->supplyUsed();
	int mineralsLeft = BWAPI::Broodwar->self()->minerals();
	int gasLeft = BWAPI::Broodwar->self()->gas();
	int frame = BWAPI::Broodwar->getFrameCount();

	int numCommandCenters = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Command_Center);

	int numScvs = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_SCV);
	int scvMineralPrice = BWAPI::UnitTypes::Terran_SCV.mineralPrice();
	int scvSupplyPrice = BWAPI::UnitTypes::Terran_SCV.supplyRequired();

	int numMarines = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Marine);
	int marineMineralPrice = BWAPI::UnitTypes::Terran_Marine.mineralPrice();
	int marineSupplyPrice = BWAPI::UnitTypes::Terran_Marine.supplyRequired();
	
	int numVultures = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Vulture);
	int vultureMineralPrice = BWAPI::UnitTypes::Terran_Vulture.mineralPrice();
	int vultureSupplyPrice = BWAPI::UnitTypes::Terran_Vulture.supplyRequired();

	int numTanks = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode) + BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode);
	int tankMineralPrice = BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode.mineralPrice();
	int tankGasPrice = BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode.gasPrice();
	int tankSupplyPrice = BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode.supplyRequired();

	int numWraiths = BWAPI::Broodwar->self()->allUnitCount(BWAPI::UnitTypes::Terran_Wraith);
	int wraithMineralPrice = BWAPI::UnitTypes::Terran_Wraith.mineralPrice();
	int wraithGasPrice = BWAPI::UnitTypes::Terran_Wraith.gasPrice();
	int wraithSupplyPrice = BWAPI::UnitTypes::Terran_Wraith.supplyRequired();

	if (!queue.isEmpty())
	{
		BuildOrderItem<PRIORITY_TYPE>& highestQueueItem = queue.getHighestPriorityItem();
		if (mineralsLeft > 100
			&& BWAPI::Broodwar->self()->supplyTotal() < (200 * 2)
			&& BWAPI::Broodwar->self()->supplyUsed() + 5 > BWAPI::Broodwar->self()->supplyTotal()
			&& BWAPI::Broodwar->self()->incompleteUnitCount(BWAPI::UnitTypes::Terran_Supply_Depot) < 1
			&& !(highestQueueItem.metaType.isBuilding() && highestQueueItem.metaType.mineralPrice() == 100))
		{
			queue.queueAsHighestPriority(MetaType(BWAPI::UnitTypes::Terran_Supply_Depot), true);
		}
	}


	if (frame > 14000)
	{
		mineralsLeft -= 400;
		gasLeft -= 250;
	}
	else
	{
		mineralsLeft -= 100;
		gasLeft -= 50;
	}

	//// To prevent taking all the resources
	//if (frame < 12000)
	//{
	//	mineralsLeft -= 150;
	//}
	//else if (numCommandCenters < 2
	//	&& frame > 12000
	//	&& BWAPI::Broodwar->self()->supplyUsed() > (24 * 2))
	//{
	//	mineralsLeft -= 450;
	//}
	//else if (BWAPI::Broodwar->self()->supplyUsed() > (30 * 2))
	//{
	//	mineralsLeft -= 300;
	//	gasLeft -= 200;
	//}
	//else
	//{
	//	mineralsLeft -= 150;
	//	gasLeft -= 100;
	//}

	BOOST_FOREACH(BWAPI::Unit* unit, BWAPI::Broodwar->self()->getUnits())
	{
		BWAPI::UnitType unitType = unit->getType();
		// Loop through buildings
		if (!unitType.isBuilding())
		{
			continue;
		}

		// Idle procution building
		if (unitType.canProduce()
			&& !unit->isTraining()
			&& !unit->isLifted())
		{
			if (unitType == BWAPI::UnitTypes::Terran_Barracks)
			{
				if (mineralsLeft >= marineMineralPrice
					&& supplyLeft >= marineSupplyPrice
					&& numMarines < 24)
				{
					unit->train(BWAPI::UnitTypes::Terran_Marine);

					mineralsLeft -= marineMineralPrice;
					supplyLeft -= marineSupplyPrice;
				}
			}
			else if (unitType == BWAPI::UnitTypes::Terran_Factory)
			{
				if (numVultures * 1.4 > numTanks
					&& mineralsLeft > tankMineralPrice
					&& gasLeft > tankMineralPrice
					&& supplyLeft > tankSupplyPrice
					&& unit->getAddon() != NULL)
				{
					unit->train(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode);

					mineralsLeft -= tankMineralPrice;
					gasLeft -= tankGasPrice;
					supplyLeft -= tankSupplyPrice;
				}
				else if (mineralsLeft > marineMineralPrice
					&& supplyLeft > vultureSupplyPrice)
				{
					unit->train(BWAPI::UnitTypes::Terran_Vulture); 

					mineralsLeft -= vultureMineralPrice;
					supplyLeft -= vultureSupplyPrice;
				}
			}
			else if (unitType == BWAPI::UnitTypes::Terran_Command_Center)
			{
				if (numScvs < (numCommandCenters * 20)
					&& mineralsLeft > scvMineralPrice
					&& supplyLeft > scvSupplyPrice)
				{
					unit->train(BWAPI::UnitTypes::Terran_SCV);

					mineralsLeft -= scvMineralPrice;
					supplyLeft -= scvSupplyPrice;
				}
			}
			else if (unitType == BWAPI::UnitTypes::Terran_Starport)
			{
				if (mineralsLeft > wraithMineralPrice
					&& gasLeft > wraithGasPrice
					&& supplyLeft > wraithSupplyPrice)
				{
					unit->train(BWAPI::UnitTypes::Terran_Wraith);

					mineralsLeft -= scvMineralPrice;
					gasLeft -= wraithGasPrice;
					supplyLeft -= scvSupplyPrice;
				}
			}
		}
	}
}

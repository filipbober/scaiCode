#pragma once

#include "BWAPI.h"
#include <boost/foreach.hpp>
#include <boost/array.hpp>
#include "assert.h"
#include <stdio.h>
#include <sstream>

#include <fstream>
#include <iostream>
#include <sstream>

#include "DependencyGraph.hpp"
#include "ActionSetExt.hpp"
#include "StarcraftAction.hpp"
#include "ActionInProgress.hpp"
#include "MacroAction.hpp"

static bool DEBUG_StarcraftData = true;

namespace BuildOrderSearch
{

class StarcraftData;
StarcraftData & getStarcraftDataInstance();

///////////////////////////////////////////////////////////////////////////////
//
// StarcraftData
//
///////////////////////////////////////////////////////////////////////////////

class StarcraftData {

	BWAPI::Race 					race;
	std::vector<StarcraftAction> 	actions;
	std::vector<MacroAction> 		macroActions;
	
	DependencyGraph<bool> 			DG;

	Action 							worker, 
									refinery, 
									resourceDepot, 
									supplyProvider;
									
	unsigned int       				hashData[MAX_ACTIONS][MAX_OF_ACTION][NUM_HASHES];
	
	// adds the actions we want from each race to the vector
	// gets data from BWAPI
	void addActions(BWAPI::Race & r)
	{
		if (r == BWAPI::Races::Protoss)
		{
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Probe, actions.size()));					// 0
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Pylon, actions.size()));					// 1
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Nexus, actions.size()));					// 2
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Gateway, actions.size()));					// 3
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Zealot, actions.size()));					// 4
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Cybernetics_Core, actions.size()));			// 5
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Dragoon, actions.size()));					// 6
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Assimilator, actions.size()));				// 7
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Singularity_Charge, actions.size()));			// 8
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Forge, actions.size()));					// 9
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Photon_Cannon, actions.size()));			// 10
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_High_Templar, actions.size()));				// 11
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Citadel_of_Adun, actions.size()));			// 12
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Templar_Archives, actions.size()));			// 13
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Robotics_Facility, actions.size()));		// 14
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Robotics_Support_Bay, actions.size()));		// 15
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Observatory, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Stargate, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Scout, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Arbiter_Tribunal, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Arbiter, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Shield_Battery, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Dark_Templar, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Shuttle, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Reaver, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Observer, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Corsair, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Fleet_Beacon, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Protoss_Carrier, actions.size()));
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Leg_Enhancements, actions.size()));
		}
		else if (r == BWAPI::Races::Terran)
		{
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_SCV, actions.size()));					
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Supply_Depot, actions.size()));
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Command_Center, actions.size()));
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Barracks, actions.size()));
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Refinery, actions.size()));
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Marine, actions.size()));
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Academy, actions.size()));
			//actions.push_back(StarcraftAction(BWAPI::TechTypes::Stim_Packs, actions.size()));
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Medic, actions.size()));
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Factory, actions.size()));
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Starport, actions.size()));
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Wraith, actions.size()));

			// Units
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_SCV, actions.size()));							// 0
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Marine, actions.size()));						// 1
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Medic, actions.size()));							// 2
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Vulture, actions.size()));						// 3
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Firebat, actions.size()));						// 4
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Ghost, actions.size()));							// 5
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Goliath, actions.size()));						// 6
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode, actions.size()));			// 7
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Siege_Tank_Siege_Mode, actions.size()));			// 8
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Dropship, actions.size()));						// 9
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Wraith, actions.size()));						// 10
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Valkyrie, actions.size()));						// 11
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Battlecruiser, actions.size()));					// 12
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Science_Vessel, actions.size()));				// 13

			// Buildings
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Command_Center, actions.size()));				// 14
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Comsat_Station, actions.size()));				// 15
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Nuclear_Silo, actions.size()));					// 16
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Supply_Depot, actions.size()));					// 17
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Refinery, actions.size()));						// 18
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Barracks, actions.size()));						// 19
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Academy, actions.size()));						// 20
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Engineering_Bay, actions.size()));				// 21
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Factory, actions.size()));						// 22
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Machine_Shop, actions.size()));					// 23
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Armory, actions.size()));						// 24
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Starport, actions.size()));						// 25
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Control_Tower, actions.size()));					// 26
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Science_Facility, actions.size()));				// 27
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Physics_Lab, actions.size()));					// 28
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Covert_Ops, actions.size()));					// 29
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Missile_Turret, actions.size()));				// 30

			// Upgrades
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Terran_Infantry_Armor, actions.size()));				// 31
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Terran_Infantry_Weapons, actions.size()));			// 32
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Terran_Ship_Plating, actions.size()));				// 33
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Terran_Ship_Weapons, actions.size()));				// 34
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Terran_Vehicle_Plating, actions.size()));			// 35
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Terran_Vehicle_Weapons, actions.size()));			// 36

			// Techs
			actions.push_back(StarcraftAction(BWAPI::TechTypes::Stim_Packs, actions.size()));							// 37
			actions.push_back(StarcraftAction(BWAPI::TechTypes::Spider_Mines, actions.size()));							// 38
			actions.push_back(StarcraftAction(BWAPI::TechTypes::Cloaking_Field, actions.size()));						// 39			
			actions.push_back(StarcraftAction(BWAPI::TechTypes::Restoration, actions.size()));							// 40
			actions.push_back(StarcraftAction(BWAPI::TechTypes::Optical_Flare, actions.size()));						// 41
			actions.push_back(StarcraftAction(BWAPI::TechTypes::Lockdown, actions.size()));								// 42
			actions.push_back(StarcraftAction(BWAPI::TechTypes::Tank_Siege_Mode, actions.size()));						// 43
			actions.push_back(StarcraftAction(BWAPI::TechTypes::EMP_Shockwave, actions.size()));						// 44			
			actions.push_back(StarcraftAction(BWAPI::TechTypes::Irradiate, actions.size()));							// 45
			actions.push_back(StarcraftAction(BWAPI::TechTypes::Yamato_Gun, actions.size()));							// 46

			// Additional (forgotten...)
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Ion_Thrusters, actions.size()));						// 47
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Charon_Boosters, actions.size()));					// 48
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::U_238_Shells, actions.size()));						// 49

			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Terran_Supply_Depot, actions.size()));					// 17

			//actions.push_back(StarcraftAction(BWAPI::TechTypes::Healing, actions.size()));								// xx - starting tech
			//actions.push_back(StarcraftAction(BWAPI::TechTypes::Defensive_Matrix, actions.size()));						// xx - starting tech

		}
		else if (r == BWAPI::Races::Zerg)
		{
			// Max actions = 30
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Drone, actions.size()));					// 0
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Overlord, actions.size()));				// 1
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Hatchery, actions.size()));				// 2
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Spawning_Pool, actions.size()));			// 3
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Zergling, actions.size()));				// 4
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Extractor, actions.size()));				// 5
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Lair, actions.size()));					// 6
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Hydralisk_Den, actions.size()));			// 7
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Spire, actions.size()));					// 8
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Hydralisk, actions.size()));				// 9
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Mutalisk, actions.size()));				// 10

			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Hive, actions.size()));					// 11
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Evolution_Chamber, actions.size()));		// 12
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Creep_Colony, actions.size()));			// 13
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Sunken_Colony, actions.size()));			// 14	
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Spore_Colony, actions.size()));			// 15
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Queens_Nest, actions.size()));				// 16
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Greater_Spire, actions.size()));			// 17
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Defiler_Mound, actions.size()));			// 18
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Ultralisk_Cavern, actions.size()));		// 19
			actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Nydus_Canal, actions.size()));				// 20

			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Queen, actions.size()));					// 21
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Ultralisk, actions.size()));				// 22
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Defiler, actions.size()));					// 23
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Lurker, actions.size()));					// 24
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Guardian, actions.size()));				// 25
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Devourer, actions.size()));				// 26
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Broodling, actions.size()));				// 27
			//actions.push_back(StarcraftAction(BWAPI::UnitTypes::Zerg_Infested_Terran, actions.size()));			// 28

			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Zerg_Carapace, actions.size()));				// 29
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Zerg_Flyer_Attacks, actions.size()));		// 30
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Zerg_Flyer_Carapace, actions.size()));		// 31
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Zerg_Melee_Attacks, actions.size()));		// 32
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Zerg_Missile_Attacks, actions.size()));		// 33
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Chitinous_Plating, actions.size()));			// 34
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Adrenal_Glands, actions.size()));			// 35
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Pneumatized_Carapace, actions.size()));		// 36
			actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Metabolic_Boost, actions.size()));			// 37 21
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Ventral_Sacs, actions.size()));				// 38
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::Muscular_Augments, actions.size()));			// 39
			//actions.push_back(StarcraftAction(BWAPI::TechTypes::Burrowing, actions.size()));					// 40
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::getUpgradeType("Glial Reconstruction"), actions.size()));		// 41
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::getUpgradeType("Tunneling Claws"), actions.size()));				// 42
			//actions.push_back(StarcraftAction(BWAPI::UpgradeTypes::getUpgradeType("Centrifugal Hooks"), actions.size()));			// 43
		}	
	}

	// adds all prerequisites to the StarcraftAction objects
	void addPrerequisites()
	{
		// for each of the actions
		for (size_t i=0; i<actions.size(); i++)
		{
			if (DEBUG_StarcraftData) printf("Adding Pre %d of %d : %s\n", (int)i, (int)actions.size(), actions[i].getName().c_str());

			// set the prerequisite ActionSet based on the function below
			actions[i].setPrerequisites(calculatePrerequisites(actions[i]));
		}
	}
	
	void calculateSpecialUnits()
	{
		worker			= getAction(race.getWorker());
		resourceDepot	= getAction(race.getCenter());
		supplyProvider	= getAction(race.getSupplyProvider());
		refinery		= getAction(race.getRefinery());

		std::cout << race.getCenter().getName() << std::endl;
	
		// also set what builds actions
		for (size_t i=0; i<actions.size(); i++)
		{			
			if (actions[i].whatBuilds() == BWAPI::UnitTypes::Zerg_Larva)
			{
				Action a = 255;
				actions[i].setWhatBuildsAction(a, false, true);
			}
			else
			{
				Action a = getAction(actions[i].whatBuilds());
				actions[i].setWhatBuildsAction(a, DATA[a].isBuilding(), false);
			}		
			
		}
	}
	
	// returns an ActionSet of prerequisites for a given action
	ActionSet calculatePrerequisites(StarcraftAction & action)
	{
		ActionSet pre;

		if (DEBUG_StarcraftData) 
		{
			printf("DEBUG: Hello\n");
			printf("DEBUG: %d  \t%s \t%s\n", getAction(action), action.getName().c_str(), actions[getAction(action)].getName().c_str());
		}

		// if it's a UnitType
		if (action.getType() == StarcraftAction::UnitType)
		{
			std::map<BWAPI::UnitType, int> requiredUnits = action.getUnitType().requiredUnits();
			BWAPI::UnitType actionType = action.getUnitType();

			// if it's a protoss building that isn't a Nexus or Assimilator, we need a pylon (indirectly)
			if (actionType.getRace() == BWAPI::Races::Protoss && actionType.isBuilding() && !actionType.isResourceDepot() && 
				!(actionType == BWAPI::UnitTypes::Protoss_Pylon) && !(actionType == BWAPI::UnitTypes::Protoss_Assimilator))
			{
				pre.add(getAction(BWAPI::UnitTypes::Protoss_Pylon));
			}

			// for each of the required UnitTypes
			for (std::map<BWAPI::UnitType, int>::iterator unitIt = requiredUnits.begin(); unitIt != requiredUnits.end(); unitIt++)
			{
				if (DEBUG_StarcraftData) printf("\tPRE: %s\n", unitIt->first.getName().c_str());
	
				BWAPI::UnitType type = unitIt->first;

				// add the action to the ActionSet if it is not a larva
				if (type != BWAPI::UnitTypes::Zerg_Larva)
				{
					//printf("\t\tAdding %s\n", type.getName().c_str());
					pre.add(getAction(type));
				}
			}

			// if there is a TechType required
			if (action.getUnitType().requiredTech() != BWAPI::TechTypes::None)
			{
				if (DEBUG_StarcraftData) printf("\tPRE: %s\n", action.getUnitType().requiredTech().getName().c_str());

				// add it to the ActionSet
				pre.add(getAction(action.getUnitType().requiredTech()));
			}
		}

		// if it's a TechType
		if (action.getType() == StarcraftAction::TechType)
		{
			if (action.getTechType().whatResearches() != BWAPI::UnitTypes::None)
			{
				if (DEBUG_StarcraftData) printf("\tPRE: %s\n", action.getTechType().whatResearches().getName().c_str());

				// add what researches it
				pre.add(getAction(action.getTechType().whatResearches()));
			}
		}

		// if it's an UpgradeType
		if (action.getType() == StarcraftAction::UpgradeType)
		{
			if (action.getUpgradeType().whatUpgrades() != BWAPI::UnitTypes::None)
			{
				if (DEBUG_StarcraftData) printf("\tPRE: %s\n", action.getUpgradeType().whatUpgrades().getName().c_str());

				// add what upgrades it
				pre.add(getAction(action.getUpgradeType().whatUpgrades()));
			}
		}

		//printf("Finish Prerequisites\n");
		return pre;
	}
	
	
	void calculateDependencyGraph()
	{
		// zero out the graph
		DG = DependencyGraph<bool>(size());
		
		// for each action we have
		for (size_t a=0; a<actions.size(); ++a)
		{
			// get the prerequisites for this action
			ActionSet pre = actions[a].getPrerequisites();
			
			// subtract the worker from resource depot, prevent cyclic dependency
			if (a == DATA.getResourceDepot())
			{
				pre.subtract(DATA.getWorker());
			}
			
			// loop through prerequisites
			while (!pre.isEmpty())
			{
				// get the next action
				Action p = pre.popAction();
				
				// add it to the dependency graph
				DG.set(a, p, true); 
			}
		}
		
		// do transitive reduction to obtain the tree
		DG.transitiveReduction();
	}
	
	void addMacroActions()
	{
		for (Action i=0; i<DATA.size(); ++i)
		{
			macroActions.push_back(MacroAction(i));
		}
	}
	
	void calculateHashData()
	{
	    //srand(time(NULL));
	    	    
	    for (int a(0); a<MAX_ACTIONS; ++a)
	    {
	    	for (int n(0); n<MAX_OF_ACTION; ++n)
	    	{
	    		for (int h(0); h<NUM_HASHES; ++h)
	    		{
	    			hashData[a][n][h] = rand();
	    		}
	    	}
	    }
	}

public:

	double mpwpf;
	double gpwpf;
	
	int mpwpfScaled;
	int gpwpfScaled;

	int resourceScalingFactor;
	
	StarcraftData() {}
	~StarcraftData() {}
	
	// initialize the data with a race
	void init(BWAPI::Race r)
	{	
		// set the race
		race = r;
	
		// set resource constants (determined empyrically)
		mpwpf = 0.045;
		gpwpf = 0.07;
		resourceScalingFactor = 100;
		mpwpfScaled = (int)(mpwpf * resourceScalingFactor);
		gpwpfScaled = (int)(gpwpf * resourceScalingFactor);
	
		// read the actions from file
		addActions(r);
		
		// calculate the prerequisites of those actions
		addPrerequisites();
		
		// calculate worker, refinery, resource and supply depots
		// this is done for constant time returns without BWAPI reference
		calculateSpecialUnits();
		
		// calculates the dependency graph tree based on prerequisite graph
		calculateDependencyGraph();
		
		// addMacroActions
		addMacroActions();
		
		// set random hash data
		calculateHashData();
	}

	
	ActionSet getStrictDependency(int action)
	{
		return DG.getStrictDependency(action);
	}

	const StarcraftAction & operator [] (int i) const
	{ 
		return actions[i]; 
	}

	StarcraftAction & operator [] (BWAPI::UnitType & t) 
	{ 
		return getStarcraftAction(t); 
	}

	ActionSet getPrerequisites(Action action) const
	{
		return actions[action].getPrerequisites();
	}

	ActionSet getPrerequisites(StarcraftAction & action) const
	{
		return actions[getAction(action)].getPrerequisites();
	}

	Action getAction(StarcraftAction & a) const
	{
		int index = -1;

		for (size_t i(0); i < actions.size(); ++i)
		{
			if (actions[i] == a)
			{
				index = i;
				break;
			}
		}

		assert(index > -1);
		return (Action)index;
	}

	Action getAction(const BWAPI::UnitType & a) const
	{
		int index = -1;

		for (size_t i(0); i < actions.size(); ++i)
		{
			if (actions[i].isUnit() && actions[i].getUnitType() == a)
			{
				index = i;				
				break;
			}
		}
		
		if (index < 0) 
		{
			printf("Error Incoming: %s\n", a.getName().c_str());
		}

		assert(index > -1);		
		return (Action)index;
	}

	Action getAction(const BWAPI::TechType & a) const
	{
		int index = -1;

		for (size_t i(0); i < actions.size(); ++i)
		{
			if (actions[i].isTech() && actions[i].getTechType() == a)
			{
				index = i;
				break;
			}
		}

		assert(index > -1);
		return (Action)index;
	}

	Action getAction(const BWAPI::UpgradeType & a) const
	{
		int index = -1;

		for (size_t i(0); i < actions.size(); ++i)
		{
			if (actions[i].isUpgrade() && actions[i].getUpgradeType() == a)
			{
				index = i;
				break;
			}
		}

		assert(index > -1);
		return (Action)index;
	}

	Action getWorker() const
	{
		return worker;
	}

	Action getRefinery() const
	{
		return refinery;
	}

	Action getSupplyProvider() const
	{
		return supplyProvider;
	}
	
	Action getResourceDepot() const
	{
		return resourceDepot;
	}

	int size() const
	{
		return actions.size();
	}

	ActionSet getAllActions() const
	{
		ActionSet temp;

		for (int i=0; i<size(); ++i)
		{
			temp.add(i);
		}

		return temp;
	}

	const BWAPI::Race & getRace() const
	{
		return race;
	}

	const StarcraftAction & getStarcraftAction(Action i) const
	{
		return actions[i];
	}

	StarcraftAction & getStarcraftAction(BWAPI::UnitType & t)
	{		
		return actions[getAction(t)];
	}

	StarcraftAction & getStarcraftAction(BWAPI::TechType & t)
	{
		return actions[getAction(t)];
	}

	StarcraftAction & getStarcraftAction(BWAPI::UpgradeType & t)
	{
		return actions[getAction(t)];
	}
	
	const unsigned int getHashValue(Action a, int n, int h) const
	{
	    return hashData[a][n][h];
	}
	
	void printActionNames(ActionSet s)
	{	
		while (!s.isEmpty())
		{
			Action a = s.popAction();
			
			printf("%s\n", getStarcraftAction(a).getName().c_str());
		}
	}
};
}
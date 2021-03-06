#pragma once
#include "base\BuildOrderQueue.h"
#include "..\..\StarcraftBuildOrderSearch\Source\starcraftsearch\ActionSet.hpp"
#include "..\..\StarcraftBuildOrderSearch\Source\starcraftsearch\StarcraftData.hpp"


class QueueConstructorExt
{
public:
	QueueConstructorExt();
	~QueueConstructorExt();

	// Singleton
	static QueueConstructorExt& Instance();

	BuildOrderQueue getQueue();
	void clearQueue();

	void makeTestQueue();

	// Terran queues
	void makeTerranVulturesAndTanksQueue();
	void makeTerranWraithRush1PortQueue();

	void cleanQueue();

	void makeExpansion();

	// Specific units
	void queueTerranMarines(double prodPercent);
	void queueTerranFirebats(double prodPercent);
	void queueTerranVultures(double prodPercent);
	void queueTerranTanks(double prodPercent);
	void queueTerranGoliaths(double prodPercent);
	void queueTerranWraiths(double prodPercent);
	void queueTerranSCVs(double prodPercent);
	void queueTerranBCs(double prodPercent);	
	void queueTerranScienceVessels(double prodPercent);

	void queueCommandCenters(int desiredNo);
	void queueTerranSupply(int desiredNo);
	void queueTerranBunkers(int desiredNo);
	void queueEngineeringBays(int desiredNo);
	void queueTerranTurrets(int desiredNo);
	void queueTerranBarracks(int desiredNo);
	void queueTerranAcademies(int desiredNo);
	void queueTerranFactories(int desiredNo);
	void queueTerranStarports(int desiredNo);
	void queueTerranScienceFacilities(int desiredNo);
	void queueTerranArmory(int desiredNo);
	void addControlTowers();

	void queueTerranBCUpgrades();
	void queueTerranWraithUpgrades();
	void queueTerranTankUpgrades();
	void queueTerranMarinesUpgrades();
	void queueTerranGhostUpgrades();

	void queueTerranBioUpgrades();

	void queueTerranBioWeapons();
	void queueTerranBioArmor();

	void queueTerranShipUpgrades();
	void queueTerranAirWeapons();
	void queueTerranAirArmor();

	// Tech
	void queueTechMarines();
	void queueTechVultures();
	void queueTechTanks();
	void queueTechBattlecruisers();
	void queueTechGoliaths();	

	// Other
	bool isAirThreat();
	bool isDetectorNeeded();
	int getQueueSupply();
	


private:
	BuildOrderQueue _queue;
	int _lastInvoked;
	bool _airThreatExists;
	bool _isDetectorNeeded;


};


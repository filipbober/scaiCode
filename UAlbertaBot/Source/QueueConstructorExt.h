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
	// Specific units
	void queueTerranMarines(double prodPercent);
	void queueTerranFirebats(double prodPercent);
	void queueTerranWraiths(double prodPercent);
	void queueTerranSCVs(double prodPercent);
	void queueTerranBCs(double prodPercent);

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

	void queueTerranBCUpgrades();
	void queueTerranWraithUpgrades();
	void queueTerranTankUpgrades();
	void queueTerranMarinesUpgrades();
	void queueTerranGhostUpgrades();

	void queueTerranBioUpgrades();

	void queueTerranBioWeapons();
	void queueTerranBioArmor();

	void queueTerranAirWeapons();
	void queueTerranAirArmor();
	


private:
	BuildOrderQueue _queue;


};


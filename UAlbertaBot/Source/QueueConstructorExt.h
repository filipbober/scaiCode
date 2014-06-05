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

	void queueTerranSupply(double prodPercent);
	void queueTerranBunkers(double prodPercent);
	void queueTerranTurrets(double prodPercent);

	void queueTerranBCUpgrades(double prodPercent);
	void queueTerranTankUpgrades(double prodPercent);
	void queueTerranBioUpgrades(double prodPercent);


private:
	BuildOrderQueue _queue;


};


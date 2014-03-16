#pragma once
#include "base\BuildOrderQueue.h"
#include "base\ProductionManager.h"

class BuildOrderCreator
{
public:
	BuildOrderCreator();
	virtual ~BuildOrderCreator();

	virtual void Initialize();
	
	virtual std::vector<MetaType>  GenerateQueue() = 0;
	

};


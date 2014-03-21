#pragma once
#include "base\BuildOrderQueue.h"
#include "base\ProductionManager.h"

class BuildOrderTest
{
public:
	BuildOrderTest();
	~BuildOrderTest();

	std::vector<MetaType>  BuildOrderTest::GenerateTestQueue();
};

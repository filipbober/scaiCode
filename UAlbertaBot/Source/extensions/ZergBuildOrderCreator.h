#pragma once
#include "BuildOrderCreator.h"
class ZergBuildOrderCreator :
	public BuildOrderCreator
{
public:
	ZergBuildOrderCreator();
	~ZergBuildOrderCreator();

	std::vector<MetaType>  GenerateQueue();
};


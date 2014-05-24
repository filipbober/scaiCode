#pragma once
#include "Common.h"

class UnitManagerExt
{
public:
	UnitManagerExt();
	~UnitManagerExt();

	void update();
	void addUnit(BWAPI::Unit* unitToAdd);
	

private:
	UnitVector _units;

};


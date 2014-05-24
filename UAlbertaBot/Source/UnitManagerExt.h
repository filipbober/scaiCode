#pragma once
#include "Common.h"
#include "UnitDataExt.h"

typedef std::vector<UnitDataExt> UnitDataVector;

class UnitManagerExt
{
public:
	UnitManagerExt();
	~UnitManagerExt();

	void update();
	void addUnit(BWAPI::Position destination, BWAPI::Unit* unitToAdd);
	

private:
	//UnitVector _units;
	UnitDataVector _unitsData;
	

};


#pragma once
#include "Common.h"
#include "UnitDataExt.h"

typedef std::vector<UnitDataExt> UnitDataVector;

class UnitManagerExt
{
public:
	UnitManagerExt();
	~UnitManagerExt();

	// Singleton
	static UnitManagerExt& Instance();

	void update();
	void addUnit(BWAPI::Unit* unitToAdd);
	

private:
	//UnitVector _units;
	UnitDataVector _unitsData;
	

};


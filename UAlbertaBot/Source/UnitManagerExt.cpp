#include "UnitManagerExt.h"


UnitManagerExt::UnitManagerExt()
{
}


UnitManagerExt::~UnitManagerExt()
{
}

void UnitManagerExt::update()
{
	// Erase not-existing units from the vector
	for (UnitDataVector::iterator it = _unitsData.begin(); it != _unitsData.end(); /*++it*/)
	{
		
		if (!it->getUnit()->exists())
		{
			_unitsData.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void UnitManagerExt::addUnit(BWAPI::Unit* unitToAdd)
{
	int unitToAddId = unitToAdd->getID();

	// Check if unit is already added
	BOOST_FOREACH(UnitDataExt unitData, _unitsData)
	{
		if (unitToAddId == unitData.getUnit()->getID())
		{
			return;
		}
	}

	// if it's not then add it
	UnitDataExt unitData = UnitDataExt(unitToAdd);
	_unitsData.push_back(unitData);

}

UnitManagerExt& UnitManagerExt::Instance()
{	
	static UnitManagerExt instance;
	return instance;
}

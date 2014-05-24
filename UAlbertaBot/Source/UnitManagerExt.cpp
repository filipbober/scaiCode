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
	for (UnitVector::iterator it = _units.begin(); it != _units.end(); /*++it*/)
	{
		
		if (!(*it)->exists())
		{
			_units.erase(it);
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
	BOOST_FOREACH(BWAPI::Unit * unit, _units)
	{
		if (unitToAddId == unit->getID())
		{
			return;
		}
	}

	// if it's not then add it
	_units.push_back(unitToAdd);

}

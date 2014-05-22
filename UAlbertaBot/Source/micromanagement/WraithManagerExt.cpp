#include "WraithManagerExt.h"


WraithManagerExt::WraithManagerExt()
{
}


WraithManagerExt::~WraithManagerExt()
{
}

void WraithManagerExt::executeMicro(const UnitVector & targets)
{
	//const UnitVector & selectedUnits = getUnits();


	//// figure out targets
	//UnitVector selectedUnitTargets;
	//for (size_t i(0); i<targets.size(); i++)
	//{
	//	// conditions for targeting
	//	if (targets[i]->isVisible())
	//	{
	//		selectedUnitTargets.push_back(targets[i]);
	//	}
	//}

	//setAverageEnemyPosition(selectedUnitTargets);

	//// For each unit
	//BOOST_FOREACH(BWAPI::Unit * selectedUnit, selectedUnits)
	//{
	//	if ((selectedUnit->getType() == BWAPI::UnitTypes::Terran_Marine)
	//		&& !(selectedUnit->isLoaded())
	//		&& (hasBunkerSpace()))
	//	{

	//		goToBunker(selectedUnit);
	//		continue;

	//	}

	//	// if the order is to attack or defend
	//	if (order.type == order.Attack || order.type == order.Defend)
	//	{
	//		// if there are targets
	//		if (!selectedUnitTargets.empty())
	//		{
	//			// find the best target for this Vulture
	//			BWAPI::Unit * target = getTarget(selectedUnit, selectedUnitTargets);

	//			// attack it				
	//			kiteTarget(selectedUnit, target);

	//		}
	//		// if there are no targets
	//		else
	//		{
	//			// if we're not near the order position
	//			if (selectedUnit->getDistance(order.position) > 100)
	//			{
	//				// move to it
	//				smartAttackMove(selectedUnit, order.position);
	//			}
	//		}
	//	}

	//	if (Options::Debug::DRAW_UALBERTABOT_DEBUG)
	//	{
	//		BWAPI::Broodwar->drawLineMap(selectedUnit->getPosition().x(), selectedUnit->getPosition().y(),
	//			selectedUnit->getTargetPosition().x(), selectedUnit->getTargetPosition().y(), Options::Debug::COLOR_LINE_TARGET);
	//	}
	//}

}

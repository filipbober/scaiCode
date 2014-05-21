#pragma once
#include "MicroManager.h"
class WraithManagerExt :
	public MicroManager
{
public:
	WraithManagerExt();
	~WraithManagerExt();

	void executeMicro(const UnitVector & targets);
};


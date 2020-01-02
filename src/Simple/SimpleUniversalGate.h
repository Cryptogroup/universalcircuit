//#pragma once
#ifndef _SimpleUniversalGate_h_
#define _SimpleUniversalGate_h_

#include "SimpleGate.h"
#include "../Eu/EugNode.h"
class SimpleUniversalGate : public SimpleGate
{
public:
	bool generateOutput();
	void clear();

	explicit SimpleUniversalGate(EugNode* controlNode);
	~SimpleUniversalGate();
};
#endif

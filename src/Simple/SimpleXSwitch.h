//#pragma once
#ifndef _SimpleXSwitch_h_
#define _SimpleXSwitch_h_

#include "../Eu/EugNode.h"

#include "SimpleGate.h"
//class SimpleGate;
class SimpleXSwitch : public SimpleGate
{
public:
	explicit SimpleXSwitch(EugNode* controlNode);
	~SimpleXSwitch();

	bool generateOutput();
	void clear();
};

#endif // !SimpleXSwitch
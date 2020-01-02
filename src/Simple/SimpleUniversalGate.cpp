//#include "stdafx.h"
#include "SimpleUniversalGate.h"

SimpleUniversalGate::SimpleUniversalGate(EugNode* controlNode)
	: SimpleGate(controlNode)
{
}


SimpleUniversalGate::~SimpleUniversalGate()
{
}

bool SimpleUniversalGate::generateOutput()
{
	if ((inputReady0 && inputReady1) == false)
	{
		return false;
	}
	//cout << endl;




	if (*input0 == false)
	{
		if (*input1 == false)
		{
			output0 = (controlNode->gateType >> 3) % 2;
			output1 = output0;
		}
		else
		{
			output0 = (controlNode->gateType >> 2) % 2;
			output1 = output0;
		}
	}
	else
	{
		if (*input1 == false)
		{
			output0 = (controlNode->gateType >> 1) % 2;
			output1 = output0;
		}
		else
		{
			output0 = controlNode->gateType % 2;
			output1 = output0;
		}
		inputReady1 = false;
		inputReady0 = false;
	}
	
	//cout << "----controlNode->poleId-------" << controlNode->poleId << "------------------------" << endl;
	//cout << "-----output0------------------" << output0 << "----------------------------" << endl;

	//output the gate.

	/**
	if (*input0 == false)
	{
		if (*input1 == false)
		{
			cout << "-------------------  F F ------------------------" << endl;
			cout << "----controlNode->gateType------" << controlNode->gateType << "------------------------" << endl;
			cout << "----controlNode->poleId-------" << controlNode->poleId << "------------------------" << endl;
			cout << "-----output0------------------" << output0 << "----------------------------" << endl;
		}
		else
		{
			cout << "-------------------  F T ------------------------" << endl;
			cout << "----controlNode->gateType------" << controlNode->gateType << "------------------------" << endl;
			cout << "----controlNode->poleId-------" << controlNode->poleId << "------------------------" << endl;
			cout << "-----output0------------------" << output0 << "----------------------------" << endl;
		}
	}
	else
	{
		if (*input1 == false)
		{
			cout << "-------------------  T F ------------------------" << endl;
			cout << "----controlNode->gateType------" << controlNode->gateType << "------------------------" << endl;
			cout << "----controlNode->poleId-------" << controlNode->poleId << "------------------------" << endl;
			cout << "-----output0------------------" << output0 << "----------------------------" << endl;
		}
		else
		{
			cout << "-------------------  T T ------------------------" << endl;
			cout << "----controlNode->gateType------" << controlNode->gateType << "------------------------" << endl;
			cout << "----controlNode->poleId-------" << controlNode->poleId << "------------------------" << endl;
			cout << "-----output0------------------" << output0 << "----------------------------" << endl;
		}
		inputReady1 = false;
		inputReady0 = false;
	}
	*/

	return SimpleGate::generateOutput();
}

void SimpleUniversalGate::clear()
{
	SimpleGate::clear();
}

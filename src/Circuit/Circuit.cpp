//#include "stdafx.h"
#include "Circuit.h"


void Circuit::calculate()
{
	for (size_t i = 0; i < gateDoubleFixInList.size(); i++)
	{
		gateDoubleFixInList[i]->GenerateAllPostOutput();
	}
}

void Circuit::setReady()
{
	for (size_t i = 0; i < gateSingleFixInList.size(); i++)
	{
		gateSingleFixInList[i]->inputReady1 = true;
	}
	for (size_t i = 0; i < gateDoubleFixInList.size(); i++)
	{
		gateDoubleFixInList[i]->inputReady0 = true;
		gateDoubleFixInList[i]->inputReady1 = true;
	}
}

Circuit::Circuit(size_t inFixNum, size_t outFixNum, size_t inGateNum, size_t outGateNum)
{
	inFixSize = inFixNum;
	outFixSize = outFixNum;
	inGateSize = inGateNum;
	outGateSize = outGateNum;
	inFixList = new bool[inFixSize];
	outFixList = new bool*[outFixSize];
	inGateList = new Gate*[inGateSize];
	outGateList = new Gate*[outGateSize];
}

Circuit::~Circuit()
{
	delete[] inFixList;
	delete[] outFixList;
	delete[] inGateList;
	delete[] outGateList;
}

void Circuit::clear()
{
	for (size_t i = 0; i < gateList.size(); i++)
	{
		gateList[i]->clearInput();
	}
}

Circuit * Circuit::getXSwitch()
{
	return nullptr;
}

Circuit * Circuit::getYSwitch()
{
	return nullptr;
}

Circuit * Circuit::getInvYSwitch()
{
	return nullptr;
}

Circuit * Circuit::getUniversalGate()
{
	return nullptr;
}

Circuit * Circuit::getSuperNode2()
{
	return nullptr;
}

Circuit * Circuit::getPseudoSuperNode2()
{
	return nullptr;
}

Circuit * Circuit::getUniversalCircuit(uint32_t N)
{
	return nullptr;
}


Circuit * Circuit::generateToyXSwitch()
{
	Circuit* circuit = new Circuit(3u,2u,0u,0u);

	Gate* xor0 = new Gate(GateType::GATE_TYPE_XOR, &circuit->inFixList[0], &circuit->inFixList[1]);
	circuit->gateList.push_back(xor0);
	circuit->gateDoubleFixInList.push_back(xor0);
	
	Gate* and0 = new Gate(GateType::GATE_TYPE_AND, xor0, &circuit->inFixList[2]);
	circuit->gateList.push_back(and0);
	circuit->gateSingleFixInList.push_back(and0);

	Gate* xor1 = new Gate(GateType::GATE_TYPE_XOR, and0, &circuit->inFixList[0]);
	circuit->gateList.push_back(xor1);
	circuit->gateSingleFixInList.push_back(xor1);

	Gate* xor2 = new Gate(GateType::GATE_TYPE_XOR, and0, &circuit->inFixList[1]);
	circuit->gateList.push_back(xor2);
	circuit->gateSingleFixInList.push_back(xor2);

	circuit->outFixList[0] = &xor1->output;
	circuit->outFixList[1] = &xor2->output;

	return circuit;
}


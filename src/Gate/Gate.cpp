//#include "stdafx.h"
#include "Gate.h"


Gate::Gate(GateType type, Gate * inGate0, Gate * inGate1)
{
	setGateType(type); // Set the type and truth table of gate

	inputGate0 = inGate0;
	gateMode0 = GateMode::GATE_MODE_GATEIN;
	input0 = new bool;
	inGate0->postGateList0.push_back(this); // Add this gate to the postGateList0 of its inputGates

	inputGate1 = inGate1;
	gateMode1 = GateMode::GATE_MODE_GATEIN;
	input1 = new bool;
	inGate1->postGateList1.push_back(this); // Add this gate to the postGateList1 of its inputGates

	output = false;
	hasGeneratedOutput = false;

	inputReady0 = false;
	inputReady1 = false;
}

Gate::Gate(GateType type, Gate* inGate0, bool* inBool1)
{
	setGateType(type);

	inputGate0 = inGate0;
	gateMode0 = GateMode::GATE_MODE_GATEIN;
	input0 = new bool;
	inGate0->postGateList0.push_back(this); // Add this gate to the postGateList0 of its inputGates

	inputGate1 = nullptr;
	gateMode1 = GateMode::GATE_MODE_FIXIN;
	input1 = inBool1;
	
	output = false;
	hasGeneratedOutput = false;

	inputReady0 = false;
	inputReady1 = false;
}

Gate::Gate(GateType type, bool* inBool0, bool* inBool1)
{
	setGateType(type);

	inputGate0 = nullptr;
	gateMode0 = GateMode::GATE_MODE_FIXIN;
	input0 = inBool0;

	inputGate1 = nullptr;
	gateMode1 = GateMode::GATE_MODE_FIXIN;
	input1 = inBool1;

	output = false;
	hasGeneratedOutput = false;

	inputReady0 = false;
	inputReady1 = false;
}

void Gate::setGateType(GateType type)
{
	this->gateType = type;
	switch (type)
	{
		case GATE_TYPE_AND:
			this->truthTable = Gate::truthTableAnd;
			break;
		case GATE_TYPE_XOR:
			this->truthTable = Gate::truthTableXor;
			break;
		default:
			this->truthTable = Gate::truthTableFalse;
	}
}

void Gate::GenerateOutput()
{
	if (inputReady0 && inputReady1 == false)
	{
#ifdef DEBUG
		cout << "Input not ready" << endl;
#endif // DEBUG
		return;
	}

	if (gateMode0 == GateMode::GATE_MODE_GATEIN)
	{
		(*input0) = inputGate0->output;
	}
	if (gateMode1 == GateMode::GATE_MODE_GATEIN)
	{
		(*input1) = inputGate1->output;
	}

	output = truthTable(*input0, *input1);
	hasGeneratedOutput = true;

	for (size_t i = 0; i < postGateList0.size(); i++)
	{
		postGateList0[i]->inputReady0 = true;
	}
	for (size_t i = 1; i < postGateList1.size(); i++)
	{
		postGateList1[i]->inputReady1 = true;
	}
}

void Gate::GenerateAllPostOutput()
{
	if (inputReady0 && inputReady1 == false)
	{
#ifdef DEBUG
		cout << "Input not ready" << endl;
#endif // DEBUG
		return;
	}

	if (gateMode0 == GateMode::GATE_MODE_GATEIN)
	{
		(*input0) = inputGate0->output;
	}
	if (gateMode1 == GateMode::GATE_MODE_GATEIN)
	{
		(*input1) = inputGate1->output;
	}

	output = truthTable(*input0, *input1);
	hasGeneratedOutput = true;

	for (size_t i = 0; i < postGateList0.size(); i++)
	{
		postGateList0[i]->inputReady0 = true;
		postGateList0[i]->GenerateAllPostOutput();
	}
	for (size_t i = 1; i < postGateList1.size(); i++)
	{
		postGateList1[i]->inputReady1 = true;
		postGateList1[i]->GenerateAllPostOutput();
	}
}


Gate::~Gate()
{
	if (gateMode0 == GATE_MODE_GATEIN)
	{
		delete input0;
	}
	if (gateMode1 == GATE_MODE_GATEIN)
	{
		delete input1;
	}
}

void Gate::clearGateInput()
{
	if (gateMode0 == GATE_MODE_GATEIN)
	{
		inputReady0 = false;
	}
	if (gateMode1 == GATE_MODE_GATEIN)
	{
		inputReady1 = false;
	}

	output = false;
	hasGeneratedOutput = false;
}

void Gate::clearInput()
{
	inputReady0 = false;

	inputReady1 = false;

	output = false;
	hasGeneratedOutput = false;
}

bool Gate::truthTableAnd(bool input0, bool input1)
{
	return input0 && input1;
}

bool Gate::truthTableXor(bool input0, bool input1)
{
	return input0 ^ input1;
}

bool Gate::truthTableFalse(bool input0, bool input1)
{
	return false;
}



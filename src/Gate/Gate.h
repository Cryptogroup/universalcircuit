//#pragma once
#ifndef _Gate_h_
#define _Gate_h_

#include <vector>


#include <iostream>


using namespace std;

enum GateMode
{
	GATE_MODE_FIXIN,
	GATE_MODE_GATEIN,
};

enum GateType
{
	GATE_TYPE_AND,
	GATE_TYPE_XOR,
	GATE_TYPE_UNKNOWN
};

class Gate
{
public:
	GateType gateType;
	Gate* inputGate0;
	Gate* inputGate1;
	GateMode gateMode0;
	GateMode gateMode1;
	bool* input0;
	bool* input1;
	bool inputReady0;
	bool inputReady1;
	bool hasGeneratedOutput;
	bool output;
	vector<Gate*> postGateList0;
	vector<Gate*> postGateList1;
	bool(*truthTable)(bool, bool); // The value of this function pointer is decided by gateType

	// Different ways of input, get input from a previous gate or a fixed bool variable
	Gate(GateType type, Gate* inGate0, Gate* inGate1);
	Gate(GateType type, Gate* inGate0, bool* inBool1); // bool is just a mark of mode
	Gate(GateType type, bool* inBool0, bool* inBool1);
	void setGateType(GateType type); // Set the right function to be the truth table

	void GenerateOutput(); // Compute output if (inputReady0 & inputReady1) == true
	void GenerateAllPostOutput();

	virtual ~Gate();
	
	void clearGateInput(); // Will not clear input for post-gates
	void clearInput(); // Will not clear  input for post-gates

	static bool truthTableAnd(bool input0, bool input1);
	static bool truthTableXor(bool input0, bool input1);
	static bool truthTableFalse(bool input0, bool input1);
};

#endif
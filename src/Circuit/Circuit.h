//#pragma once
#ifndef _Circuit_h_
#define _Circuit_h_


#include <cstdint>


#include <vector>



#include "../Gate/Gate.h"


#include <iostream>

using namespace std;

class Circuit
{
public:
	size_t inFixSize;
	size_t outFixSize;
	size_t inGateSize;
	size_t outGateSize;
	vector<Gate*> gateList;
	vector<Gate*> gateSingleFixInList;
	vector<Gate*> gateDoubleFixInList;
	bool* inFixList;
	bool** outFixList;
	Gate** inGateList;
	Gate** outGateList;

	void calculate();
	void setReady();

	Circuit(size_t inFixNum, size_t outFixNum, size_t inGateNum, size_t outGateNum);
	~Circuit();

	void clear(); // clear input of all gates

	// small components
	static Circuit* getXSwitch();
	static Circuit* getYSwitch();
	static Circuit* getInvYSwitch();
	static Circuit* getUniversalGate();

	// big components
	static Circuit* getSuperNode2();
	static Circuit* getPseudoSuperNode2();
	static Circuit* getUniversalCircuit(uint32_t N); // Get a universal circuit with 2^N gates


	static Circuit* generateToyXSwitch();

};
#endif

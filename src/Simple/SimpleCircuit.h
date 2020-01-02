//#pragma once
#ifndef _SimpleCircuit_h_
#define _SimpleCircuit_h_

#include "../UC.h"
#include "../Eu/EugComponent.h"
#include <vector>
#include <cstdint>
#include <iostream>

#include "SimpleGate.h"
#include "SimpleXSwitch.h"
#include "SimpleUniversalGate.h"

using namespace std;
//class EugNode;
class UC;
//class SimpleGate;
//class EugComponent;

enum SimpleCircuitType
{
	SIMPLE_CIRCUIT_SN1,
	SIMPLE_CIRCUIT_SN2,
	SIMPLE_CIRCUIT_PSEUDO_SN1,
	SIMPLE_CIRCUIT_PSEUDO_SN2,
	SIMPLE_CIRCUIT_UC,
	SIMPLE_CIRCUIT_CONTROLLER,
	SIMPLE_CIRCUIT_DEFAULT,
};

class SimpleCircuit
{
public:
	SimpleCircuitType simpleCircuitType;
	vector<SimpleGate*> simpleGateList; // used when linking to other simple circuit component and deconstruct

	SimpleCircuit(SimpleCircuitType type);
	SimpleCircuit(uint32_t m, uint32_t * from1, uint32_t * from2, bool * output);
	vector<SimpleGate*> switchList;
	vector<SimpleGate*> supList;
	vector<SimpleUniversalGate*> universalGateList;
	~SimpleCircuit();

	static SimpleCircuit* getSimpleSuperNode2(EugNode* pole0, EugNode* pole1, EugNode* normalLeft0,
		EugNode* normalLeft1, EugNode* normalLeft2, EugNode* normalRight0, EugNode* normalRight1, EugNode* normalRight2);
	static SimpleCircuit* getSimpleSuperNode1(EugNode * pole0);
	static SimpleCircuit* getSimplePseudoSuperNode2(EugNode* normal0, EugNode* normal1, EugNode* normal2);
	static SimpleCircuit* getSimplePseudoSuperNode1(EugNode * pole0);
	static SimpleCircuit* getSimpleUnivseralCircuit(uint32_t t, UC * uc);
	static SimpleCircuit* getSimpleUnivseralCircuit(UC* uc);
	static SimpleCircuit* getSimpleController(EugComponent* eug1); // Get a controller to control UC of layer N+1

	static void testUC(); // a sample circuit to test whether the gates works fine or not


	static void testCircuit(); // a sample circuit to test whether the gates works fine or not

};
#endif

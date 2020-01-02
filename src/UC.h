/*
	The Eug whose fan-in and fan-out is 2 made up by 2 Eug1_2N
	This data structure will be used to controll the universal circuit directly
*/
#ifndef _UC_H_
#define _UC_H_
//#pragma once
#include <math.h>
#include <string>
#include <fstream>
#include <sstream>

#include "Dag/DAG.h"
#include "Dag/DagNode.h"
#include "Eu/EugNode.h"
#include "Eu/EugComponent.h"
#include "Circuit/Circuit.h"
#include "Simple/SimpleCircuit.h"
#include "Simple/SimpleGate.h"
#include "Simple/SimpleUniversalGate.h"
using namespace std;
class SimpleUniversalGate;
class SimpleCircuit;

//class SimpleGate;
class UC
{
public:
	uint32_t* from;
	uint32_t* to;
	uint32_t* gate;
	uint32_t layerCount;
	uint32_t nodeNum;
	EugComponent* SubEug0;
	EugComponent* SubEug1;
	bool hasSetCircuit;
	bool hasGeneratedOutput;
	Circuit* universalCircuit;
	SimpleCircuit* simpleUniversalCircuit;
	vector<SimpleUniversalGate*>* universalGates;
	vector<SimpleGate*> Gates;
	vector<SimpleGate*> inputGates;
	vector<SimpleGate*> outputGates;
	vector<SimpleGate*>* switches;
	vector<SimpleGate*>* supSwiches;
	vector<SimpleGate*> midGates;

	Dag** GetCircuitFromFile(string fileName, FileType fileType);// Get a Dag2 with a .circuit file
	Dag** CircuitFileParser(string fileName);
	void GetDags(size_t Dagsize, uint32_t location, uint32_t* from, uint32_t* to, uint32_t* gate, Dag** dags);
	UC(uint32_t N); // Constuct a Eug that max fan-in and max fan-out are 2 with 2^N poles
	UC(uint32_t t, uint32_t m);
	~UC();

	void setCircuit(uint32_t m, uint32_t * from1, uint32_t * from2);

	void setCircuit(string fileName, FileType fileType);
	void generateSimpleCircuit();

	void setInput(uint32_t m, uint32_t * from1, uint32_t * from2);

	void setInput(bool* input, size_t inputSize,string filename,uint32_t gatenum);
	bool* getOutput();
};
#endif
//#pragma once
#ifndef _SimpleGate_h_
#define _SimpleGate_h_
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "../Eu/EugNode.h"
//#include "../Eu/EugComponent.h"

using namespace std;

class EugNode;

//class EugNode;

struct Node4Struct{
	// Identify the node to which it is connected
	int nextnodeA ;
	int nextnodeB ;
	int prevnodeA ;
	int prevnodeB ;
	//Identifies the edge connected to this node
	int nextSideA ;
	int nextSideB ;
	int prevSideA ;
	int prevSideB ;
	//Identifies the type of the node
	//0:Normal Node 1:Input Node -1:Output Node
	long long nodeid;
	bool controlflag;
};


class SimpleGate
{
public:
	const bool* input0;
	const bool* input1;
	bool inputReady0;
	bool inputReady1;
	bool output0;
	bool output1;
	bool isSN1;
	bool hasGeneratedOutput;
	EugNode* controlNode;

	const static bool TRUE = true;
	const static bool FALSE = false;
	
	size_t hasinput0;
	size_t hasinput1;


	vector<SimpleGate*> postGateList0;
	vector<SimpleGate*> postGateList1;
	
	virtual bool generateOutput();
	virtual void generateOutputChain(vector<Node4Struct> &nodelist);
	virtual void clear();

	explicit SimpleGate(EugNode* controlNode);
	virtual ~SimpleGate();
};

#endif

//#pragma once
#ifndef _DAG_h_
#define _DAG_h_

#include <iostream>
#include <cstdint>
#include <vector>

#include "DagNode.h"

using namespace std;

enum FileType
{
	FILE_TYPE_CIRCUIT
};

class Dag
{
public:
	vector<DagNode*> nodeList;
	vector<DagNode*> headList; // headList is a subset of nodeList
	size_t maxFanIn; // max in-degree for every node
	size_t maxFanOut; // max out-degree for every node

	Dag(size_t maxFanIn, size_t maxFanOut);
	Dag(size_t maxFanIn, size_t maxFanOut, size_t size); // Warning: the heads must be set mannually
	Dag(size_t maxFanIn, size_t maxFanOut, size_t size, uint32_t* from, uint32_t* to);
	~Dag();
	
	size_t getHeadSize() const;
	size_t getSize() const;

	void print();

	DagNode* getNode(uint32_t id) const; // Get the ptr of a node by its id

	bool addHead(DagNode* nodePtr);
	bool addNode(DagNode* nodePtr);
	bool addEdge(uint32_t idFrom, uint32_t idTo);
};
#endif

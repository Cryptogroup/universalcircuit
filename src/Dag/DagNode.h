//#pragma once
#ifndef _DagNode_h_
#define _DagNode_h_

#include <iostream>

using namespace std;

enum DagNodeType
{
	DAG_NODE_NORMAL,
	DAG_NODE_HEAD,
	DAG_NODE_TAIL,
	DAG_NODE_DEFAULT
};

class DagNode
{
public:
	uint32_t id;
	DagNode** posts;
	DagNodeType type;
	size_t fanIn;
	size_t fanOut;
	uint32_t gateType;

	DagNode(uint32_t id, DagNodeType type);
	~DagNode();

	bool hasConnecetedTo(uint32_t id);
	uint32_t* getPostNodeIds();
	size_t getPostNum();
};

#endif

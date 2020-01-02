//#include "stdafx.h"
#include "DagNode.h"


DagNode::DagNode(uint32_t id, DagNodeType type)
{
	this->id = id;
	this->type = type;
	posts = new DagNode*[2];
	posts[0] = nullptr;
	posts[1] = nullptr;
	fanIn = 0;
	fanOut = 0;
	gateType = UINT32_MAX;
}

DagNode::~DagNode()
{
	delete[] posts;
}

bool DagNode::hasConnecetedTo(uint32_t id)
{
	if ((posts[0] != nullptr&&posts[0]->id == id) || (posts[1] != nullptr&&posts[1]->id == id))
		return true;
	else
		return false;
}

uint32_t * DagNode::getPostNodeIds()
{
	uint32_t* ids = new uint32_t[fanOut];
	
	for (size_t i = 0; i < fanOut; i++)
	{
		ids[i] = posts[i]->id;
	}

	return ids;
}

size_t DagNode::getPostNum()
{
	return fanOut;
}


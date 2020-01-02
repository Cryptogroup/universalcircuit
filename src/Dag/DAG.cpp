//#include "stdafx.h"
#include "DAG.h"


Dag::Dag(size_t maxFanIn, size_t maxFanOut)
{
	this->maxFanIn = maxFanIn;
	this->maxFanOut = maxFanOut;
}

Dag::Dag(size_t maxFanIn, size_t maxFanOut, size_t size)
{
	this->maxFanIn = maxFanIn;
	this->maxFanOut = maxFanOut;

	DagNode** nodes = new DagNode*[size];
	for (size_t i = 0; i < size; i++)
	{
		nodes[i] = new DagNode(uint32_t(i), DagNodeType::DAG_NODE_DEFAULT);
		this->nodeList.push_back(nodes[i]);
	}

	delete[] nodes;
}

Dag::Dag(size_t maxFanIn, size_t maxFanOut, size_t size, uint32_t* from, uint32_t* to)
{
	this->maxFanIn = maxFanIn;
	this->maxFanOut = maxFanOut;

	DagNode** nodes = new DagNode*[size];
	for (size_t i = 0; i < size; i++)
	{
		nodes[i] = new DagNode(uint32_t(i), DagNodeType::DAG_NODE_DEFAULT);
		this->nodeList.push_back(nodes[i]);
	}

	for (uint32_t i = 0; i < size; i++)
	{
		if(from[i]<to[i])
			this->addEdge(from[i], to[i]);
	}

	for (uint32_t i = 0; i < size; i++)
	{
		if (this->nodeList[i]->posts[0] == nullptr)
			this->headList.push_back(this->nodeList[i]);
	}

	delete[] nodes;
}

Dag::~Dag()
{
	for (size_t i = 0; i < nodeList.size(); i++)
	{
		delete nodeList[i];
	}
}

size_t Dag::getHeadSize() const
{
	return headList.size();
}

size_t Dag::getSize() const
{
	return nodeList.size();
}

void Dag::print()
{
	
	cout << "Outputing DAG..." << endl;
	for (size_t i = 0; i < nodeList.size(); i++)
	{
		if (nodeList[i]->posts[0] == nullptr)
			continue;
		for(size_t j=0;;j++)
		{ 
			if (nodeList[i]->posts[j] != nullptr)
				cout << i << " -> " << nodeList[i]->posts[0]->id << endl;
			else
				break;
		}
	}

}

DagNode* Dag::getNode(uint32_t id) const
{
	return nodeList[id];
}

bool Dag::addHead(DagNode * nodePtr)
{
	this->headList.push_back(nodePtr);
	this->nodeList.push_back(nodePtr);

	return true;
}

bool Dag::addNode(DagNode * nodePtr)
{
	this->nodeList.push_back(nodePtr);

	return true;
}

bool Dag::addEdge(uint32_t idFrom, uint32_t idTo)
{
	DagNode* nodeFrom = getNode(idFrom);
	DagNode* nodeTo = getNode(idTo);

	if (nodeFrom->fanOut >= maxFanOut)
	{
		//cout << "The Pre-Node " << nodeFrom->id << "'s fanOut has reached maxFanOut!" << endl;
		return false;
	}
	if (nodeTo->fanIn >= maxFanIn)
	{
		return false;
		//cout << "The post-Node " << nodeTo->id << "'s fanIn has reached maxFanInt!" << endl;
	}
	nodeFrom->fanOut += 1;
	nodeTo->fanIn += 1;
	//printf("Link %d to %d.\n", nodeFrom->id, nodeTo->id);
	if (nodeFrom->posts[0] == nullptr)
	{
		nodeFrom->posts[0] = nodeTo;
		//cout << "write in 0" << endl;
	}
	else
	{
		nodeFrom->posts[1] = nodeTo;
		//printf("%d,  posts[1]=%d\n", nodeFrom->id, nodeFrom->posts[1]->id);
	}

	return true;
}

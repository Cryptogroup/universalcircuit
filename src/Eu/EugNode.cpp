//#include "stdafx.h"
#include "EugNode.h"


uint32_t EugNode::getNextId()
{
	return idCounter++;
}

EugNode::EugNode(EugNodeType type, EugComponent * component, uint32_t poleId = UINT32_MAX)
{
	this->type = type;
	this->poleId = poleId;
	this->component = component;

	this->id = getNextId();

	posts = new EugNode*[2];
	posts[0] = nullptr;
	posts[1] = nullptr;

	pres = new EugNode*[2];
	pres[0] = nullptr;
	pres[1] = nullptr;

	lastPole = nullptr;
	nextPole = nullptr;

	gateType = UINT32_MAX;
	dagNodeType = DagNodeType::DAG_NODE_DEFAULT;
	controlledGate = nullptr;

	this->fanIn = 0;
	this->fanOut = 0;

	this->control = false;

	EugComponent::allNodes.push_back(this);
}

EugNode::~EugNode()
{
	delete[] pres;
	delete[] posts;
}

bool EugNode::linkTo(EugNode * post)
{
	if (this->fanOut >= 2)
	{
		cout << "Failed to add a post-node to a node whose fanOut >= 2" << endl;
		return false;
	}
	if (post->fanIn >= 2)
	{
		cout << "Failed to add a pre-node to a node whose fanIn >= 2" << endl;
		return false;
	}

	this->fanOut++;
	post->fanIn++;
	if (this->posts[0] == nullptr)
		this->posts[0] = post;
	else
		this->posts[1] = post;

	if (post->pres[0] == nullptr)
		post->pres[0] = this;
	else
		post->pres[1] = this;

	return true;
}

bool EugNode::unlink()
{
	EugNode* remove = nullptr;
	if (this->posts[1] != nullptr)
	{
		remove = posts[1];
		this->posts[1] = nullptr;
	}
	else if (this->posts[0] != nullptr)
	{
		remove = posts[0];
		this->posts[0] = nullptr;
	}

	if (remove != nullptr)
	{
		if (remove->pres[0]->id == this->id)
		{
			remove->pres[0] = remove->pres[1];
			remove->pres[1] = nullptr;
		}
		else
		{
			remove->pres[1] = nullptr;
		}

		remove->fanIn--;
		this->fanOut--;
		return true;
	}
	else
	{
		//cout << "Failed to remove a post-node from a node whose fanIn == 0" << endl;
		return false;
	}
}

bool EugNode::unlink(size_t which)
{
	if (which == 0 && posts[0] != nullptr)
	{
		if (posts[0]->pres[0]->id == this->id)
		{
			posts[0]->pres[0] = posts[0]->pres[1];
			posts[0]->pres[1] = nullptr;
		}
		else
		{
			posts[0]->pres[1] = nullptr;
		}

		posts[0]->fanIn--;
		this->fanOut--;

		this->posts[0] = this->posts[1];
		this->posts[1] = nullptr;

		return true;
	}
	if (which == 1 && posts[1] != nullptr)
	{
		if (posts[1]->pres[0]->id == this->id)
		{
			posts[1]->pres[0] = posts[1]->pres[1];
			posts[1]->pres[1] = nullptr;
		}
		else
		{
			posts[1]->pres[1] = nullptr;
		}

		posts[0]->fanIn--;
		this->fanOut--;

		this->posts[1] = nullptr;

		return true;
	}

	return false;
}

EugNode * EugNode::getInNode(uint32_t inId, EugComponent* component)
{
	return new EugNode(EugNodeType::EUG_NODE_IN, component);
}

EugNode * EugNode::getOutNode(uint32_t outId, EugComponent* component)
{
	return new EugNode(EugNodeType::EUG_NODE_OUT, component);
}

EugNode * EugNode::getNormalNode(EugComponent* component)
{
	return new EugNode(EugNodeType::EUG_NODE_NORMAL, component);
}

EugNode * EugNode::getPoleNode(EugComponent* component, uint32_t poleId)
{
	return new EugNode(EugNodeType::EUG_NODE_POLE, component, poleId);
}

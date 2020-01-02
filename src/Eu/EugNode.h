

//#pragma once
#ifndef _EugNode_h_
#define _EugNode_h_

#include "EugComponent.h"

#include <iostream>
#include <cstdint>

#include "../Simple/SimpleGate.h"
#include "../Dag/DagNode.h"

using namespace std;
class SimpleGate;
class EugComponent;

enum EugNodeType
{
	EUG_NODE_POLE,
	EUG_NODE_NORMAL,
	EUG_NODE_IN,  // Has no unified code
	EUG_NODE_OUT, // Has no unified code
	EUG_NODE_DEFAULT,
};

//class SimpleGate;

class  EugNode
{
	public:
		EugNodeType type;
		uint32_t id;
		uint32_t poleId;
		EugComponent* component;
		size_t fanIn;
		size_t fanOut;
		EugNode** posts; // Post-nodes in EUG
		EugNode** pres; // Pre-nodes in EUG
		EugNode* nextPole;
		EugNode* lastPole;
		DagNodeType dagNodeType;

		// variabls for exact circuit file type
		SimpleGate* controlledGate;

		// variable for computation after edge embedding
		bool control;
		uint32_t gateType;

		static uint32_t idCounter;
		static uint32_t getNextId();

		EugNode(EugNodeType type, EugComponent* component, uint32_t poleId);
		~EugNode();

		bool linkTo(EugNode* post);
		bool unlink();
		bool unlink(size_t which);

		static EugNode* getInNode(uint32_t inId, EugComponent* component);
		static EugNode* getOutNode(uint32_t outId, EugComponent* component);
		static EugNode* getNormalNode(EugComponent* component);
		static EugNode* getPoleNode(EugComponent* component, uint32_t poleId);
};
#endif



#ifndef _EugComponent_h_
#define _EugComponent_h_
#include "EugNode.h"

//#include "../Simple/SimpleGate.h"
#include "../Dag/DAG.h"
#include "math.h"
#include "../Dag/DagNode.h"
#include <set>
#include <iostream>



//#pragma once


using namespace std;



enum Sn2InStyle
{
	IN_STYLE_0, // In0 goes to Pole0
	IN_STYLE_1, // In1 goes to Pole0
	IN_STYLE_UNDECIDED,
	IN_STYLE_NONESN2
};

enum Sn2PoleStyle
{
	POLE_STYLE_CASCADE, // Pole0 goes to Pole1
	POLE_STYLE_PARALLEL, // Pole0 goes to Out Directly
	POLE_STYLE_UNDECIDED,
	POLE_STYLE_NONESN2
};

enum Sn2OutStyle
{
	OUT_STYLE_0, // Pole0 goes to Out0
	OUT_STYLE_1, // Pole0 goes to Out1
	OUT_STYLE_UNDECIDED,
	OUT_STYLE_NONESN2
};
class EugNode;
class EugComponent
{
public:
	vector<EugNode*> poleList;
	vector<EugNode*> pseudopoleList;
	vector<EugNode*> inList;
	vector<EugNode*> outList;
	vector<EugNode*> normalList;
	vector<EugComponent*> subCompList;
	vector<EugComponent*> sn2List;
	size_t inNum;
	size_t outNum;
	//size_t specialNum;
	uint32_t compId;
	size_t layer;
	Sn2InStyle inStyle;
	Sn2PoleStyle poleStyle;
	Sn2OutStyle outStyle;
	bool isSn2;
	Dag* myDag;

	vector<uint32_t> from;
	vector<uint32_t> to;

	static uint32_t compIdCounter;
	static uint32_t getCompNextId();

	static uint32_t maxLayer;

	// TODO: Implement this function
	void removePoles(); // Romove useless poles in subDags


	void printEdges(/* Node4Side *NodeStruct*/);
	void printPoles();
	void printIns();


	void setDag(Dag* dag);
	void edgeEmbed(uint32_t pre, uint32_t post, size_t location, Dag* Dag1, Dag* Dag2);
	void computeControl();

	uint32_t** getAdjointMatrix();
	void updateUsedAdjointMatrix();

	EugComponent(size_t inNum, size_t outNum, size_t layer, bool isSn2, Sn2InStyle sn2InState, Sn2PoleStyle sn2PoleState, Sn2OutStyle sn2OutState, uint32_t compId);
	~EugComponent();

	static EugComponent* getSuperNode1();

	static EugComponent* getSuperNode2();
	static EugComponent* getEug1(uint32_t N);
	static EugComponent* getEug_m(uint32_t m);
	static EugComponent* getEug1_2N(uint32_t N);
	static EugComponent* getEug1_m(uint32_t m);

	static vector<EugComponent*> allSn2s;
	static vector<EugNode*> allNodes;
	static uint32_t** usedMatrix;
};

#endif


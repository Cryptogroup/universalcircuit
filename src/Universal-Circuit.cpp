// Universal-Circuit.cpp
//
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <cstdlib>
#include <math.h>
#include "Dag/Dag.h"
#include "Eug/EugNode.h"
#include "Eug/EugComponent.h"
#include "Circuit/Circuit.h"
#include "Gate/Gate.h"
#include "UC.h"
#include "Simple/SimpleCircuit.h"
#define DEBUG
using namespace std;

uint32_t EugNode::idCounter = 0u;
uint32_t EugComponent::compIdCounter = 0u;
uint32_t EugComponent::maxLayer = 0u;
vector<EugNode*> EugComponent::allNodes;
uint32_t** EugComponent::usedMatrix;

void Eug1Test();
void circuitTest();
void UniversalCircuitTest();
void testSampleSimpleCircuit();

uint32_t* getPerm(int n, int seed);
void testUC(uint32_t n, uint32_t seed1, uint32_t seed2);
bool* getGateOutput(uint32_t n,uint32_t m, uint32_t* perm1, uint32_t* perm2);
bool* getGateOutput(uint32_t m, uint32_t* perm1, uint32_t* perm2);

int main(int argc, char* argv[])
{
	/**
	uint32_t m = uint32_t(atoi(argv[1]));
	uint32_t seed1 = uint32_t(atoi(argv[2]));
	uint32_t seed2 = uint32_t(atoi(argv[3]));
	*/



	//uint32_t m=10;
	//uint32_t seed1 = 123;
	//uint32_t seed2 = 1233;
	if(argc!=3)
		cout<<"Input format: - filename.txt"<<endl;
	string gatefilename;
	gatefilename = argv[2];
	cout<<"Input filename is "<<gatefilename<<endl; 
	
	UniversalCircuitTest();//
	
	//Eug1Test();
	
	//circuitTest();
    
	//testSampleSimpleCircuit();
	/**
	m = 215;
	cout << "m = " << m << endl;
	

	for (seed1 = 18; seed1 < 20; seed1++) {
		for (seed2 = seed1+1; seed2 < 20; seed2++) {
			cout << "seed1 = " << seed1 << endl;
			cout << "seed2 = " << seed2 << endl;
			//testUC(m, seed1, seed2);//test the correctness of UC
			testUC(m, seed1-10, seed2-10);//test the correctness of UC
		}
	}
	*/
	
	//testUC(m, seed1, seed2);//test the correctness of UC


	cout << "Test Finished." << endl;
	return 0;
}


void testUC(uint32_t m, uint32_t seed1, uint32_t seed2)
{
	if (seed1 % 1001 == 1) 
	{
		cout << "seed 1   = " << seed1 << endl;
	}

	uint32_t* perm1 = getPerm(m, seed1);
	uint32_t* perm2 = getPerm(m, seed2);
	
	/**
	cout << "Permutation1 : " << endl;
	for (uint32_t i = 0; i < m; i++)
	{
		cout << perm1[i] << " ";
	}
	cout << endl << "Permutation2 :" << endl; 
	for (uint32_t i = 0; i < m; i++)
	{
		cout << perm2[i] << " ";
	}
	cout << endl;
	*/

	bool* output1 = new bool[m];
	bool* output2 = new bool[m];

	SimpleCircuit* circuit = new SimpleCircuit(m, perm1, perm2, output1);
	
	output2 = getGateOutput(m, perm1, perm2);

	/**
	for (uint32_t i = 0; i < m; i++) {
		if (output1[i] == output2[i])
			cout << "output = " << output1[i] << "  " << output2[i] << endl;
	}
	
	
	cout << "========== output1 not equal output2=============" << endl;
	*/

	for (uint32_t i = 0; i < m; i++) 
	{
		if (output1[i] != output2[i]){
			cout << " i =   " << i << endl;
			cout << " seed1  =  " << seed1 << endl;
			cout << " seed2  =  " << seed2 << endl;
			
			cout << " not output = " << output1[i] << "  " << output2[i] << endl;
		}
	}
	cout << "UC Test Passed." << endl;
}

bool* getGateOutput(uint32_t n,uint32_t m, uint32_t* from1, uint32_t* from2) 
{
	UC* uc = new UC(n);

	uc->setCircuit(m, from1, from2);

	uc->generateSimpleCircuit();

	uc->setInput(m,from1,from2);

	bool* output = new bool[32];
	for (uint32_t i = 0; i < m; i++) 
	{
		output[i] = uc->SubEug0->poleList[i]->controlledGate->output0;
	}
	return output;
}

bool* getGateOutput(uint32_t m, uint32_t* from1, uint32_t* from2)
{
	UC* uc = new UC(1,m);

	uc->setCircuit(m, from1, from2);

	uc->generateSimpleCircuit();

	uc->setInput(m, from1, from2);

	bool* output = new bool[32];
	for (uint32_t i = 0; i < m; i++)
	{
		output[i] = uc->SubEug0->poleList[i]->controlledGate->output0;
	}
	return output;
}


void UniversalCircuitTest(string *filename)
{
	string fileName = "C:\\Users\\hanli\\Documents\\GitHub\\Universal-Circuit2\\Universal-Circuit\\Test7Gate.circuit";

	UC* uc = new UC(1,7); // get UC
	
	bool* input = new bool[4];
	input[0] = true;
	input[1] = false;
	input[2] = true;
	input[3] = true;
	//input[4] = false;
	//input[5] = true;
	//input[6] = false;
	//input[7] = false;
	uc->setCircuit(fileName, FileType::FILE_TYPE_CIRCUIT); //set the value of X- and Y-Gate



	uc->generateSimpleCircuit();

	uc->setInput(input, 4);
	bool* output = uc->getOutput();

	for (int i = 0; i < uc->midGates.size(); i++) {
		if (uc->midGates[i]->hasinput0 > 1) {
			cout << "+++++++++++ uc->midGates[i]->controlNode->id +++++++++++++++++++++"<< uc->midGates[i]->controlNode->id << endl;
		}
		if (uc->midGates[i]->hasinput1 > 1) 
		{
			cout << "+++++++++++ uc->midGates[i]->controlNode->id +++++++++++++++++++++" << uc->midGates[i]->controlNode->id << endl;
		}
		
	}

	for (int i = 0; i < uc->Gates.size(); i++) {
		if (uc->Gates[i]->hasinput0 > 0) {
			cout << "+++++++++++ uc->Gates[i]->controlNode->id +++++++++++++++++++++" << uc->Gates[i]->controlNode->id << endl;
		}
		if (uc->Gates[i]->hasinput1 > 1)
		{
			cout << "+++++++++++ uc->Gates[i]->controlNode->id +++++++++++++++++++++" << uc->Gates[i]->controlNode->id << endl;
		}
	}

	//cout << "Input size of uc: " << uc->inputGates.size() << endl;
	//cout << "Input: " << input[0] << " " << input[1]<< " " << input[2] << " " << input[3] << endl;

	/*
	for (size_t i = 0; i < uc->SubEug0->poleList.size(); i++)
	{
		cout << "++++++++++++++++++++++++++++++" << endl;
		cout << "Id = " << i << endl;
		cout << "Type: ";
		switch (uc->SubEug0->poleList[i]->dagNodeType)
		{
		case DAG_NODE_HEAD:
			cout << "Input" << endl;
			cout << "Output: " << uc->SubEug0->poleList[i]->controlledGate->output0 << endl;
			break;
		case DAG_NODE_NORMAL:
			cout << "Mid" << endl;
			cout << "Gate Type: " << uc->SubEug0->poleList[i]->gateType << endl;
			cout << "Output: " << uc->SubEug0->poleList[i]->controlledGate->output0 << endl;
			break;
		case DAG_NODE_TAIL:
			cout << "Output" << endl;
			cout << "Gate Type: " << uc->SubEug0->poleList[i]->gateType << endl;
			cout << "Output: " << uc->SubEug0->poleList[i]->controlledGate->output0 << endl;
			break;
		default:
			;
		}
	}
	*/

	/**
	cout << "==========================================" << endl;
	cout << "Output: " << output[0]  << endl;
	
	cout << "inputReady0 of outputGates[0]: " << uc->outputGates[0]->inputReady0 << endl;
	cout << "inputReady1 of outputGates[0]: " << uc->outputGates[0]->inputReady1 << endl;

	cout << "input0 of outputGates[0]: " << *uc->outputGates[0]->input0 << endl;
	cout << "input1 of outputGates[0]: " << *uc->outputGates[0]->input1 << endl;
	*/
	
	/*
	cout << endl;
	cout << "===========TEST subEug0===================" << endl;
	cout << "+++++++++++sn2List[0]+++++++++++++++++++++" << endl;
	cout << "inStyle: " << uc->SubEug0->sn2List[0]->inStyle << endl;
	cout << "poleStyle: " << uc->SubEug0->sn2List[0]->poleStyle << endl;
	cout << "outStyle: " << uc->SubEug0->sn2List[0]->outStyle << endl;
	cout << "+++++++++++sn2List[1]+++++++++++++++++++++" << endl;
	cout << "inStyle: " << uc->SubEug0->sn2List[1]->inStyle << endl;
	cout << "poleStyle: " << uc->SubEug0->sn2List[1]->poleStyle << endl;
	cout << "outStyle: " << uc->SubEug0->sn2List[1]->outStyle << endl;
	cout << "+++++++++++Left subComp+++++++++++++++++++" << endl;
	cout << "inStyle: " << uc->SubEug0->subCompList[0]->inStyle << endl;
	cout << "poleStyle: " << uc->SubEug0->subCompList[0]->poleStyle << endl;
	cout << "outStyle: " << uc->SubEug0->subCompList[0]->outStyle << endl;
	cout << "+++++++++++Right subComp++++++++++++++++++" << endl;
	cout << "inStyle: " << uc->SubEug0->subCompList[1]->inStyle << endl;
	cout << "poleStyle: " << uc->SubEug0->subCompList[1]->poleStyle << endl;
	cout << "outStyle: " << uc->SubEug0->subCompList[1]->outStyle << endl;
	*/

	/*
	cout << endl;
	cout << "===========TEST CONTROL===================" << endl;
	cout << "+++++++++++sn2List[0]+++++++++++++++++++++" << endl;
	cout << "control of normal0: " << uc->SubEug0->sn2List[0]->normalList[0]->control << endl;
	cout << "control of normal0: " << uc->SubEug0->sn2List[0]->normalList[1]->control << endl;
	cout << "control of normal0: " << uc->SubEug0->sn2List[0]->normalList[2]->control << endl;
	cout << "+++++++++++sn2List[1]+++++++++++++++++++++" << endl;
	cout << "control of normal0: " << uc->SubEug0->sn2List[1]->normalList[0]->control << endl;
	cout << "control of normal0: " << uc->SubEug0->sn2List[1]->normalList[1]->control << endl;
	cout << "control of normal0: " << uc->SubEug0->sn2List[1]->normalList[2]->control << endl;
	cout << "+++++++++++Left subComp+++++++++++++++++++" << endl;
	cout << "control of normal0: " << uc->SubEug0->subCompList[0]->normalList[0]->control << endl;
	cout << "control of normal0: " << uc->SubEug0->subCompList[0]->normalList[1]->control << endl;
	cout << "control of normal0: " << uc->SubEug0->subCompList[0]->normalList[2]->control << endl;
	cout << "+++++++++++Right subComp++++++++++++++++++" << endl;
	cout << "control of normal0: " << uc->SubEug0->subCompList[1]->normalList[0]->control << endl;
	cout << "control of normal0: " << uc->SubEug0->subCompList[1]->normalList[1]->control << endl;
	cout << "control of normal0: " << uc->SubEug0->subCompList[1]->normalList[2]->control << endl;
	*/

	for (uint32_t i = 0; i < 7; i++)
	{
		cout << "output " <<i <<"  =  "<< output[i] << endl;
	}
	cout << "UC Test Passed." << endl;


}


void testSampleSimpleCircuit()
{
	cout << "in testSampleSimpleCircuit() " << endl;
	SimpleCircuit::testCircuit();
}


void circuitTest()
{
	Circuit* xswitch = Circuit::generateToyXSwitch();

	xswitch->inFixList[0] = true;
	xswitch->inFixList[1] = false;
	xswitch->inFixList[2] = false;

	xswitch->setReady();
	xswitch->calculate();

	cout << *xswitch->outFixList[0] << " " << *xswitch->outFixList[1] << endl;
}







uint32_t* getPerm(int m, int seed)
{
	bool b[257];           //a check array
	int a[256];            //the arrangement (output)
	uint32_t* perm = new uint32_t[256];
	int i, j, k;
	for (i = 1; i <= m; i++) b[i] = false;
	srand(seed);
	for (i = m - 1; i >= 0; i--) 
	{
		j = rand() % (i + 1);
		for (k = 1; k <= m; k++)
		{
			if (!b[k]) j--;
			if (j<0) break;
		}//for k
		a[i] = k;
		b[k] = true;
	}//for i

	for (int t = 0; t < 256; t++)
		perm[t] = uint32_t(a[t] - 1);

	return perm;
} // arrange 


#ifdef DEBUG

void Eug1Test()
{
	//uint32_t n = uint32_t(atoi(argv[1]));
	//uint32_t n = 2;
	//uint32_t m = uint32_t(pow(2, n));



	uint32_t m = 256; //173
	uint32_t n = uint32_t(log(m - 1) / log(2)) + 1;

	EugComponent::usedMatrix = new uint32_t*[EugComponent::allNodes.size()];
	for (size_t i = 0; i < EugComponent::allNodes.size(); i++)
	{
		EugComponent::usedMatrix[i] = new uint32_t[EugComponent::allNodes.size()];
		for (size_t j = 0; j < EugComponent::allNodes.size(); j++)
			EugComponent::usedMatrix[i][j] = 0u;
	}

	uint32_t seed1 = uint32_t(2342);
	uint32_t seed2 = uint32_t(3422);
	//uint32_t seed1 = 100u;
	//uint32_t seed2 = 200u;

	uint32_t* perm1 = getPerm(m, seed1);
	uint32_t* perm2 = getPerm(m, seed2);
/**
	cout << "Permutation1 : " << endl;
	for (size_t i = 0; i < m; i++)
	{
		cout << perm1[i] << " ";
	}
	cout << endl << "Permutation2 :" << endl;
	for (size_t i = 0; i < m; i++)
	{
		cout << perm2[i] << " ";
	}
	cout << endl;
*/
	Dag* testDag = new Dag(1, 1, m, perm1, perm2);
	testDag->print();


	EugComponent* Eug = EugComponent::getEug_m(m);
	cout << "Nodes: " << EugNode::idCounter - 1 << endl;
	cout << "SN2 Parts: " << EugComponent::compIdCounter - 1 << endl;
	
	EugComponent::usedMatrix = new uint32_t*[EugComponent::allNodes.size()];
	for (size_t i = 0; i < EugComponent::allNodes.size(); i++)
	{
		EugComponent::usedMatrix[i] = new uint32_t[EugComponent::allNodes.size()];
		for (size_t j = 0; j < EugComponent::allNodes.size(); j++)
		{
			EugComponent::usedMatrix[i][j] = 0u;
		}
	}
	cout << "printIns()" << endl;
	Eug->printIns();
	cout << "printPoles()" << endl;
	Eug->printPoles();
	cout << "printEdges()" << endl;
	Eug->printEdges();
	cout << "setDag()" << endl;
	Eug->setDag(testDag);
	cout << "finished setDag()" << endl;
	delete testDag;

	uint32_t** adjMatrix = Eug->getAdjointMatrix();
	Eug->updateUsedAdjointMatrix();
	/**
	cout << "Adjoin Matrix:" << endl;

	for (size_t i = 0; i < EugComponent::allNodes.size(); i++)
	{
		for (size_t j = 0; j < EugComponent::allNodes.size(); j++)
		{
			if (adjMatrix[i][j] != 0)
				cout << i << " -> " << j << endl;
		}
	}

	cout << endl << endl << "Used Matrix: " << endl;

	for (size_t i = 0; i < EugComponent::allNodes.size(); i++)
	{
		for (size_t j = 0; j < EugComponent::allNodes.size(); j++)
		{
			switch (EugComponent::usedMatrix[i][j])
			{
			case 1:
				cout << i << " -> " << j << endl;
				break;
			case 2:
				cout << i << " -> " << j << endl;
				cout << "#########FOUND A REUSED EDGE###########" << endl;
				cout << "WRONG!!" << endl;
				delete Eug;
				return;
				break;
			default:
				break;
			}
		}
	}

	*/
	
	cout << "Adjoin Matrix:" << endl;

	for (size_t i = 0; i < EugComponent::allNodes.size(); i++)
	{
		uint32_t numi = 0;
		uint32_t numj = 0;
		for (size_t j = 0; j < EugComponent::allNodes.size(); j++)
		{
			if (adjMatrix[i][j] > 0) {
				numi++;
				if (numi > 2) {
					cout << i << " double used" << endl;
				}
			}
			if (adjMatrix[j][i] > 0) {
				numj++;
				if (numj > 2) {
					cout << i << " double used" << endl;
				}
			}
		}
	}


	

	cout << endl << endl << "Used Matrix: " << endl;

	for (size_t i = 0; i < EugComponent::allNodes.size(); i++)
	{
		for (size_t j = 0; j < EugComponent::allNodes.size(); j++)
		{
			if (EugComponent::usedMatrix[i][j] == 1) 
			{
				if (adjMatrix[i][j] == 0) {
					cout << i << " without used " << endl;
				}
			}
			if (EugComponent::usedMatrix[i][j] > 1) {
				cout << i << " double linking " << j << endl;
			}
		}
	}


	//delete Eug;

	cout << "Eug Test Passed." << endl;
}

#endif // DEBUG

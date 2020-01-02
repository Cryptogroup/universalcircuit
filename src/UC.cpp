//#include "stdafx.h"
#include "UC.h"

Dag ** UC::GetCircuitFromFile(string fileName, FileType fileType)
{
	switch (fileType)
	{
	case FILE_TYPE_CIRCUIT:
		return CircuitFileParser(fileName);
		break;
	default:
		return nullptr;
	}
}

void UC::GetDags(size_t Dagsize, uint32_t location, uint32_t* from, uint32_t* to, uint32_t* gate, Dag** dags)
{
	uint32_t IndexFrom = from[location];
	uint32_t IndexTo = to[location];

	//cout << from[location] << " GetDag   " << to[location] << " " << gate[location] << endl;

	from[location] = UINT32_MAX;


	if (dags[0]->nodeList[IndexFrom]->fanOut < 1 && dags[0]->nodeList[IndexTo]->fanIn < 1)
	{
		dags[0]->addEdge(IndexFrom, IndexTo);
		if (location != 0) 
		{
			if (to[location - 1] == to[location]) 
			{
				uint32_t a = (gate[location] >> 1) % 2;
				uint32_t b = (gate[location] >> 2) % 2;
				gate[location] += a * 2;
				gate[location] -= b * 2;
			}
		}
	}
	else if(dags[1]->nodeList[IndexFrom]->fanOut < 1 && dags[1]->nodeList[IndexTo]->fanIn < 1)
	{
		dags[1]->addEdge(IndexFrom, IndexTo);
		
if (location != 0)
		{
			if (to[location + 1] != to[location] && to[location - 1] != to[location])
			{
				uint32_t a = (gate[location] >> 1) % 2;
				uint32_t b = (gate[location] >> 2) % 2;
				gate[location] += a * 2;
				gate[location] -= b * 2;
			}

			if (to[location + 1] == to[location])
			{
				uint32_t a = (gate[location] >> 1) % 2;
				uint32_t b = (gate[location] >> 2) % 2;
				gate[location] += a * 2;
				gate[location] -= b * 2;
			}
		}
	}

	else 
	{
		cout << "error" << endl;
	}
	for (int i = 0; i < Dagsize; i++) 
	{
		if (from[i] == IndexFrom) 
		{
			GetDags(Dagsize, i, from, to, gate, dags);
		}
		if (to[i] == IndexTo && from[i]!= UINT32_MAX)
		{
			GetDags(Dagsize, i, from, to, gate, dags);
		}
	}
}


Dag ** UC::CircuitFileParser(string fileName)
{
	ifstream fileStream;
	fileStream.open(fileName);

	size_t inputCount = 0;
	string lineReader;

	uint32_t Dagsize = 0;


	vector<uint32_t> inList; // The list of input gates
	vector<uint32_t> outList; // The list of output gates
	vector<uint32_t> midList; // The list of inner gates
	vector<uint32_t> nonInputList; // The list of all non-input gates
	vector<uint32_t> gateList; // The list of all gates
	vector<uint32_t> gateTypeList; // The gate type of all gates, the type of input gates are always and(1u).
	vector<uint32_t> inputList0;
	vector<uint32_t> inputList1;

	while (getline(fileStream, lineReader))
	{
		istringstream parser(lineReader);
		uint32_t nodeNo;
		string nodeType;
		try {
			parser >> nodeNo >> nodeType;
		}
		catch (...) { break; }
		if (nodeType == "input")
		{
			inList.push_back(nodeNo);
			gateList.push_back(nodeNo);
			gateTypeList.push_back(7u);
			//cout << nodeNo << " " << nodeType << endl;
		}
		else if (nodeType == "gate")
		{
			gateList.push_back(nodeNo);
			midList.push_back(nodeNo);
			nonInputList.push_back(nodeNo);

			// Throw "arity 2 table ["
			parser >> nodeType;
			uint32_t typeParser;


			parser >> typeParser;

			parser >> nodeType;
			parser >> nodeType;

			if (typeParser == 1)
			{
				Dagsize++;
				uint32_t gateType = 0;
				parser >> typeParser;
				gateType += typeParser;
				gateType *= 2;
				parser >> typeParser;
				gateType += typeParser;
				gateType = -6 + 9* gateType;
				

				gateTypeList.push_back(gateType);

				// Throw "] inputs ["
				parser >> nodeType;
				parser >> nodeType;
				parser >> nodeType;

				// Parse pre-gate
				uint32_t input0;
				parser >> input0;
				inputList0.push_back(input0);
				inputList1.push_back(input0);

				//cout << nodeNo << " " << gateType << " " << input0 << endl;
			}
			else
			{
				// Parse gateType
				Dagsize += 2;
				uint32_t gateType = 0;
				parser >> typeParser;
				gateType += typeParser;
				gateType *= 2;
				parser >> typeParser;
				gateType += typeParser;
				gateType *= 2;
				parser >> typeParser;
				gateType += typeParser;
				gateType *= 2;
				parser >> typeParser;
				gateType += typeParser;
				gateTypeList.push_back(gateType);

				// Throw "] inputs ["
				parser >> nodeType;
				parser >> nodeType;
				parser >> nodeType;

				// Parse pre-gate
				uint32_t input0, input1;
				parser >> input0 >> input1;
				inputList0.push_back(input0);
				inputList1.push_back(input1);

				//cout << nodeNo << " " << gateType << " " << input0 << " " << input1 << endl;
			}
		}
		else if (nodeType == "output")
		{
			gateList.push_back(nodeNo);
			outList.push_back(nodeNo);
			nonInputList.push_back(nodeNo);

			// Throw "gate arity 2 table ["
			parser >> nodeType;
			parser >> nodeType;
			uint32_t typeParser;
			parser >> typeParser;
			parser >> nodeType;
			parser >> nodeType;
			if (typeParser == 1)
			{
				Dagsize++;
				// Parse gateType
				uint32_t gateType = 0;
				uint32_t typeParser;
				parser >> typeParser;
				gateType += typeParser;
				gateType *= 2;
				parser >> typeParser;
				gateType += typeParser;
				gateType = -6 + 9* gateType;
				gateTypeList.push_back(gateType);

				// Throw "] inputs ["
				parser >> nodeType;
				parser >> nodeType;
				parser >> nodeType;

				// Parse pre-gate
				uint32_t input0;
				parser >> input0;
				inputList0.push_back(input0);
				inputList1.push_back(input0);
				//cout << nodeNo << " " << gateType << " " << input0 << endl;
			}
			else {
				// Parse gateType
				Dagsize += 2;
				uint32_t gateType = 0;
				uint32_t typeParser;
				parser >> typeParser;
				gateType += typeParser;
				gateType *= 2;
				parser >> typeParser;
				gateType += typeParser;
				gateType *= 2;
				parser >> typeParser;
				gateType += typeParser;
				gateType *= 2;
				parser >> typeParser;
				gateType += typeParser;
				gateTypeList.push_back(gateType);

				// Throw "] inputs ["
				parser >> nodeType;
				parser >> nodeType;
				parser >> nodeType;

				// Parse pre-gate
				uint32_t input0, input1;
				parser >> input0 >> input1;
				inputList0.push_back(input0);
				inputList1.push_back(input1);

				//cout << nodeNo << " " << gateType << " " << input0 << " " << input1 << endl;
			}
		}
	}
	//cout << "         gate type list                               " << endl;
	for (size_t i = 0; i < gateTypeList.size(); i++) {
		//cout << gateTypeList[i] << endl;
	}



	fileStream.close();
	Dag** dags = new Dag*[2];
	dags[0] = new Dag(1, 1, gateList.size());
	dags[1] = new Dag(1, 1, gateList.size());

	// Set head node of DAG(dag)
	for (vector<uint32_t>::iterator it = inList.begin(); it != inList.end(); it++)
	{
		dags[0]->getNode(*it)->type = DAG_NODE_HEAD;
		//cout << "get node: " << dags[0]->getNode(*it)->id << endl;
		dags[1]->getNode(*it)->type = DAG_NODE_HEAD;

		dags[0]->headList.push_back(dags[0]->getNode(*it));
		dags[1]->headList.push_back(dags[1]->getNode(*it));
	}

	// Set mid node of DAG(dag)
	for (vector<uint32_t>::iterator it = midList.begin(); it != midList.end(); it++)
	{
		dags[0]->getNode(*it)->type = DAG_NODE_NORMAL;
		dags[1]->getNode(*it)->type = DAG_NODE_NORMAL;
	}

	// Set tail node of DAG(dag)
	for (vector<uint32_t>::iterator it = outList.begin(); it != outList.end(); it++)
	{
		dags[0]->getNode(*it)->type = DAG_NODE_TAIL;
		dags[1]->getNode(*it)->type = DAG_NODE_TAIL;
	}

	// Link nodes
	vector<uint32_t>::iterator itIn0 = inputList0.begin();
	vector<uint32_t>::iterator itIn1 = inputList1.begin();
	vector<uint32_t>::iterator itNonIn = nonInputList.begin();
	vector<uint32_t>::iterator itGateType = gateTypeList.begin();
	
	
	for (uint32_t i = 0; i < inList.size(); i++) 
	{
		itGateType++;
	}
	
	
	from = new uint32_t[Dagsize];
	to = new uint32_t[Dagsize];
	gate = new uint32_t[Dagsize];
	
	uint32_t index = 0;

	while (itIn0 != inputList0.end()) 
	{
		if (*itIn1 == *itIn0) 
		{
			from[index] = *itIn0;
			to[index] = *itNonIn;
			gate[index] = (*itGateType);
			index++;
		}
		else 
		{
			from[index] = *itIn0;
			to[index] = *itNonIn;
			gate[index] = (*itGateType);
			index++;
			from[index] = *itIn1;
			to[index] = *itNonIn;
			gate[index] = (*itGateType);
			index++;
		}
		itIn0++;
		itIn1++;
		itNonIn++;
		itGateType++;
	}


	for (uint32_t i = 0; i < Dagsize; i++) 
	{
		cout << from[i] << "-->" << to[i] << endl;
	}

	for (uint32_t i = 0; i < Dagsize; i++)
	{
		if (from[i] != UINT32_MAX) 
		{
			GetDags(Dagsize,i,from, to, gate, dags);

		}
	}


    itIn0 = inputList0.begin();
    itIn1 = inputList1.begin();
	itNonIn = nonInputList.begin();
	itGateType = gateTypeList.begin();

	index = 0;
	for (uint32_t i = 0; i < inList.size(); i++)
	{
		itGateType++;
	}
	while (itIn0 != inputList0.end())
	{
		if (*itIn1 == *itIn0)
		{
			*itGateType = gate[index];
			index++;
		}
		else
		{
			*itGateType = gate[index];
			index += 2;
		}
		itIn0++;
		itIn1++;
		itNonIn++;
		itGateType++;
	}



	/**
	while (itIn0 != inputList0.end())
	{
		cout << (*itIn0) << "->" << (*itNonIn) <<"   " << (*itIn1) << "->" << (*itNonIn) << endl;

		if (*itIn1 == *itIn0)
		{
			if (dags[0]->nodeList[*itIn0]->fanOut < 1)
			{
				dags[0]->addEdge((*itIn0), (*itNonIn));
			}
			else
			{
				dags[1]->addEdge((*itIn0), (*itNonIn));
				uint32_t a = ((*itGateType) >> 1) % 2;
				uint32_t b = ((*itGateType) >> 2) % 2;
				*itGateType += a * 2;
				*itGateType -= b * 2;
			}
		}
		else
		{
			if (dags[0]->nodeList[*itIn0]->fanOut < 1 && dags[1]->nodeList[*itIn1]->fanOut < 1)
			{
				dags[0]->addEdge((*itIn0), (*itNonIn));
				dags[1]->addEdge((*itIn1), (*itNonIn));
			}
			else if (dags[0]->nodeList[*itIn1]->fanOut < 1 && dags[1]->nodeList[*itIn0]->fanOut < 1)
			{
				dags[0]->addEdge((*itIn1), (*itNonIn));
				dags[1]->addEdge((*itIn0), (*itNonIn));

				uint32_t a = ((*itGateType) >> 1) % 2;
				uint32_t b = ((*itGateType) >> 2) % 2;
				*itGateType += a * 2;
				*itGateType -= b * 2;
				
			}
			else 
			{
				cout << "error" << endl;
			}
		}
		itIn0++;
		itIn1++;
		itNonIn++;
		itGateType++;
	}
	*/



	// Set type of Gates
	vector<uint32_t>::iterator itGate = gateList.begin();
	itGateType = gateTypeList.begin();
	while (itGate != gateList.end())
	{
		dags[0]->getNode(*itGate)->gateType = (*itGateType);
		dags[1]->getNode(*itGate)->gateType = (*itGateType);

		itGate++;
		itGateType++;
	};

#ifdef DEBUG
	//cout << "Pirnt Dags: " << endl;
	//cout << "================DAGS 0===================" << endl;
	dags[0]->print();
	//cout << "================DAGS 1===================" << endl;
	dags[1]->print();
#endif // DEBUG

	return dags;
}

UC::UC(uint32_t N)
{
	layerCount = N;
	nodeNum = uint32_t(pow(2, N));

	SubEug0 = EugComponent::getEug1(N);
	SubEug1 = EugComponent::getEug1(N);

	universalCircuit = nullptr;
	simpleUniversalCircuit = nullptr;

	hasSetCircuit = false;
	hasGeneratedOutput = false;

	switches = nullptr;
	supSwiches = nullptr;
}


UC::UC(uint32_t t, uint32_t m) // t is distingush from UC(uint32_t N), m = 2^N
{
	nodeNum = m;

	layerCount = uint32_t(log(m - 1) / log(2)) + 1;

	SubEug0 = EugComponent::getEug_m(m);
	SubEug1 = EugComponent::getEug_m(m);

	/**
	cout << " ---------------------SubEug0-----------------------  " << endl;
	SubEug0->printIns();
	SubEug0->printPoles();
	SubEug0->printEdges();


	cout << " -------------------SubEug1------------------------  " << endl;
	SubEug1->printIns();
	SubEug1->printPoles();
	SubEug1->printEdges();
	*/

	universalCircuit = nullptr;
	simpleUniversalCircuit = nullptr;

	hasSetCircuit = false;
	hasGeneratedOutput = false;

	switches = nullptr;
	supSwiches = nullptr;
}



UC::~UC()
{
}


void UC::setCircuit(uint32_t m, uint32_t* from1, uint32_t* from2)
{
	vector<uint32_t> inList; // The list of input gates
	vector<uint32_t> outList; // The list of output gates
	vector<uint32_t> midList; // The list of inner gates
	vector<uint32_t> nonInputList; // The list of all non-input gates
	vector<uint32_t> gateList; // The list of all gates
	vector<uint32_t> gateTypeList; // The gate type of all gates, the type of input gates are always and(1u).
	vector<uint32_t> inputList0;
	vector<uint32_t> inputList1;

	for (uint32_t i = 0; i < m; i++) {
		if (from1[i] < i && from2[i] < i) //gate i is gate
		{
			gateList.push_back(i);
			midList.push_back(i);
			nonInputList.push_back(i);
			gateTypeList.push_back(i % 16);
			inputList0.push_back(from1[i]);
			inputList1.push_back(from2[i]);
		}
		else //gate i is input
		{
			inList.push_back(i);
			gateList.push_back(i);
			gateTypeList.push_back(7u);
		}
	}

	/*
	cout << "         gate type list                               " << endl;
	for (size_t i = 0; i < gateTypeList.size(); i++) {
		cout << gateTypeList[i] << endl;
	}
	*/

	Dag** dags = new Dag*[2];
	dags[0] = new Dag(1, 1, gateList.size());
	dags[1] = new Dag(1, 1, gateList.size());

	// Set head node of DAG(dag)
	for (vector<uint32_t>::iterator it = inList.begin(); it != inList.end(); it++)
	{
		dags[0]->getNode(*it)->type = DAG_NODE_HEAD;
		//cout << "get node: " << dags[0]->getNode(*it)->id << endl;
		dags[1]->getNode(*it)->type = DAG_NODE_HEAD;

		dags[0]->headList.push_back(dags[0]->getNode(*it));
		dags[1]->headList.push_back(dags[1]->getNode(*it));
	}

	// Set mid node of DAG(dag)
	for (vector<uint32_t>::iterator it = midList.begin(); it != midList.end(); it++)
	{
		dags[0]->getNode(*it)->type = DAG_NODE_NORMAL;
		dags[1]->getNode(*it)->type = DAG_NODE_NORMAL;
	}

	// Set tail node of DAG(dag)
	for (vector<uint32_t>::iterator it = outList.begin(); it != outList.end(); it++)
	{
		dags[0]->getNode(*it)->type = DAG_NODE_TAIL;
		dags[1]->getNode(*it)->type = DAG_NODE_TAIL;
	}

	// Link nodes
	vector<uint32_t>::iterator itIn0 = inputList0.begin();
	vector<uint32_t>::iterator itIn1 = inputList1.begin();
	vector<uint32_t>::iterator itNonIn = nonInputList.begin();
	vector<uint32_t>::iterator itGateType = gateTypeList.begin();
	while (itIn0 != inputList0.end())
	{
		// 
		if (dags[0]->nodeList[*itIn0]->fanOut < 1 && dags[1]->nodeList[*itIn1]->fanOut < 1)
		{
			dags[0]->addEdge((*itIn0), (*itNonIn));
			dags[1]->addEdge((*itIn1), (*itNonIn));
		}
		else
		{
			dags[0]->addEdge((*itIn1), (*itNonIn));
			dags[1]->addEdge((*itIn0), (*itNonIn));

			uint32_t a = ((*itGateType) >> 1) % 2;
			uint32_t b = ((*itGateType) >> 2) % 2;
			*itGateType += a * 2;
			*itGateType -= b * 2;
		}

		itIn0++;
		itIn1++;
		itNonIn++;
		itGateType++;
	}

	// Set type of Gates
	vector<uint32_t>::iterator itGate = gateList.begin();
	itGateType = gateTypeList.begin();
	while (itGate != gateList.end())
	{
		dags[0]->getNode(*itGate)->gateType = (*itGateType);
		dags[1]->getNode(*itGate)->gateType = (*itGateType);

		itGate++;
		itGateType++;
	};

#ifdef DEBUG
	//cout << "Pirnt Dags: " << endl;
	//cout << "================DAGS 0===================" << endl;
	//dags[0]->print();
	//cout << "================DAGS 1===================" << endl;
	//dags[1]->print();
#endif // DEBUG

	SubEug0->setDag(dags[0]);
	SubEug1->setDag(dags[1]);

	hasSetCircuit = true;

	delete dags[0];
	delete dags[1];
	delete[] dags;
}



void UC::setCircuit(string fileName, FileType fileType)
{
	Dag** dags = GetCircuitFromFile(fileName, fileType);
	cout << "++++++++++++Dags[0]+++++++++++++" << endl;
	dags[0]->print();
	cout << "++++++++++++Dags[1]+++++++++++++" << endl;
	dags[1]->print();
	SubEug0->setDag(dags[0]);
	SubEug1->setDag(dags[1]);

	hasSetCircuit = true;

	delete dags[0];
	delete dags[1];
	delete[] dags;
}

void UC::generateSimpleCircuit()
{
	if (hasSetCircuit == false)
	{
		cout << "Haven't set circuit for UC!" << endl;

		return;
	}

	simpleUniversalCircuit = SimpleCircuit::getSimpleUnivseralCircuit(1, this);

	switches = &simpleUniversalCircuit->switchList;



	Gates.clear();
	inputGates.clear();
	outputGates.clear();
#ifdef DEBUG
	midGates.clear();
#endif // DEBUG

	for (size_t i = 0; i < SubEug0->poleList.size(); i++)
	{
		switch (SubEug0->poleList[i]->dagNodeType)
		{
		case DagNodeType::DAG_NODE_HEAD:
			Gates.push_back(SubEug0->poleList[i]->controlledGate);
			inputGates.push_back(SubEug0->poleList[i]->controlledGate);
			break;
		case DagNodeType::DAG_NODE_TAIL:
			Gates.push_back(SubEug0->poleList[i]->controlledGate);
			outputGates.push_back(SubEug0->poleList[i]->controlledGate);
			break;
#ifdef DEBUG
		case DagNodeType::DAG_NODE_NORMAL:
			Gates.push_back(SubEug0->poleList[i]->controlledGate);
			midGates.push_back(SubEug0->poleList[i]->controlledGate);
			break;
#endif // DEBUG
		default:
			;
		}
	}
}

void UC::setInput(uint32_t m, uint32_t* from1, uint32_t* from2)
{
    bool* input = new bool[m];
    for (uint32_t i = 0; i < m; i++)
    {
        input[i] = (bool)(i % 2);
    }
    
    for (uint32_t i = 0; i < m; i++)
    {
        if (!(from1[i] < i && from2[i] < i))
        {
            if (input[i] == true)
            {
                Gates[i]->input0 = &SimpleGate::TRUE;
                Gates[i]->input1 = &SimpleGate::TRUE;
            }
            else
            {
                Gates[i]->input0 = &SimpleGate::FALSE;
                Gates[i]->input1 = &SimpleGate::FALSE;
            }
        }
    }
    
    string filename = "Random_input";
    int count;
    string Outfilename = "Output/"+filename+"_prog.txt";
    ofstream outputprog;
    outputprog.open(Outfilename);
    vector<Node4Struct> nodelist;
    Outfilename = "Output/"+filename+"_circ.txt";
    ofstream outputcirc;
    outputcirc.open(Outfilename);
    
    
    for (size_t i = 0; i < m; i++)
    {
        if (!(from1[i] < i && from2[i] < i))
        {
            Gates[i]->generateOutputChain(nodelist);
        }
    }
    
    for (size_t i = 0; i < supSwiches->size(); i++)
    {
        //cout << "           size of supSwiches             " <<supSwiches->size() << endl;
        (*supSwiches)[i]->generateOutputChain(nodelist);
    }
}
//find and return the node's num whose id is num in nodelist 
long long findnumid(uint32_t num,vector< Node4Struct> nodelist)
{
	long long result = -1;
	for(uint32_t i=0;i<nodelist.size();i++)
	{
		if(nodelist[i].nodeid==num)
			break;
	}
	return result;
}

void UC::setInput(bool* input, size_t inputSize,string filename,uint32_t gatenum)
{
    if (inputSize != inputGates.size())
    {
        cout << "Wrong input size!" << endl;
        return;
    }
    
    for (size_t i = 0; i < inputGates.size(); i++)
    {
        if (input[i] == true)
        {
            inputGates[i]->input0 = &SimpleGate::TRUE;
            inputGates[i]->input1 = &SimpleGate::TRUE;
        }
        else
        {
            inputGates[i]->input0 = &SimpleGate::FALSE;
            inputGates[i]->input1 = &SimpleGate::FALSE;
        }
    }

    
	/* 
	outputcirc<<"C";
	
	for (size_t i = 0;i<SubEug0->poleList.size(); i++)
	{
		switch (this->SubEug0->poleList[i]->dagNodeType)
		{	
			case DAG_NODE_TAIL:
				//cout << "Output" << endl;
				//cout << "Gate Type: " << uc->SubEug0->poleList[i]->gateType << endl;
				//cout << "Output: " << uc->SubEug0->poleList[i]->controlledGate->output0 << endl;
				outputcirc << " " << this->SubEug0->poleList[i]->id;
				break;
			default:
			;
		}
	}
	cout<<endl;
	*/
	int count=0;
    string Outfilename = "Output/"+filename+"_prog.txt";
    ofstream outputprog;
    outputprog.open(Outfilename);
    vector<string> addrlist;
    vector<int> numlist;
    Outfilename = "Output/"+filename+"_circ.txt";
    ofstream outputcirc;
    outputcirc.open(Outfilename);
	vector <Node4Struct>nodelist;
    for (size_t i = 0; i < inputGates.size(); i++)
    {
    	inputGates[i]->generateOutputChain(nodelist);
        
    }
	
    for (size_t i = 0; i < supSwiches->size(); i++)
    {
        cout << "           size of supSwiches             " << supSwiches->size() << endl;
        cout << "           (*supSwiches)[i]            " << (*supSwiches)[i]->controlNode->id << endl;
        (*supSwiches)[i]->generateOutputChain(nodelist);
    }
	// Number each side
	for(int i=0;i<nodelist.size();i++)
	{
		//cout<<"Node sideA:"<<nodelist[i].nextnodeA<<endl;
		//number nextsideA
		if((nodelist[i].nextnodeA!=-1)&&(nodelist[i].nextSideA==-1))
		{
			nodelist[i].nextSideA=count;
			count++;
			int nextid=findnumid(nodelist[i].nextnodeA,nodelist);
			if(nextid!=-1)
			{
			if(nodelist[nextid].prevnodeA==nodelist[i].nodeid&&nodelist[nextid].prevSideA==-1)
			{
				nodelist[nextid].prevSideA=count;
			}else if(nodelist[nextid].prevnodeB==nodelist[i].nodeid&&nodelist[nextid].prevSideB==-1){
				nodelist[nextid].prevSideB=count;
			}
			}

		}
		//number nextsideB
		if((nodelist[i].nextnodeB!=-1)&&(nodelist[i].nextSideB==-1))
		{
			nodelist[i].nextSideB=count;
			count++;
			int nextid=findnumid(nodelist[i].nextnodeB,nodelist);
			if(nextid!=-1)
			{
				if(nodelist[nextid].prevnodeA==nodelist[i].nodeid&&nodelist[nextid].prevSideA==-1)
			{
				nodelist[nextid].prevSideA=count;
			}else if(nodelist[nextid].prevnodeB==nodelist[i].nodeid&&nodelist[nextid].prevSideB==-1){
				nodelist[nextid].prevSideB=count;
			}
			}

		}
		//number prevSideA
		if((nodelist[i].prevnodeA!=-1)&&(nodelist[i].prevSideA==-1))
		{
			nodelist[i].prevSideA=count;
			count++;
			int nextid=findnumid(nodelist[i].prevnodeA,nodelist);
			if(nextid!=-1)
			{
			if(nodelist[nextid].nextnodeA==nodelist[i].nodeid&&nodelist[nextid].nextSideA==-1)
			{
				nodelist[nextid].nextSideA=count;
			}else if(nodelist[nextid].nextnodeB==nodelist[i].nodeid&&nodelist[nextid].nextSideB==-1){
				nodelist[nextid].nextSideB=count;
			}
			}
		}
		//number prevSideB
		if((nodelist[i].prevnodeB!=-1)&&(nodelist[i].prevSideB==-1))
		{
			nodelist[i].prevSideB=count;
			count++;
			int nextid=findnumid(nodelist[i].prevnodeB,nodelist);
			if(nextid!=-1)
			{
			if(nodelist[nextid].nextnodeA==nodelist[i].nodeid&&nodelist[nextid].nextSideA==-1)
			{
				nodelist[nextid].nextSideA=count;
			}else if(nodelist[nextid].nextnodeB==nodelist[i].nodeid&&nodelist[nextid].nextSideB==-1){
				nodelist[nextid].nextSideB=count;
			}
			}
		}
		if((nodelist[i].prevSideA!=-1||nodelist[i].prevSideB!=-1)&&(nodelist[i].nextSideA!=-1||nodelist[i].nextSideB!=-1))
		{
			outputcirc<<nodelist[i].nodeid<<" X "<<nodelist[i].prevSideA<<" "<<nodelist[i].prevSideB<<" "<<nodelist[i].nextSideA<<" "<<nodelist[i].nextSideB<<endl;
			outputprog<<nodelist[i].controlflag<<endl;
		}
		//outputcirc<<nodelist[i].nodeid<<endl;
	}
	outputcirc.close();
	outputprog.close();
	 
    
}

bool* UC::getOutput()
{
    bool* result = new bool[outputGates.size()];
    for (size_t i = 0; i < outputGates.size(); i++)
    {
        result[i] = outputGates[i]->output0;
    }
    
    return result;
}

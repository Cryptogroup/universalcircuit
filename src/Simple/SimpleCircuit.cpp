//#include "stdafx.h"
#include "SimpleCircuit.h"


SimpleCircuit::SimpleCircuit(SimpleCircuitType type)
{
	simpleCircuitType = type;
}

SimpleCircuit::SimpleCircuit(uint32_t m, uint32_t* from1, uint32_t* from2, bool* output) 
{
	EugNode** controlNodes = new EugNode*[m];
	for (uint32_t i = 0; i < m; i++)
	{
		controlNodes[i] = new EugNode(EUG_NODE_DEFAULT, nullptr, 0u);
	}

	for (uint32_t i = 0; i < m; i++)
	{
		if ((from1[i] < i) && (from2[i] < i)) 
		{
			controlNodes[i]->dagNodeType = DAG_NODE_NORMAL;
			controlNodes[i]->gateType = i%16;
		}
		else 
		{
			controlNodes[i]->dagNodeType = DAG_NODE_HEAD;
			controlNodes[i]->gateType = 7u;
		}
	}

	SimpleGate** simpleGates = new SimpleGate*[m];
	for (uint32_t i = 0; i < m; i++)
	{
		simpleGates[i] = new SimpleUniversalGate(controlNodes[i]);
	}

	bool* input = new bool[m];

	for (uint32_t i = 0; i < m; i++)
	{
		input[i] = (bool)(i % 2);
		//cout << " input[i] =   " << input[i] << endl;
		if (from1[i] < i && from2[i] < i)
		{
			simpleGates[from1[i]]->postGateList0.push_back(simpleGates[i]);
			simpleGates[from2[i]]->postGateList1.push_back(simpleGates[i]);
			simpleGates[i]->input0 = &simpleGates[from1[i]]->output0;
			simpleGates[i]->input1 = &simpleGates[from2[i]]->output0;
		}
		else 
		{
			simpleGates[i]->input0 = &input[i];
			simpleGates[i]->input1 = &input[i];
			simpleGates[i]->inputReady0 = true;
			simpleGates[i]->inputReady1 = true;
		}
	}


	for (uint32_t i = 0; i < m; i++)
	{
		input[i] = (bool)(i % 2);
		//cout << " input[i] =   " << input[i] << endl;
		if (!(from1[i] < i && from2[i] < i))
		{
			//simpleGates[i]->generateOutputChain();
		}
	}

	for (uint32_t i = 0; i < m; i++) 
	{
		output[i] = simpleGates[i]->output0;
	}
}


SimpleCircuit::~SimpleCircuit()
{
	switch (simpleCircuitType)
	{
	case SIMPLE_CIRCUIT_UC:
		break;
	default:
		for (size_t i = 0; i < simpleGateList.size(); i++)
		{
			delete simpleGateList[i];
		}
	}
}

SimpleCircuit * SimpleCircuit::getSimpleSuperNode2(EugNode* pole0, EugNode* pole1, EugNode* normalLeft0,
	EugNode* normalLeft1, EugNode* normalLeft2, EugNode* normalRight0, EugNode* normalRight1, EugNode* normalRight2)
{
	// Get new universal gate and x-switches for the new SN2
	SimpleUniversalGate* simpleUniversalGate0 = new SimpleUniversalGate(pole0);
	SimpleUniversalGate* simpleUniversalGate1 = new SimpleUniversalGate(pole1);
	SimpleXSwitch* simpleXSwitchLeft0 = new SimpleXSwitch(normalLeft0);
	SimpleXSwitch* simpleXSwitchLeft1 = new SimpleXSwitch(normalLeft1);
	SimpleXSwitch* simpleXSwitchLeft2 = new SimpleXSwitch(normalLeft2);
	SimpleXSwitch* simpleXSwitchRight0 = new SimpleXSwitch(normalRight0);
	SimpleXSwitch* simpleXSwitchRight1 = new SimpleXSwitch(normalRight1);
	SimpleXSwitch* simpleXSwitchRight2 = new SimpleXSwitch(normalRight2);

	// Set controlledGate for poles
	pole0->controlledGate = simpleUniversalGate0;
	pole1->controlledGate = simpleUniversalGate1;
	//cout << "                real super node                     " << endl;
	// Link the newly gotten simple gates
	simpleUniversalGate0->input0 = &simpleXSwitchLeft0->output0;
	simpleUniversalGate0->input1 = &simpleXSwitchRight0->output0;
	//cout << simpleUniversalGate0->controlNode->id << "    ->   " << simpleXSwitchLeft0->controlNode->id << endl;
	//cout << simpleUniversalGate0->controlNode->id << "    ->   " << simpleXSwitchRight0->controlNode->id << endl;
	
	simpleXSwitchLeft1->input1 = &simpleXSwitchLeft0->output1;
	simpleXSwitchRight1->input1 = &simpleXSwitchRight0->output1;
	//cout << simpleXSwitchLeft1->controlNode->id << "    ->   " << simpleXSwitchLeft0->controlNode->id << endl;
	//cout << simpleXSwitchRight1->controlNode->id << "    ->   " << simpleXSwitchRight0->controlNode->id << endl;
	
	simpleXSwitchLeft1->input0 = &simpleUniversalGate0->output0;
	simpleXSwitchRight1->input0 = &simpleUniversalGate0->output1;
	//cout << simpleXSwitchLeft1->controlNode->id << "    ->   " << simpleUniversalGate0->controlNode->id << endl;
	//cout << simpleXSwitchRight1->controlNode->id << "    ->   " << simpleUniversalGate0->controlNode->id << endl;
	
	simpleUniversalGate1->input0 = &simpleXSwitchLeft1->output0;
	simpleUniversalGate1->input1 = &simpleXSwitchRight1->output0;
	//cout << simpleUniversalGate1->controlNode->id << "    ->   " << simpleXSwitchLeft1->controlNode->id << endl;
	//cout << simpleUniversalGate1->controlNode->id << "    ->   " << simpleXSwitchRight1->controlNode->id << endl;
	
	simpleXSwitchLeft2->input1 = &simpleXSwitchLeft1->output1;
	simpleXSwitchRight2->input1 = &simpleXSwitchRight1->output1;
	//cout << simpleXSwitchLeft2 ->controlNode->id << "    ->   " << simpleXSwitchLeft1->controlNode->id << endl;
	//cout << simpleXSwitchRight2->controlNode->id << "    ->   " << simpleXSwitchRight1->controlNode->id << endl;
	
	simpleXSwitchLeft2->input0 = &simpleUniversalGate1->output0;
	simpleXSwitchRight2->input0 = &simpleUniversalGate1->output1;
	//cout << simpleXSwitchLeft2->controlNode->id << "    ->   " << simpleUniversalGate1->controlNode->id << endl;
	//cout << simpleXSwitchRight2->controlNode->id << "    ->   " << simpleUniversalGate1->controlNode->id << endl;

	// Set the postGateList(0&1) of the simple gates, in order to use generateOutputChain()	
	simpleXSwitchLeft0->postGateList0.push_back(simpleUniversalGate0);
	simpleXSwitchRight0->postGateList1.push_back(simpleUniversalGate0);
	simpleUniversalGate0->hasinput0++;
	simpleUniversalGate0->hasinput1++;

	simpleXSwitchLeft0->postGateList1.push_back(simpleXSwitchLeft1);
	simpleXSwitchLeft1->hasinput1++;
	simpleXSwitchRight0->postGateList1.push_back(simpleXSwitchRight1);
	simpleXSwitchRight1->hasinput1++;

	simpleUniversalGate0->postGateList0.push_back(simpleXSwitchLeft1);
	simpleXSwitchLeft1->hasinput0++;
	simpleUniversalGate0->postGateList0.push_back(simpleXSwitchRight1);
	simpleXSwitchRight1->hasinput0++;

	simpleXSwitchLeft1->postGateList0.push_back(simpleUniversalGate1);
	simpleUniversalGate1->hasinput0++;
	simpleXSwitchRight1->postGateList1.push_back(simpleUniversalGate1);
	simpleUniversalGate1->hasinput1++;

	simpleXSwitchLeft1->postGateList1.push_back(simpleXSwitchLeft2);
	simpleXSwitchLeft2->hasinput1++;
	simpleXSwitchRight1->postGateList1.push_back(simpleXSwitchRight2);
	simpleXSwitchRight2->hasinput1++;
	simpleUniversalGate1->postGateList0.push_back(simpleXSwitchLeft2);
	simpleXSwitchLeft2->hasinput0++;
	simpleUniversalGate1->postGateList0.push_back(simpleXSwitchRight2);
	simpleXSwitchRight2->hasinput0++;

	SimpleCircuit* simpleSuperNode2 = new SimpleCircuit(SIMPLE_CIRCUIT_SN2);

	// Store 2 universal gates in universalGateList
	simpleSuperNode2->universalGateList.push_back(simpleUniversalGate0);
	simpleSuperNode2->universalGateList.push_back(simpleUniversalGate1);


	// Store pointers of switches in vector switchList
	simpleSuperNode2->switchList.push_back(simpleXSwitchLeft0);
	simpleSuperNode2->switchList.push_back(simpleXSwitchLeft1);
	simpleSuperNode2->switchList.push_back(simpleXSwitchLeft2);
	simpleSuperNode2->switchList.push_back(simpleXSwitchRight0);
	simpleSuperNode2->switchList.push_back(simpleXSwitchRight1);
	simpleSuperNode2->switchList.push_back(simpleXSwitchRight2);


	// push the gates in the new SN2's simpleGateList
	simpleSuperNode2->simpleGateList.push_back(simpleXSwitchLeft0);
	simpleSuperNode2->simpleGateList.push_back(simpleXSwitchLeft1);
	simpleSuperNode2->simpleGateList.push_back(simpleXSwitchLeft2);
	simpleSuperNode2->simpleGateList.push_back(simpleXSwitchRight0);
	simpleSuperNode2->simpleGateList.push_back(simpleXSwitchRight1);
	simpleSuperNode2->simpleGateList.push_back(simpleXSwitchRight2);
	simpleSuperNode2->simpleGateList.push_back(simpleUniversalGate0);
	simpleSuperNode2->simpleGateList.push_back(simpleUniversalGate1);

	return 	simpleSuperNode2;
}


SimpleCircuit * SimpleCircuit::getSimpleSuperNode1(EugNode* pole0)
{
	// Get new universal gate and x-switches for the new SN2
	SimpleUniversalGate* simpleUniversalGate0 = new SimpleUniversalGate(pole0);

	// Set controlledGate for poles
	pole0->controlledGate = simpleUniversalGate0;

	//cout << "                real super node                     " << endl;
	// Link the newly gotten simple gates

	SimpleCircuit* simpleSuperNode1 = new SimpleCircuit(SIMPLE_CIRCUIT_SN1);

	// Store 2 universal gates in universalGateList
	simpleSuperNode1->universalGateList.push_back(simpleUniversalGate0);

	// Store pointers of switches in vector switchList
	simpleSuperNode1->switchList.push_back(simpleUniversalGate0);

	// push the gates in the new SN2's simpleGateList
	simpleSuperNode1->simpleGateList.push_back(simpleUniversalGate0);

	return 	simpleSuperNode1;
}



SimpleCircuit * SimpleCircuit::getSimplePseudoSuperNode2(EugNode* normal0, EugNode* normal1, EugNode* normal2)
{
	SimpleXSwitch* simpleXSwitch0 = new SimpleXSwitch(normal0);
	SimpleXSwitch* simpleXSwitch1 = new SimpleXSwitch(normal1);
	SimpleXSwitch* simpleXSwitch2 = new SimpleXSwitch(normal2);

	simpleXSwitch1->input1 = &simpleXSwitch0->output1;
	simpleXSwitch2->input1 = &simpleXSwitch1->output1;

	simpleXSwitch0->postGateList1.push_back(simpleXSwitch1);
	simpleXSwitch1->hasinput1++;
	simpleXSwitch1->postGateList1.push_back(simpleXSwitch2);
	simpleXSwitch2->hasinput1++;

	SimpleCircuit* simplePseudoSuperNode2 = new SimpleCircuit(SIMPLE_CIRCUIT_PSEUDO_SN2);

	// Store pointers of switches in vector switchList
	simplePseudoSuperNode2->switchList.push_back(simpleXSwitch0);
	simplePseudoSuperNode2->switchList.push_back(simpleXSwitch1);
	simplePseudoSuperNode2->switchList.push_back(simpleXSwitch2);

	simplePseudoSuperNode2->simpleGateList.push_back(simpleXSwitch0);
	simplePseudoSuperNode2->simpleGateList.push_back(simpleXSwitch1);
	simplePseudoSuperNode2->simpleGateList.push_back(simpleXSwitch2);

	return simplePseudoSuperNode2;
}

SimpleCircuit * SimpleCircuit::getSimplePseudoSuperNode1(EugNode* pole0)
{
	SimpleXSwitch* simpleXSwitch0 = new SimpleXSwitch(pole0);

	SimpleCircuit* simplePseudoSuperNode1 = new SimpleCircuit(SIMPLE_CIRCUIT_PSEUDO_SN1);

	// Store pointers of switches in vector switchList
	simplePseudoSuperNode1->switchList.push_back(simpleXSwitch0);

	simplePseudoSuperNode1->simpleGateList.push_back(simpleXSwitch0);

	return simplePseudoSuperNode1;
}

SimpleCircuit * SimpleCircuit::getSimpleUnivseralCircuit(uint32_t t, UC* uc)
{
	uint32_t N = uc->layerCount;
	uint32_t m = uc->nodeNum; //total pole number
	uint32_t temp = m % 2; //whether have SimpleSuperNode1
	
	uint32_t M1 = (m + 1) / 2;
	uint32_t M2 = m / 2; // The number of SimpleSuperNode2
	uint32_t M1temp = M1 % 2;
	uint32_t M2temp = M2 % 2;


	SimpleCircuit* simpleUniversalCircuit = new SimpleCircuit(SIMPLE_CIRCUIT_UC);

	vector<SimpleGate*> simpleXSwitches0;
	vector<SimpleGate*> simpleXSwitches1;

	SimpleCircuit** SimpleSuperNode2s = new SimpleCircuit*[M1];
	for (uint32_t i = 0; i < M2; i++)
	{
		// Generate part of UC: SimpleSuperNodes2s
		SimpleSuperNode2s[i] = getSimpleSuperNode2(
			uc->SubEug0->sn2List[i]->poleList[0],
			uc->SubEug0->sn2List[i]->poleList[1],
			uc->SubEug0->sn2List[i]->normalList[0],
			uc->SubEug0->sn2List[i]->normalList[1],
			uc->SubEug0->sn2List[i]->normalList[2],
			uc->SubEug1->sn2List[i]->normalList[0],
			uc->SubEug1->sn2List[i]->normalList[1],
			uc->SubEug1->sn2List[i]->normalList[2]
		);

		simpleUniversalCircuit->universalGateList.push_back(SimpleSuperNode2s[i]->universalGateList[0]);
		simpleUniversalCircuit->universalGateList.push_back(SimpleSuperNode2s[i]->universalGateList[1]);

		simpleXSwitches0.push_back(SimpleSuperNode2s[i]->simpleGateList[0]);
		simpleXSwitches0.push_back(SimpleSuperNode2s[i]->simpleGateList[1]);
		simpleXSwitches0.push_back(SimpleSuperNode2s[i]->simpleGateList[2]);
		simpleXSwitches1.push_back(SimpleSuperNode2s[i]->simpleGateList[3]);
		simpleXSwitches1.push_back(SimpleSuperNode2s[i]->simpleGateList[4]);
		simpleXSwitches1.push_back(SimpleSuperNode2s[i]->simpleGateList[5]);

		// store 2 universal gate in gateList;


		// store switches of Sn2 to the switchList of uc
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[0]);
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[1]);
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[2]);
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[3]);
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[4]);
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[5]);
	}
	
	if (temp == 1) 
	{
		SimpleSuperNode2s[M2] = getSimpleSuperNode1(uc->SubEug0->sn2List[M2]->poleList[0]);
		SimpleSuperNode2s[M2]->simpleGateList[0]->isSN1 = true;
		simpleUniversalCircuit->universalGateList.push_back(SimpleSuperNode2s[M2]->universalGateList[0]);
		
		simpleXSwitches0.push_back(SimpleSuperNode2s[M2]->universalGateList[0]);
		simpleXSwitches1.push_back(SimpleSuperNode2s[M2]->universalGateList[0]);
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[M2]->universalGateList[0]);
		
	}


	uc->universalGates = &simpleUniversalCircuit->universalGateList;


	if (uc->SubEug0->poleList.size() > 3)
	{
		SimpleCircuit* simpleController0 = getSimpleController(uc->SubEug0->subCompList[0]);
		SimpleCircuit* simpleController1 = getSimpleController(uc->SubEug0->subCompList[1]);
		SimpleCircuit* simpleController2 = getSimpleController(uc->SubEug1->subCompList[0]);
		SimpleCircuit* simpleController3 = getSimpleController(uc->SubEug1->subCompList[1]);

		// Store the switches of 4 controllers to the switchList of uc 

		for (size_t i = 0; i < simpleController0->switchList.size(); i++)
			simpleUniversalCircuit->switchList.push_back(simpleController0->switchList[i]);

		for (size_t i = 0; i < simpleController1->switchList.size(); i++)
			simpleUniversalCircuit->switchList.push_back(simpleController1->switchList[i]);

		for (size_t i = 0; i < simpleController2->switchList.size(); i++)
			simpleUniversalCircuit->switchList.push_back(simpleController2->switchList[i]);

		for (size_t i = 0; i < simpleController3->switchList.size(); i++)
			simpleUniversalCircuit->switchList.push_back(simpleController3->switchList[i]);


		for (size_t i = 0; i < simpleController0->supList.size(); i++)
			simpleUniversalCircuit->supList.push_back(simpleController0->supList[i]);

		for (size_t i = 0; i < simpleController1->supList.size(); i++)
			simpleUniversalCircuit->supList.push_back(simpleController1->supList[i]);

		for (size_t i = 0; i < simpleController2->supList.size(); i++)
			simpleUniversalCircuit->supList.push_back(simpleController2->supList[i]);

		for (size_t i = 0; i < simpleController3->supList.size(); i++)
			simpleUniversalCircuit->supList.push_back(simpleController3->supList[i]);

		//cout << "           linked the circuit      getSimpleUnivseralCircuit          " << endl;
		
		if (M2temp == 0) {
			for (uint32_t i = 0; i < M2; i++)
			{
				simpleXSwitches0[i * 3]->input0 = &simpleController0->simpleGateList[i / 2 * 3 + i % 2]->output0;
				simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches0[i * 3 + 2]->output0;

				//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController0->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
				//cout << simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simpleXSwitches0[i * 3 + 2]->controlNode->id << endl;

				simpleXSwitches0[i * 3]->input1 = &simpleController1->simpleGateList[i / 2 * 3 + i % 2]->output0;
				simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches0[i * 3 + 2]->output1;

				//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
				//cout << simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simpleXSwitches0[i * 3 + 2]->controlNode->id << endl;

				simpleXSwitches1[i * 3]->input0 = &simpleController2->simpleGateList[i / 2 * 3 + i % 2]->output0;
				simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches1[i * 3 + 2]->output0;
				//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController2->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
				//cout << simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simpleXSwitches1[i * 3 + 2]->controlNode->id << endl;

				simpleXSwitches1[i * 3]->input1 = &simpleController3->simpleGateList[i / 2 * 3 + i % 2]->output0;
				simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches1[i * 3 + 2]->output1;
				//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController3->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
				//cout << simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simpleXSwitches1[i * 3 + 2]->controlNode->id << endl;

				simpleController0->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simpleXSwitches0[i * 3]);
				simpleXSwitches0[i * 3]->hasinput0++;
				simpleXSwitches0[i * 3 + 2]->postGateList0.push_back(simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;

				simpleController1->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simpleXSwitches0[i * 3]);
				simpleXSwitches0[i * 3]->hasinput1++;
				simpleXSwitches0[i * 3 + 2]->postGateList0.push_back(simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;


				simpleController2->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simpleXSwitches1[i * 3]);
				simpleXSwitches1[i * 3]->hasinput0++;
				simpleXSwitches1[i * 3 + 2]->postGateList0.push_back(simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;

				simpleController3->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simpleXSwitches1[i * 3]);
				simpleXSwitches1[i * 3]->hasinput1++;
				simpleXSwitches1[i * 3 + 2]->postGateList0.push_back(simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
			}
			//for SN(1) in left.
			if (M1temp == 1) {
				simpleXSwitches0[M2 * 3]->input0 = &simpleController0->simpleGateList[M2 / 2 * 3]->output0; //3.13 output1

				//cout << simpleXSwitches0[M * 3]->controlNode->id << "    ->   " 
				//	<< simpleController0->simpleGateList[M / 2 * 3]->controlNode->id << endl;

				simpleXSwitches1[M2 * 3]->input1 = &simpleController2->simpleGateList[M2 / 2 * 3]->output0; //3.13 output1
				
				//cout << simpleXSwitches1[M * 3]->controlNode->id << "    ->   " 
				//	<< simpleController2->simpleGateList[M / 2 * 3]->controlNode->id << endl;

				simpleController0->simpleGateList[M2 / 2 * 3]->postGateList0.push_back(simpleXSwitches0[M2 * 3]);
				simpleXSwitches0[M2 * 3]->hasinput0++;


				simpleController2->simpleGateList[M2 / 2 * 3]->postGateList1.push_back(simpleXSwitches1[M2 * 3]);
				simpleXSwitches1[M2 * 3]->hasinput1++;
			}
		}

		if (M2temp == 1) {
			for (uint32_t i = 0; i < M2 - 1; i++)
			{
				simpleXSwitches0[i * 3]->input0 = &simpleController0->simpleGateList[i / 2 * 3 + i % 2]->output0;
				simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches0[i * 3 + 2]->output0;

				//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController0->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
				//cout << simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simpleXSwitches0[i * 3 + 2]->controlNode->id << endl;

				simpleXSwitches0[i * 3]->input1 = &simpleController1->simpleGateList[i / 2 * 3 + i % 2]->output0;
				simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches0[i * 3 + 2]->output1;

				//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
				//cout << simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simpleXSwitches0[i * 3 + 2]->controlNode->id << endl;

				simpleXSwitches1[i * 3]->input0 = &simpleController2->simpleGateList[i / 2 * 3 + i % 2]->output0;
				simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches1[i * 3 + 2]->output0;
				//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController2->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
				//cout << simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simpleXSwitches1[i * 3 + 2]->controlNode->id << endl;

				simpleXSwitches1[i * 3]->input1 = &simpleController3->simpleGateList[i / 2 * 3 + i % 2]->output0;
				simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches1[i * 3 + 2]->output1;
				//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController3->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
				//cout << simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simpleXSwitches1[i * 3 + 2]->controlNode->id << endl;

				simpleController0->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simpleXSwitches0[i * 3]);
				simpleXSwitches0[i * 3]->hasinput0++;
				simpleXSwitches0[i * 3 + 2]->postGateList0.push_back(simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;

				simpleController1->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simpleXSwitches0[i * 3]);
				simpleXSwitches0[i * 3]->hasinput1++;
				simpleXSwitches0[i * 3 + 2]->postGateList0.push_back(simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;


				simpleController2->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simpleXSwitches1[i * 3]);
				simpleXSwitches1[i * 3]->hasinput0++;
				simpleXSwitches1[i * 3 + 2]->postGateList0.push_back(simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;

				simpleController3->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simpleXSwitches1[i * 3]);
				simpleXSwitches1[i * 3]->hasinput1++;
				simpleXSwitches1[i * 3 + 2]->postGateList0.push_back(simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
			}
			
			//for sn(1) in left and right side
			if (M1temp == 1) 
			{
				
				simpleXSwitches0[(M2 - 1) * 3]->input0 = &simpleController0->simpleGateList[(M2 - 1) / 2 * 3]->output0; //3.13 output1
				//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController0->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				simpleXSwitches0[(M2 - 1) * 3]->input1 = &simpleController1->simpleGateList[(M2 - 1) / 2 * 3]->output0; //3.13 output1
				//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				simpleXSwitches1[(M2 - 1) * 3]->input0 = &simpleController2->simpleGateList[(M2 - 1) / 2 * 3]->output0; //3.13 output1
				//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController2->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				simpleXSwitches1[(M2 - 1) * 3]->input1 = &simpleController3->simpleGateList[(M2 - 1) / 2 * 3]->output0; //3.13 output1
				//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController3->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				simpleController0->simpleGateList[(M2 - 1) / 2 * 3]->postGateList0.push_back(simpleXSwitches0[(M2 - 1) * 3]);
				simpleXSwitches0[(M2 - 1) * 3]->hasinput0++;

				simpleController1->simpleGateList[(M2 - 1) / 2 * 3]->postGateList1.push_back(simpleXSwitches0[(M2 - 1) * 3]);
				simpleXSwitches0[(M2 - 1) * 3]->hasinput1++;

				simpleController2->simpleGateList[(M2 - 1) / 2 * 3]->postGateList0.push_back(simpleXSwitches1[(M2 - 1) * 3]);
				simpleXSwitches1[(M2 - 1) * 3]->hasinput0++;

				simpleController3->simpleGateList[(M2 - 1) / 2 * 3]->postGateList1.push_back(simpleXSwitches1[(M2 - 1) * 3]);
				simpleXSwitches1[(M2 - 1) * 3]->hasinput1++;
			}
			if (M1temp == 0) 
			{
				//for sn(2) in left side 
				simpleXSwitches0[(M2 - 1) * 3]->input0 = &simpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2]->output0;
				simpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]->input0 = &simpleXSwitches0[(M2 - 1) * 3 + 2]->output0;

				//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController0->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
				//cout << simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simpleXSwitches0[i * 3 + 2]->controlNode->id << endl;

				simpleXSwitches0[(M2 - 1) * 3]->input1 = &simpleController1->simpleGateList[(M2 - 1) / 2 * 3]->output0;// 3.13 output1

				//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				simpleXSwitches1[(M2 - 1) * 3]->input0 = &simpleController2->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2]->output0;
				simpleController2->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]->input0 = &simpleXSwitches1[(M2 - 1) * 3 + 2]->output0;
				//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController2->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
				//cout << simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simpleXSwitches1[i * 3 + 2]->controlNode->id << endl;

				simpleXSwitches1[(M2 - 1) * 3]->input1 = &simpleController3->simpleGateList[(M2 - 1) / 2 * 3]->output0;//3.13 output1
				//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController3->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				simpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2]->postGateList0.push_back(simpleXSwitches0[(M2 - 1) * 3]);
				simpleXSwitches0[(M2 - 1) * 3]->hasinput0++;
				simpleXSwitches0[(M2 - 1) * 3 + 2]->postGateList0.push_back(simpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]);
				simpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]->hasinput0++;

				simpleController1->simpleGateList[(M2 - 1) / 2 * 3]->postGateList1.push_back(simpleXSwitches0[(M2 - 1) * 3]);
				simpleXSwitches0[(M2 - 1) * 3]->hasinput1++;


				simpleController2->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2]->postGateList0.push_back(simpleXSwitches1[(M2 - 1) * 3]);
				simpleXSwitches1[(M2 - 1) * 3]->hasinput0++;
				
				simpleXSwitches1[(M2 - 1) * 3 + 2]->postGateList0.push_back(simpleController2->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]);
				simpleController2->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]->hasinput0++;
				simpleController3->simpleGateList[(M2 - 1) / 2 * 3]->postGateList1.push_back(simpleXSwitches1[(M2 - 1) * 3]);
				simpleXSwitches1[(M2 - 1) * 3]->hasinput1++;

				
				//for sn(1) 
				simpleXSwitches0[M2 * 3]->input0 = &simpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2]->output0;

				//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController0->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
			
				simpleXSwitches1[M2 * 3]->input1 = &simpleController2->simpleGateList[M2 / 2 * 3 + M2 % 2]->output0;
				//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   "
				//	<< simpleController2->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				simpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2]->postGateList0.push_back(simpleXSwitches0[M2 * 3]);
				simpleXSwitches0[M2 * 3]->hasinput0++;

				simpleController2->simpleGateList[M2 / 2 * 3 + M2 % 2]->postGateList1.push_back(simpleXSwitches1[M2 * 3]);
				simpleXSwitches1[M2 * 3]->hasinput1++;
			}
		}

		/**
		if (Mtemp == 1)
		{
			if (temp == 1)
			{
				//SN(2)
				simpleXSwitches0[M * 3]->input0 = &simpleController0->simpleGateList[M / 2 * 3 + M % 2]->output0;
				simpleController0->simpleGateList[M / 2 * 3 + M % 2 + 1]->input0 = &simpleXSwitches0[M * 3 + 2]->output0;

				//	cout << simpleXSwitches0[M * 3]->controlNode->id << "    ->   " << simpleController0->simpleGateList[M / 2 * 3 + M % 2]->controlNode->id << endl;
				//	cout << simpleController0->simpleGateList[M / 2 * 3 + M % 2 + 1]->controlNode->id << "    ->   " << simpleXSwitches0[M * 3 + 2]->controlNode->id << endl;

				simpleXSwitches0[M * 3]->input1 = &simpleController1->simpleGateList[M / 2 * 3]->output1;
				simpleController1->simpleGateList[M / 2 * 3]->input1 = &simpleXSwitches0[M * 3 + 2]->output1;

				//	cout << simpleXSwitches0[M * 3]->controlNode->id << "    ->   " << simpleController1->simpleGateList[M / 2 * 3]->controlNode->id << endl;
				//	cout << simpleController1->simpleGateList[M / 2 * 3]->controlNode->id << "    ->   " << simpleXSwitches0[M * 3 + 2]->controlNode->id << endl;

				simpleXSwitches1[M * 3]->input0 = &simpleController2->simpleGateList[M / 2 * 3 + M % 2]->output0;
				simpleController2->simpleGateList[M / 2 * 3 + M % 2 + 1]->input0 = &simpleXSwitches1[M * 3 + 2]->output0;
				//	cout << simpleXSwitches1[M * 3]->controlNode->id << "    ->   " << simpleController2->simpleGateList[M / 2 * 3 + M % 2]->controlNode->id << endl;
				//	cout << simpleController2->simpleGateList[M / 2 * 3 + M % 2 + 1]->controlNode->id << "    ->   " << simpleXSwitches1[M * 3 + 2]->controlNode->id << endl;

				simpleXSwitches1[M * 3]->input1 = &simpleController3->simpleGateList[M / 2 * 3]->output1;
				simpleController3->simpleGateList[M / 2 * 3]->input1 = &simpleXSwitches1[M * 3 + 2]->output1;
				//	cout << simpleXSwitches1[M * 3]->controlNode->id << "    ->   " << simpleController3->simpleGateList[M / 2 * 3]->controlNode->id << endl;
				//	cout << simpleController3->simpleGateList[M / 2 * 3]->controlNode->id << "    ->   " << simpleXSwitches1[M * 3 + 2]->controlNode->id << endl;

				simpleController0->simpleGateList[M / 2 * 3 + M % 2]->postGateList0.push_back(simpleXSwitches0[M * 3]);
				simpleXSwitches0[M * 3]->hasinput0++;
				simpleXSwitches0[M * 3 + 2]->postGateList0.push_back(simpleController0->simpleGateList[M / 2 * 3 + M % 2 + 1]);
				simpleController0->simpleGateList[M / 2 * 3 + M % 2 + 1]->hasinput0++;

				simpleController1->simpleGateList[M / 2 * 3]->postGateList1.push_back(simpleXSwitches0[M * 3]);
				simpleXSwitches0[M * 3]->hasinput1++;
				simpleXSwitches0[M * 3 + 2]->postGateList0.push_back(simpleController1->simpleGateList[M / 2 * 3]);
				simpleController1->simpleGateList[M / 2 * 3]->hasinput0++;


				simpleController2->simpleGateList[M / 2 * 3 + M % 2]->postGateList0.push_back(simpleXSwitches1[M * 3]);
				simpleXSwitches1[M * 3]->hasinput0++;
				simpleXSwitches1[M * 3 + 2]->postGateList0.push_back(simpleController2->simpleGateList[M / 2 * 3 + M % 2 + 1]);
				simpleController2->simpleGateList[M / 2 * 3 + M % 2 + 1]->hasinput0++;

				simpleController3->simpleGateList[M / 2 * 3]->postGateList1.push_back(simpleXSwitches1[M * 3]);
				simpleXSwitches1[M * 3]->hasinput1++;
				simpleXSwitches1[M * 3 + 2]->postGateList0.push_back(simpleController3->simpleGateList[M / 2 * 3]);
				simpleController3->simpleGateList[M / 2 * 3]->hasinput0++;


				//SN(1)
				simpleXSwitches0[(M + Mtemp) * 3]->input0 = &simpleController0->simpleGateList[(M + Mtemp) / 2 * 3 + (M + Mtemp) % 2]->output0;

				//	cout << simpleXSwitches0[(M + Mtemp) * 3]->controlNode->id << "    ->   " 
				//		<< simpleController0->simpleGateList[(M + Mtemp) / 2 * 3 + (M + Mtemp) % 2]->controlNode->id << endl;

				simpleXSwitches1[(M + Mtemp) * 3]->input1 = &simpleController2->simpleGateList[(M + Mtemp) / 2 * 3 + (M + Mtemp) % 2]->output0;
				simpleController2->simpleGateList[(M + Mtemp) / 2 * 3 + (M + Mtemp) % 2 + 1]->input0 = &simpleXSwitches1[(M + Mtemp) * 3]->output0;
				//	cout << simpleXSwitches1[(M + Mtemp) * 3]->controlNode->id << "    ->   " 
				//		<< simpleController2->simpleGateList[(M + Mtemp) / 2 * 3 + (M + Mtemp) % 2]->controlNode->id << endl;
				//	cout << simpleController2->simpleGateList[(M + Mtemp) / 2 * 3 + (M + Mtemp) % 2 + 1]->controlNode->id << "    ->   " 
				//		<< simpleXSwitches1[(M + Mtemp) * 3]->controlNode->id << endl;

				simpleController0->simpleGateList[(M + Mtemp) / 2 * 3 + (M + Mtemp) % 2]->postGateList0.push_back(simpleXSwitches0[(M + Mtemp) * 3]);
				simpleXSwitches0[(M + Mtemp) * 3]->hasinput0++;




				simpleController2->simpleGateList[(M + Mtemp) / 2 * 3 + (M + Mtemp) % 2]->postGateList1.push_back(simpleXSwitches1[(M + Mtemp) * 3]);
				simpleXSwitches1[(M + Mtemp) * 3]->hasinput1++;
				simpleXSwitches1[(M + Mtemp) * 3]->postGateList0.push_back(simpleController2->simpleGateList[(M + Mtemp) / 2 * 3 + (M + Mtemp) % 2 + 1]);
				simpleController2->simpleGateList[(M + Mtemp) / 2 * 3 + (M + Mtemp) % 2 + 1]->hasinput0++;
				uint32_t a = 0;
			}
			if (temp == 0)
			{
				simpleXSwitches0[M * 3]->input0 = &simpleController0->simpleGateList[M / 2 * 3]->output1; //3.12
				simpleController0->simpleGateList[M / 2 * 3]->input1 = &simpleXSwitches0[M * 3 + 2]->output0; //3.12

			//	cout << simpleXSwitches0[M * 3]->controlNode->id << "    ->   " << simpleController0->simpleGateList[M / 2 * 3]->controlNode->id << endl;
			//	cout << simpleController0->simpleGateList[M / 2 * 3]->controlNode->id << "    ->   " << simpleXSwitches0[M * 3 + 2]->controlNode->id << endl;

				simpleXSwitches0[M * 3]->input1 = &simpleController1->simpleGateList[M / 2 * 3]->output1;
				simpleController1->simpleGateList[M / 2 * 3]->input1 = &simpleXSwitches0[M * 3 + 2]->output1;

				//	cout << simpleXSwitches0[M * 3]->controlNode->id << "    ->   " << simpleController1->simpleGateList[M / 2 * 3]->controlNode->id << endl;
				//	cout << simpleController1->simpleGateList[M / 2 * 3]->controlNode->id << "    ->   " << simpleXSwitches0[M * 3 + 2]->controlNode->id << endl;

				simpleXSwitches1[M * 3]->input0 = &simpleController2->simpleGateList[M / 2 * 3]->output1;//3.12
				simpleController2->simpleGateList[M / 2 * 3]->input1 = &simpleXSwitches1[M * 3 + 2]->output0; //3.12
			//	cout << simpleXSwitches1[M * 3]->controlNode->id << "    ->   " << simpleController2->simpleGateList[M / 2 * 3]->controlNode->id << endl;
			//	cout << simpleController2->simpleGateList[M / 2 * 3]->controlNode->id << "    ->   " << simpleXSwitches1[M * 3 + 2]->controlNode->id << endl;

				simpleXSwitches1[M * 3]->input1 = &simpleController3->simpleGateList[M / 2 * 3]->output1;
				simpleController3->simpleGateList[M / 2 * 3]->input1 = &simpleXSwitches1[M * 3 + 2]->output1;
				//	cout << simpleXSwitches1[M * 3]->controlNode->id << "    ->   " << simpleController3->simpleGateList[M / 2 * 3]->controlNode->id << endl;
				//	cout << simpleController3->simpleGateList[M / 2 * 3]->controlNode->id << "    ->   " << simpleXSwitches1[M * 3 + 2]->controlNode->id << endl;

				simpleController0->simpleGateList[M / 2 * 3]->postGateList0.push_back(simpleXSwitches0[M * 3]);
				simpleXSwitches0[M * 3]->hasinput0++;
				simpleXSwitches0[M * 3 + 2]->postGateList0.push_back(simpleController0->simpleGateList[M / 2 * 3]);
				simpleController0->simpleGateList[M / 2 * 3]->hasinput0++;

				simpleController1->simpleGateList[M / 2 * 3]->postGateList1.push_back(simpleXSwitches0[M * 3]);
				simpleXSwitches0[M * 3]->hasinput1++;
				simpleXSwitches0[M * 3 + 2]->postGateList0.push_back(simpleController1->simpleGateList[M / 2 * 3]);
				simpleController1->simpleGateList[M / 2 * 3]->hasinput0++;


				simpleController2->simpleGateList[M / 2 * 3]->postGateList0.push_back(simpleXSwitches1[M * 3]);
				simpleXSwitches1[M * 3]->hasinput0++;
				simpleXSwitches1[M * 3 + 2]->postGateList0.push_back(simpleController2->simpleGateList[M / 2 * 3]);
				simpleController2->simpleGateList[M / 2 * 3]->hasinput0++;

				simpleController3->simpleGateList[M / 2 * 3]->postGateList1.push_back(simpleXSwitches1[M * 3]);
				simpleXSwitches1[M * 3]->hasinput1++;
				simpleXSwitches1[M * 3 + 2]->postGateList0.push_back(simpleController3->simpleGateList[M / 2 * 3]);
				simpleController3->simpleGateList[M / 2 * 3]->hasinput0++;
			}
		}

		if (Mtemp == 0)
		{
			if (temp == 1)
			{
				simpleXSwitches0[M * 3]->input0 = &simpleController0->simpleGateList[M / 2 * 3]->output1;
				simpleController0->simpleGateList[M / 2 * 3]->input1 = &simpleXSwitches0[M * 3]->output0;
				//	cout << simpleXSwitches0[M * 3]->controlNode->id << "    ->   " << simpleController0->simpleGateList[M / 2 * 3]->controlNode->id << endl;
				//	cout << simpleController0->simpleGateList[M / 2 * 3]->controlNode->id << "    ->   " << simpleXSwitches0[M * 3]->controlNode->id << endl;


				simpleXSwitches1[M * 3]->input1 = &simpleController2->simpleGateList[M / 2 * 3]->output1;
				simpleController2->simpleGateList[M / 2 * 3]->input1 = &simpleXSwitches1[M * 3]->output0;
				//	cout << simpleXSwitches1[M * 3]->controlNode->id << "    ->   " << simpleController2->simpleGateList[M / 2 * 3]->controlNode->id << endl;
				//	cout << simpleController2->simpleGateList[M / 2 * 3]->controlNode->id << "    ->   " << simpleXSwitches1[M * 3]->controlNode->id << endl;

				simpleController0->simpleGateList[M / 2 * 3]->postGateList0.push_back(simpleXSwitches0[M * 3]);
				simpleXSwitches0[M * 3]->hasinput0++;
				simpleXSwitches0[M * 3]->postGateList0.push_back(simpleController0->simpleGateList[M / 2 * 3]);
				simpleController0->simpleGateList[M / 2 * 3]->hasinput0++;


				simpleController2->simpleGateList[M / 2 * 3]->postGateList1.push_back(simpleXSwitches1[M * 3]);
				simpleXSwitches1[M * 3]->hasinput1++;
				simpleXSwitches1[M * 3]->postGateList0.push_back(simpleController2->simpleGateList[M / 2 * 3]);
				simpleController2->simpleGateList[M / 2 * 3]->hasinput0++;
				uint32_t a = 0;
			}
		}
		*/
	}
	/**
	if (uc->SubEug0->poleList.size() == 3) //uc->SubEug0->poleList.size() == 3
	{
		SimpleCircuit* simpleController0 = getSimpleController(uc->SubEug0->subCompList[0]);
		SimpleCircuit* simpleController2 = getSimpleController(uc->SubEug1->subCompList[0]);

		// Store the switches of 4 controllers to the switchList of uc 
		for (size_t i = 0; i < simpleController0->switchList.size(); i++)
			simpleUniversalCircuit->switchList.push_back(simpleController0->switchList[i]);

		for (size_t i = 0; i < simpleController2->switchList.size(); i++)
			simpleUniversalCircuit->switchList.push_back(simpleController2->switchList[i]);

		for (size_t i = 0; i < simpleController0->supList.size(); i++)
			simpleUniversalCircuit->supList.push_back(simpleController0->supList[i]);

		for (size_t i = 0; i < simpleController2->supList.size(); i++)
			simpleUniversalCircuit->supList.push_back(simpleController2->supList[i]);

		//cout << "           linked the circuit      getSimpleUnivseralCircuit          " << endl;
		for (uint32_t i = 0; i < M + Mtemp; i++)
		{
			simpleXSwitches0[i * 3]->input0 = &simpleController0->simpleGateList[i / 2 * 3 + i % 2]->output0;
			simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches0[i * 3 + 2]->output0;

			//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   " 
			//	<< simpleController0->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
			//cout << simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   " 
			//	<< simpleXSwitches0[i * 3 + 2]->controlNode->id << endl;

			simpleXSwitches1[i * 3]->input0 = &simpleController2->simpleGateList[i / 2 * 3 + i % 2]->output0;
			simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches1[i * 3 + 2]->output0;
			//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   " 
			//	<< simpleController2->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
			//cout << simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   " 
			//	<< simpleXSwitches1[i * 3 + 2]->controlNode->id << endl;

			simpleController0->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simpleXSwitches0[i * 3]);
			simpleXSwitches0[i * 3]->hasinput0++;
			simpleXSwitches0[i * 3 + 2]->postGateList0.push_back(simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]);
			simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;

			simpleController2->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simpleXSwitches1[i * 3]);
			simpleXSwitches1[i * 3]->hasinput0++;
			simpleXSwitches1[i * 3 + 2]->postGateList0.push_back(simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]);
			simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
			uint32_t a = 0;
		}

		simpleXSwitches0[M * 3 + 3]->input0 = &simpleController0->simpleGateList[M / 2 * 3 + 1]->output0;
		simpleController0->simpleGateList[M / 2 * 3 + 2]->input0 = &simpleXSwitches0[M * 3 + 3]->output0;

		//cout << simpleXSwitches0[M * 3 + 3]->controlNode->id << "    ->   " << simpleController0->simpleGateList[M / 2 * 3 + 1]->controlNode->id << endl;
		//cout << simpleController0->simpleGateList[M / 2 * 3 + 2]->controlNode->id << "    ->   " << simpleXSwitches0[M * 3 + 3]->controlNode->id << endl;

		simpleXSwitches1[M * 3 + 3]->input0 = &simpleController2->simpleGateList[M / 2 * 3 + 1]->output0;
		simpleController2->simpleGateList[M / 2 * 3 + 2]->input0 = &simpleXSwitches1[M * 3 + 3]->output0;
		//cout << simpleXSwitches1[M * 3 + 3]->controlNode->id << "    ->   " << simpleController2->simpleGateList[M / 2 * 3 + 1]->controlNode->id << endl;
		//cout << simpleController2->simpleGateList[M / 2 * 3 + 2]->controlNode->id << "    ->   " << simpleXSwitches1[M * 3 + 3]->controlNode->id << endl;

		simpleController0->simpleGateList[M / 2 * 3 + 1]->postGateList0.push_back(simpleXSwitches0[M * 3 + 3]);
		simpleXSwitches0[M * 3 + 3]->hasinput0++;
		simpleXSwitches0[M * 3 + 3]->postGateList0.push_back(simpleController0->simpleGateList[M / 2 * 3 + 2]);
		simpleController0->simpleGateList[M / 2 * 3 + 2]->hasinput0++;

		simpleController2->simpleGateList[M / 2 * 3 + 1]->postGateList0.push_back(simpleXSwitches1[M * 3 + 3]);
		simpleXSwitches1[M * 3 + 3]->hasinput0++;
		simpleXSwitches1[M * 3 + 3]->postGateList0.push_back(simpleController2->simpleGateList[M / 2 * 3 + 2]);
		simpleController2->simpleGateList[M / 2 * 3 + 2]->hasinput0++;

	}
	*/

	uc->switches = &simpleUniversalCircuit->switchList;
	uc->supSwiches = &simpleUniversalCircuit->supList;

	//cout << "========         size of supSwiches  =======" << uc->supSwiches->size() << endl;



	return simpleUniversalCircuit;
}

SimpleCircuit * SimpleCircuit::getSimpleUnivseralCircuit(UC* uc)
{
	uint32_t N = uc->layerCount;

	uint32_t M = uint32_t(pow(2, (N - 1))); // The number of SimpleSuperNode2

	SimpleCircuit* simpleUniversalCircuit = new SimpleCircuit(SIMPLE_CIRCUIT_UC);
	vector<SimpleGate*> simpleXSwitches0;
	vector<SimpleGate*> simpleXSwitches1;

	SimpleCircuit** SimpleSuperNode2s = new SimpleCircuit*[M];
	for (uint32_t i = 0; i < M; i++)
	{
		// Generate part of UC: SimpleSuperNodes2s
		SimpleSuperNode2s[i] = getSimpleSuperNode2(
			uc->SubEug0->sn2List[i]->poleList[0],
			uc->SubEug0->sn2List[i]->poleList[1],
			uc->SubEug0->sn2List[i]->normalList[0],
			uc->SubEug0->sn2List[i]->normalList[1],
			uc->SubEug0->sn2List[i]->normalList[2],
			uc->SubEug1->sn2List[i]->normalList[0],
			uc->SubEug1->sn2List[i]->normalList[1],
			uc->SubEug1->sn2List[i]->normalList[2]
		);

		simpleUniversalCircuit->universalGateList.push_back(SimpleSuperNode2s[i]->universalGateList[0]);
		simpleUniversalCircuit->universalGateList.push_back(SimpleSuperNode2s[i]->universalGateList[1]);

		simpleXSwitches0.push_back(SimpleSuperNode2s[i]->simpleGateList[0]);
		simpleXSwitches0.push_back(SimpleSuperNode2s[i]->simpleGateList[1]);
		simpleXSwitches0.push_back(SimpleSuperNode2s[i]->simpleGateList[2]);
		simpleXSwitches1.push_back(SimpleSuperNode2s[i]->simpleGateList[3]);
		simpleXSwitches1.push_back(SimpleSuperNode2s[i]->simpleGateList[4]);
		simpleXSwitches1.push_back(SimpleSuperNode2s[i]->simpleGateList[5]);

		// store 2 universal gate in gateList;


		// store switches of Sn2 to the switchList of uc
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[0]);
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[1]);
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[2]);
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[3]);
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[4]);
		simpleUniversalCircuit->switchList.push_back(SimpleSuperNode2s[i]->switchList[5]);
	}

	uc->universalGates = &simpleUniversalCircuit->universalGateList;

	SimpleCircuit* simpleController0 = getSimpleController(uc->SubEug0->subCompList[0]);
	SimpleCircuit* simpleController1 = getSimpleController(uc->SubEug0->subCompList[1]);
	SimpleCircuit* simpleController2 = getSimpleController(uc->SubEug1->subCompList[0]);
	SimpleCircuit* simpleController3 = getSimpleController(uc->SubEug1->subCompList[1]);

	// Store the switches of 4 controllers to the switchList of uc 
	for (size_t i = 0; i < simpleController0->switchList.size(); i++)
	{
		simpleUniversalCircuit->switchList.push_back(simpleController0->switchList[i]);
	}

	for (size_t i = 0; i < simpleController1->switchList.size(); i++)
	{
		simpleUniversalCircuit->switchList.push_back(simpleController1->switchList[i]);
	}

	for (size_t i = 0; i < simpleController2->switchList.size(); i++)
	{
		simpleUniversalCircuit->switchList.push_back(simpleController2->switchList[i]);
	}

	for (size_t i = 0; i < simpleController3->switchList.size(); i++)
	{
		simpleUniversalCircuit->switchList.push_back(simpleController3->switchList[i]);
	}



	for (size_t i = 0; i < simpleController0->supList.size(); i++)
	{
		simpleUniversalCircuit->supList.push_back(simpleController0->supList[i]);
	}

	for (size_t i = 0; i < simpleController1->supList.size(); i++)
	{
		simpleUniversalCircuit->supList.push_back(simpleController1->supList[i]);
	}

	for (size_t i = 0; i < simpleController2->supList.size(); i++)
	{
		simpleUniversalCircuit->supList.push_back(simpleController2->supList[i]);
	}

	for (size_t i = 0; i < simpleController3->supList.size(); i++)
	{
		simpleUniversalCircuit->supList.push_back(simpleController3->supList[i]);
	}


	//cout << "           linked the circuit      getSimpleUnivseralCircuit          " << endl;
	for (uint32_t i = 0; i < M; i++)
	{
		

		simpleXSwitches0[i * 3]->input0 = &simpleController0->simpleGateList[i / 2 * 3 + i % 2]->output0;
		simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches0[i * 3 + 2]->output0;
		
		//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   " << simpleController0->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
		//cout << simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   " << simpleXSwitches0[i * 3 + 2]->controlNode->id << endl;

		simpleXSwitches0[i * 3]->input1 = &simpleController1->simpleGateList[i / 2 * 3 + i % 2]->output0;
		simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches0[i * 3 + 2]->output1;

		//cout << simpleXSwitches0[i * 3]->controlNode->id << "    ->   " << simpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
		//cout << simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   " << simpleXSwitches0[i * 3 + 2]->controlNode->id << endl;

		simpleXSwitches1[i * 3]->input0 = &simpleController2->simpleGateList[i / 2 * 3 + i % 2]->output0;
		simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches1[i * 3 + 2]->output0;
		//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   " << simpleController2->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
		//cout << simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   " << simpleXSwitches1[i * 3 + 2]->controlNode->id << endl;

		simpleXSwitches1[i * 3]->input1 = &simpleController3->simpleGateList[i / 2 * 3 + i % 2]->output0;
		simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simpleXSwitches1[i * 3 + 2]->output1;
		//cout << simpleXSwitches1[i * 3]->controlNode->id << "    ->   " << simpleController3->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;
		//cout << simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   " << simpleXSwitches1[i * 3 + 2]->controlNode->id << endl;

		simpleController0->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simpleXSwitches0[i * 3]);
		simpleXSwitches0[i * 3]->hasinput0++;
		simpleXSwitches0[i * 3 + 2]->postGateList0.push_back(simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]);
	    simpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;

		simpleController1->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simpleXSwitches0[i * 3]);
		simpleXSwitches0[i * 3]->hasinput1++;
		simpleXSwitches0[i * 3 + 2]->postGateList0.push_back(simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]);
		simpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;


		simpleController2->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simpleXSwitches1[i * 3]);
		simpleXSwitches1[i * 3]->hasinput0++;
		simpleXSwitches1[i * 3 + 2]->postGateList0.push_back(simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]);
		simpleController2->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;

		simpleController3->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simpleXSwitches1[i * 3]);
		simpleXSwitches1[i * 3]->hasinput1++;
		simpleXSwitches1[i * 3 + 2]->postGateList0.push_back(simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]);
		simpleController3->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
		uint32_t a = 0;
	}

	uc->switches = &simpleUniversalCircuit->switchList;
	uc->supSwiches = &simpleUniversalCircuit->supList;
	
	//cout << "========         size of supSwiches  =======" << uc->supSwiches->size() << endl;
	
	

	return simpleUniversalCircuit;
}

SimpleCircuit * SimpleCircuit::getSimpleController(EugComponent* eug1)
{
	if (eug1->isSn2 == true)
	{
		SimpleCircuit* simplePseudoSuperNode2 = getSimplePseudoSuperNode2(eug1->normalList[0], eug1->normalList[1], eug1->normalList[2]);
		simplePseudoSuperNode2->switchList[0]->input0 = &SimpleGate::FALSE;
		simplePseudoSuperNode2->switchList[0]->input1 = &SimpleGate::FALSE;
		simplePseudoSuperNode2->switchList[0]->inputReady0 = true;
		simplePseudoSuperNode2->switchList[0]->inputReady1 = true;
		simplePseudoSuperNode2->supList.push_back(simplePseudoSuperNode2->switchList[0]);
		//cout << " ============ suplist =====    " << simplePseudoSuperNode2->switchList[0]->controlNode->id << endl;
		return simplePseudoSuperNode2;
	}

	if (eug1->poleList.size() == 3) {
		SimpleCircuit* simpleController = new SimpleCircuit(SIMPLE_CIRCUIT_CONTROLLER);
		SimpleCircuit* subSimpleController0 = getSimpleController(eug1->subCompList[0]);
		for (size_t i = 0; i < subSimpleController0->supList.size(); i++)
		{
			simpleController->supList.push_back(subSimpleController0->supList[i]);
		}
		
		//Store all the switch of sub-controller to controller
		for (size_t i = 0; i < subSimpleController0->switchList.size(); i++)
		{
			simpleController->switchList.push_back(subSimpleController0->switchList[i]);
		}

		size_t M1 = 2;
		size_t M2 = 1;

		SimpleCircuit** simplePseudoSuperNode2s = new SimpleCircuit*[M1];
		//cout << "           linked the circuit      getSimpleController          " << endl;


		//sn(2)
		simplePseudoSuperNode2s[0] = getSimplePseudoSuperNode2(eug1->sn2List[0]->normalList[0],
			eug1->sn2List[0]->normalList[1], eug1->sn2List[0]->normalList[2]);

		simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[0]->simpleGateList[0]);
		simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[0]->simpleGateList[1]);
		simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[0]->simpleGateList[2]);

		simplePseudoSuperNode2s[0]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[0 / 2 * 3 + 0 % 2]->output0;
		//cout << simplePseudoSuperNode2s[0]->switchList[0]->controlNode->id << "    ->   "
		//	<< subSimpleController0->switchList[0 / 2 * 3 + 0 % 2]->controlNode->id << endl;

		subSimpleController0->simpleGateList[0 / 2 * 3 + 0 % 2 + 1]->input0 = &simplePseudoSuperNode2s[0]->simpleGateList[2]->output0;
		//cout << subSimpleController0->simpleGateList[0 / 2 * 3 + 0 % 2 + 1]->controlNode->id << "    ->   "
		//	<< simplePseudoSuperNode2s[0]->simpleGateList[2]->controlNode->id << endl;


		subSimpleController0->simpleGateList[0 / 2 * 3 + 0 % 2]->postGateList0.push_back(simplePseudoSuperNode2s[0]->simpleGateList[0]);
		simplePseudoSuperNode2s[0]->simpleGateList[0]->hasinput0++;
		simplePseudoSuperNode2s[0]->simpleGateList[2]->postGateList0.push_back(subSimpleController0->simpleGateList[0 / 2 * 3 + 0 % 2 + 1]);
		subSimpleController0->simpleGateList[0 / 2 * 3 + 0 % 2 + 1]->hasinput0++;


		//sn(1)
		simplePseudoSuperNode2s[1] = getSimplePseudoSuperNode1(eug1->sn2List[1]->poleList[0]);
		
		simplePseudoSuperNode2s[1]->simpleGateList[0]->isSN1 = true;
		simplePseudoSuperNode2s[1]->simpleGateList[0]->controlNode->control = false;




		simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[1]->simpleGateList[0]);

		simpleController->supList.push_back(simplePseudoSuperNode2s[1]->switchList[0]);
		simplePseudoSuperNode2s[1]->switchList[0]->input1 = &SimpleGate::FALSE;
		simplePseudoSuperNode2s[1]->switchList[0]->inputReady1 = true;


		simpleController->supList.push_back(simplePseudoSuperNode2s[0]->switchList[0]);
		simplePseudoSuperNode2s[0]->switchList[0]->input1 = &SimpleGate::FALSE;
		simplePseudoSuperNode2s[0]->switchList[0]->inputReady1 = true;


		simplePseudoSuperNode2s[1]->simpleGateList[0]->input0 = &(subSimpleController0->simpleGateList[1 / 2 * 3 + 1 % 2]->output0);//3.15

		//cout << simplePseudoSuperNode2s[1]->switchList[0]->controlNode->id << "    ->   "
		//	<< subSimpleController0->switchList[1 / 2 * 3 + 1 % 2]->controlNode->id << endl;


		subSimpleController0->simpleGateList[1 / 2 * 3 + 1 % 2]->postGateList0.push_back(simplePseudoSuperNode2s[1]->simpleGateList[0]);//3.15 postGateList1
		simplePseudoSuperNode2s[1]->simpleGateList[0]->hasinput0++;

		/**
		for (uint32_t i = 0; i < M1; i++)
		{
			if (eug1->sn2List[i]->poleList.size() == 2) {
				simplePseudoSuperNode2s[i] = getSimplePseudoSuperNode2(eug1->sn2List[i]->normalList[0],
					eug1->sn2List[i]->normalList[1], eug1->sn2List[i]->normalList[2]);

				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[1]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[2]);

				simplePseudoSuperNode2s[i]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->output0;
				cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
					<< subSimpleController0->switchList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output0;
				cout << subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
					<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;


				subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
				simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput0++;
				simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
			}
			else //eug1->sn2List[i]->poleList.size() == 1
			{
				simplePseudoSuperNode2s[i] = getSimplePseudoSuperNode1(eug1->sn2List[i]->poleList[0]);

				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);

				simplePseudoSuperNode2s[i]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[i / 2 * 3]->output0;
				cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
					<< subSimpleController0->switchList[i / 2 * 3]->controlNode->id << endl;

				subSimpleController0->simpleGateList[i / 2 * 3]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[0]->output0;
				cout << subSimpleController0->simpleGateList[i / 2 * 3]->controlNode->id << "    ->   "
					<< simplePseudoSuperNode2s[i]->simpleGateList[0]->controlNode->id << endl;


				subSimpleController0->simpleGateList[i / 2 * 3]->postGateList0.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
				simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput0++;
				simplePseudoSuperNode2s[i]->simpleGateList[0]->postGateList0.push_back(subSimpleController0->simpleGateList[i / 2 * 3]);
				subSimpleController0->simpleGateList[i / 2 * 3]->hasinput0++;
			}
		}
		*/
		return simpleController;
	}

	if (eug1->poleList.size() > 3) {
		SimpleCircuit* simpleController = new SimpleCircuit(SIMPLE_CIRCUIT_CONTROLLER);

		SimpleCircuit* subSimpleController0 = getSimpleController(eug1->subCompList[0]);
		SimpleCircuit* subSimpleController1 = getSimpleController(eug1->subCompList[1]);

		for (size_t i = 0; i < subSimpleController0->supList.size(); i++)
		{
			simpleController->supList.push_back(subSimpleController0->supList[i]);
		}


		for (size_t i = 0; i < subSimpleController1->supList.size(); i++)
		{
			simpleController->supList.push_back(subSimpleController1->supList[i]);
		}

		//Store all the switch of sub-controller to controller
		for (size_t i = 0; i < subSimpleController0->switchList.size(); i++)
		{
			simpleController->switchList.push_back(subSimpleController0->switchList[i]);
		}


		for (size_t i = 0; i < subSimpleController1->switchList.size(); i++)
		{
			simpleController->switchList.push_back(subSimpleController1->switchList[i]);
		}

		size_t M1 = (eug1->poleList.size() + 1) / 2; //the number of pole in the left size
		size_t M2 = eug1->poleList.size() / 2; //the number of pole in the right size

		size_t M1temp = M1 % 2;
		size_t M2temp = M2 % 2;

		SimpleCircuit** simplePseudoSuperNode2s = new SimpleCircuit*[M1];

		if (M2temp == 0) 
		{ //all supernode of the right size are 2-way supernode
			for (uint32_t i = 0; i < M2; i++)
			{
				simplePseudoSuperNode2s[i] = getSimplePseudoSuperNode2(eug1->sn2List[i]->normalList[0],
					eug1->sn2List[i]->normalList[1], eug1->sn2List[i]->normalList[2]);

				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[1]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[2]);

				simplePseudoSuperNode2s[i]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->output0;
				//cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
				//	<< subSimpleController0->switchList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output0;
				//cout << subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;

				simplePseudoSuperNode2s[i]->simpleGateList[0]->input1 = &subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->output0;
				//cout << simplePseudoSuperNode2s[i]->simpleGateList[0]->controlNode->id << "    ->   "
				//	<< subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;


				subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output1;
				//cout << subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;

				subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
				simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput0++;
				
				simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
				
				subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
				simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput1++;
				
				simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
			}

			//for SN(1) in left.
			if (M1temp == 1)
			{
				simplePseudoSuperNode2s[M2] = getSimplePseudoSuperNode1(eug1->sn2List[M2]->poleList[0]);
				
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->isSN1 = true;
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->controlNode->control = false;

				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[M2]->simpleGateList[0]);

				simplePseudoSuperNode2s[M2]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2]->output0; //3.13 output1
				//cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
				//	<< subSimpleController0->switchList[i / 2 * 3 + i % 2]->controlNode->id << endl;


				subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2]->postGateList0.push_back(simplePseudoSuperNode2s[M2]->simpleGateList[0]);
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->hasinput0++;
				
				//simpleController->supList.push_back(simplePseudoSuperNode2s[M2]->switchList[0]); 3.13

				simplePseudoSuperNode2s[M2]->switchList[0]->input1 = &SimpleGate::FALSE; //3.12  22.22
				simplePseudoSuperNode2s[M2]->switchList[0]->inputReady1 = true; //3.12  22.22
				//simplePseudoSuperNode2s[M2]->switchList[0]->inputReady0 = true; 3.13

				/** 3.13
				simplePseudoSuperNode2s[0]->switchList[0]->input0 = &SimpleGate::FALSE;
				simplePseudoSuperNode2s[0]->switchList[0]->input1 = &SimpleGate::FALSE;
				simplePseudoSuperNode2s[0]->switchList[0]->inputReady0 = true;
				simplePseudoSuperNode2s[0]->switchList[0]->inputReady1 = true;
				*/

			}
		}

		if (M2temp == 1) {
			for (uint32_t i = 0; i < M2 - 1; i++)
			{
				simplePseudoSuperNode2s[i] = getSimplePseudoSuperNode2(eug1->sn2List[i]->normalList[0],
					eug1->sn2List[i]->normalList[1], eug1->sn2List[i]->normalList[2]);

				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[1]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[2]);

				simplePseudoSuperNode2s[i]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->output0;
				//cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
				//	<< subSimpleController0->switchList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output0;
				//cout << subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;

				simplePseudoSuperNode2s[i]->simpleGateList[0]->input1 = &subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->output0;
				//cout << simplePseudoSuperNode2s[i]->simpleGateList[0]->controlNode->id << "    ->   "
				//	<< subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;


				subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output1;
				//cout << subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;

				subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
				simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput0++;

				simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;

				subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
				simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput1++;

				simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]);
				subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
			}
			
			
			if (M1temp == 0) 
			{
				// for SN(2) in the main side
				simplePseudoSuperNode2s[(M2 - 1)] = getSimplePseudoSuperNode2(eug1->sn2List[(M2 - 1)]->normalList[0],
					eug1->sn2List[(M2 - 1)]->normalList[1], eug1->sn2List[(M2 - 1)]->normalList[2]);

				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[1]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[2]);

				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2]->output0;
				//cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
				//	<< subSimpleController0->switchList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]->input0 = &simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[2]->output0;
				//cout << subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
				//	<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;

				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->input1 = &subSimpleController1->simpleGateList[(M2 - 1) / 2 * 3]->output0;
				//cout << simplePseudoSuperNode2s[i]->simpleGateList[0]->controlNode->id << "    ->   "
				//	<< subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2]->postGateList0.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]);
				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->hasinput0++;

				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[2]->postGateList0.push_back(subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]);
				subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]->hasinput0++;

				subSimpleController1->simpleGateList[(M2 - 1) / 2 * 3]->postGateList1.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]);
				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->hasinput1++;


				//for SN(1) in the main side
				simplePseudoSuperNode2s[M2] = getSimplePseudoSuperNode1(eug1->sn2List[M2]->poleList[0]);

				simplePseudoSuperNode2s[M2]->simpleGateList[0]->isSN1 = true;
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->controlNode->control = false;

				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[M2]->simpleGateList[0]);

				simplePseudoSuperNode2s[M2]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2]->output0; //3.13 output1
				//cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
				//	<< subSimpleController0->switchList[i / 2 * 3 + i % 2]->controlNode->id << endl;


				subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2]->postGateList0.push_back(simplePseudoSuperNode2s[M2]->simpleGateList[0]);
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->hasinput0++;

				//simpleController->supList.push_back(simplePseudoSuperNode2s[M2]->switchList[0]); 3.13

				simplePseudoSuperNode2s[M2]->switchList[0]->input1 = &SimpleGate::FALSE; //3.12  22.22
				simplePseudoSuperNode2s[M2]->switchList[0]->inputReady1 = true; //3.12  22.22

			}
			if (M1temp == 1) 
			{
				simplePseudoSuperNode2s[(M2 - 1)] = getSimplePseudoSuperNode2(eug1->sn2List[(M2 - 1)]->normalList[0],
					eug1->sn2List[(M2 - 1)]->normalList[1], eug1->sn2List[(M2 - 1)]->normalList[2]);

				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[1]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[2]);

				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3]->output0;
				//cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
				//	<< subSimpleController0->switchList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->input1 = &subSimpleController1->simpleGateList[(M2 - 1) / 2 * 3]->output0;
				//cout << simplePseudoSuperNode2s[i]->simpleGateList[0]->controlNode->id << "    ->   "
				//	<< subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;

				subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3]->postGateList0.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]);
				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->hasinput0++;

				subSimpleController1->simpleGateList[(M2 - 1) / 2 * 3]->postGateList1.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]);
				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->hasinput1++;
			}
		}

		/**
		if (eug1->poleList.size() % 2 == 0) //all supernodes are SN(2)
		{
			SimpleCircuit** simplePseudoSuperNode2s = new SimpleCircuit*[M1];
			//cout << "           linked the circuit      getSimpleController          " << endl;
			if (M1 % 2 == 0) //all supernode of subeug also are SN(2)
			{
				for (uint32_t i = 0; i < M1; i++)
				{
					simplePseudoSuperNode2s[i] = getSimplePseudoSuperNode2(eug1->sn2List[i]->normalList[0],
						eug1->sn2List[i]->normalList[1], eug1->sn2List[i]->normalList[2]);

					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[1]);
					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[2]);

					simplePseudoSuperNode2s[i]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->output0;
					//cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
					//	<< subSimpleController0->switchList[i / 2 * 3 + i % 2]->controlNode->id << endl;

					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output0;
					//cout << subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
					//	<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;

					simplePseudoSuperNode2s[i]->simpleGateList[0]->input1 = &subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->output0;
					//cout << simplePseudoSuperNode2s[i]->simpleGateList[0]->controlNode->id << "    ->   "
					//	<< subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;


					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output1;
					//cout << subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
					//	<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;



					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput0++;
					simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]);
					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput1++;
					simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]);
					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
					uint32_t a = 0;
				}
			}
			else // (M1 % 2 != 0) 
			{
				for (uint32_t i = 0; i < M1 - 1; i++)
				{
					simplePseudoSuperNode2s[i] = getSimplePseudoSuperNode2(eug1->sn2List[i]->normalList[0],
						eug1->sn2List[i]->normalList[1], eug1->sn2List[i]->normalList[2]);

					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[1]);
					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[2]);

					simplePseudoSuperNode2s[i]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->output0;
					//cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
					//	<< subSimpleController0->switchList[i / 2 * 3 + i % 2]->controlNode->id << endl;

					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output0;
					//cout << subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
					//	<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;

					simplePseudoSuperNode2s[i]->simpleGateList[0]->input1 = &subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->output0;
					//cout << simplePseudoSuperNode2s[i]->simpleGateList[0]->controlNode->id << "    ->   "
					//	<< subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;


					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output1;
					//cout << subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
					//	<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;

					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput0++;
					simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]);
					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput1++;
					simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]);
					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
					uint32_t a = 0;
				}

				//SN(M1 - 1)
				simplePseudoSuperNode2s[(M1 - 1)] = getSimplePseudoSuperNode2(eug1->sn2List[(M1 - 1)]->normalList[0],
					eug1->sn2List[(M1 - 1)]->normalList[1], eug1->sn2List[(M1 - 1)]->normalList[2]);

				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[1]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[2]);

				simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[(M1 - 1) / 2 * 3]->output0;
				//cout << simplePseudoSuperNode2s[(M1 - 1)]->switchList[0]->controlNode->id << "    ->   " 
				//	<< subSimpleController0->simpleGateList[(M1 - 1) / 2 * 3]->controlNode->id << endl;

				subSimpleController0->simpleGateList[(M1 - 1) / 2 * 3]->input0 = &simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[2]->output0;
				//cout << subSimpleController0->simpleGateList[(M1 - 1) / 2 * 3]->controlNode->id << "    ->   "
				//	<< simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[2]->controlNode->id << endl;

				simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]->input1 = &subSimpleController1->simpleGateList[(M1 - 1)]->output0;
				//cout << simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]->controlNode->id << "    ->   "
				//	<< subSimpleController1->simpleGateList[(M1 - 1) / 2 * 3]->controlNode->id << endl;


				subSimpleController1->simpleGateList[(M1 - 1) / 2 * 3]->input0 = &simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[2]->output1;
				//cout << subSimpleController1->simpleGateList[(M1 - 1) / 2 * 3]->controlNode->id << "    ->   "
				//	<< simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[2]->controlNode->id << endl;



				subSimpleController0->simpleGateList[(M1 - 1) / 2 * 3]->postGateList0.push_back(simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]);
				simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]->hasinput0++;
				simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[2]->postGateList0.push_back(subSimpleController0->simpleGateList[(M1 - 1) / 2 * 3]);
				subSimpleController0->simpleGateList[(M1 - 1) / 2 * 3]->hasinput0++;
				subSimpleController1->simpleGateList[(M1 - 1) / 2 * 3]->postGateList1.push_back(simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]);
				simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]->hasinput1++;
				simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[2]->postGateList0.push_back(subSimpleController1->simpleGateList[(M1 - 1) / 2 * 3]);
				subSimpleController1->simpleGateList[(M1 - 1) / 2 * 3]->hasinput0++;

				//cout << " simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]->hasinput0 " << simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]->hasinput0 << endl;
				//cout << " subSimpleController0->simpleGateList[(M1 - 1) / 2 * 3]->hasinput0 " << subSimpleController0->simpleGateList[(M1 - 1) / 2 * 3]->hasinput0 << endl;


				//cout << " simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]->hasinput1 " << simplePseudoSuperNode2s[(M1 - 1)]->simpleGateList[0]->hasinput1 << endl;
				//cout << " subSimpleController1->simpleGateList[(M1 - 1) / 2 * 3]->hasinput0 " << subSimpleController1->simpleGateList[(M1 - 1) / 2 * 3]->hasinput0 << endl;

			}
		}
		else //eug1->poleList.size() % 2 != 0 have one SN(1)
		{
			SimpleCircuit** simplePseudoSuperNode2s = new SimpleCircuit*[M1];
			//cout << "           linked the circuit      getSimpleController          " << endl;
			if (M2 % 2 == 0) {
				for (uint32_t i = 0; i < M2; i++)
				{
					simplePseudoSuperNode2s[i] = getSimplePseudoSuperNode2(eug1->sn2List[i]->normalList[0],
						eug1->sn2List[i]->normalList[1], eug1->sn2List[i]->normalList[2]);

					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[1]);
					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[2]);

					simplePseudoSuperNode2s[i]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->output0;
					cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
						<< subSimpleController0->switchList[i / 2 * 3 + i % 2]->controlNode->id << endl;

					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output0;
					cout << subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
						<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;

					simplePseudoSuperNode2s[i]->simpleGateList[0]->input1 = &subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->output0;
					cout << simplePseudoSuperNode2s[i]->simpleGateList[0]->controlNode->id << "    ->   "
						<< subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;


					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output1;
					cout << subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
						<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;



					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput0++;
					simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]);
					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput1++;
					simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]);
					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;

				}

				//SN(1)
				simplePseudoSuperNode2s[M2] = getSimplePseudoSuperNode1(eug1->sn2List[M2]->poleList[0]);
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->isSN1 = true;

				simplePseudoSuperNode2s[M2]->simpleGateList[0]->controlNode->control = false;
				
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[M2]->simpleGateList[0]);

				simpleController->supList.push_back(simplePseudoSuperNode2s[M2]->switchList[0]);
				
				simplePseudoSuperNode2s[M2]->switchList[0]->input1 = &SimpleGate::FALSE; //3.12  22.22
				simplePseudoSuperNode2s[M2]->switchList[0]->inputReady1 = true; //3.12  22.22
				simplePseudoSuperNode2s[M2]->switchList[0]->inputReady0 = true;


				simplePseudoSuperNode2s[0]->switchList[0]->input0 = &SimpleGate::FALSE;
				simplePseudoSuperNode2s[0]->switchList[0]->input1 = &SimpleGate::FALSE;
				simplePseudoSuperNode2s[0]->switchList[0]->inputReady0 = true;
				simplePseudoSuperNode2s[0]->switchList[0]->inputReady1 = true;
				
				
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[M2 / 2 * 3]->output1;
				cout << simplePseudoSuperNode2s[M2]->simpleGateList[0]->controlNode->id << "    ->   "
					<< subSimpleController0->simpleGateList[M2 / 2 * 3]->controlNode->id << endl;

				subSimpleController0->simpleGateList[M2 / 2 * 3]->input1 = &simplePseudoSuperNode2s[M2]->simpleGateList[0]->output0;
				cout << subSimpleController0->simpleGateList[M2 / 2 * 3]->controlNode->id << "    ->   "
					<< simplePseudoSuperNode2s[M2]->simpleGateList[0]->controlNode->id << endl;



				subSimpleController0->simpleGateList[M2 / 2 * 3]->postGateList0.push_back(simplePseudoSuperNode2s[M2]->simpleGateList[0]);
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->hasinput0++;
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->postGateList0.push_back(subSimpleController0->simpleGateList[M2 / 2 * 3]);
				subSimpleController0->simpleGateList[M2 / 2 * 3]->hasinput0++;
				
				cout << " subSimpleController0->simpleGateList[M2 / 2 * 3]->hasinput0 " << subSimpleController0->simpleGateList[M2 / 2 * 3]->hasinput0 << endl;
				cout << " simplePseudoSuperNode2s[M2]->simpleGateList[0]->hasinput0 " << simplePseudoSuperNode2s[M2]->simpleGateList[0]->hasinput0 << endl;



			}
			else // (M2 % 2 != 0) 
			{
				for (uint32_t i = 0; i < M2 - 1; i++)
				{
					simplePseudoSuperNode2s[i] = getSimplePseudoSuperNode2(eug1->sn2List[i]->normalList[0],
						eug1->sn2List[i]->normalList[1], eug1->sn2List[i]->normalList[2]);

					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[1]);
					simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[i]->simpleGateList[2]);

					simplePseudoSuperNode2s[i]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->output0;
					cout << simplePseudoSuperNode2s[i]->switchList[0]->controlNode->id << "    ->   "
						<< subSimpleController0->switchList[i / 2 * 3 + i % 2]->controlNode->id << endl;

					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output0;
					cout << subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
						<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;

					simplePseudoSuperNode2s[i]->simpleGateList[0]->input1 = &subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->output0;
					cout << simplePseudoSuperNode2s[i]->simpleGateList[0]->controlNode->id << "    ->   "
						<< subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->controlNode->id << endl;


					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->input0 = &simplePseudoSuperNode2s[i]->simpleGateList[2]->output1;
					cout << subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->controlNode->id << "    ->   "
						<< simplePseudoSuperNode2s[i]->simpleGateList[2]->controlNode->id << endl;



					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2]->postGateList0.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput0++;
					simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]);
					subSimpleController0->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2]->postGateList1.push_back(simplePseudoSuperNode2s[i]->simpleGateList[0]);
					simplePseudoSuperNode2s[i]->simpleGateList[0]->hasinput1++;
					simplePseudoSuperNode2s[i]->simpleGateList[2]->postGateList0.push_back(subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]);
					subSimpleController1->simpleGateList[i / 2 * 3 + i % 2 + 1]->hasinput0++;
					uint32_t a = 0;
				}

				// simplePseudoSuperNode2s[(M2-1)]
				simplePseudoSuperNode2s[(M2 - 1)] = getSimplePseudoSuperNode2(eug1->sn2List[(M2 - 1)]->normalList[0],
					eug1->sn2List[(M2 - 1)]->normalList[1], eug1->sn2List[(M2 - 1)]->normalList[2]);

				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[1]);
				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[2]);

				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2]->output0;
				cout << simplePseudoSuperNode2s[(M2 - 1)]->switchList[0]->controlNode->id << "    ->   "
					<< subSimpleController0->switchList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2]->controlNode->id << endl;

				subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]->input0 = &simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[2]->output0;
				cout << subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]->controlNode->id << "    ->   "
					<< simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[2]->controlNode->id << endl;

				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->input1 = &subSimpleController1->simpleGateList[(M2 - 1) / 2 * 3]->output0;
				cout << simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->controlNode->id << "    ->   "
					<< subSimpleController1->simpleGateList[(M2 - 1) / 2 * 3]->controlNode->id << endl;


				subSimpleController1->simpleGateList[(M2 - 1) / 2 * 3]->input0 = &simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[2]->output1;
				cout << subSimpleController1->simpleGateList[(M2 - 1) / 2 * 3]->controlNode->id << "    ->   "
					<< simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[2]->controlNode->id << endl;



				subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2]->postGateList0.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]);
				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->hasinput0++;
				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[2]->postGateList0.push_back(subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]);
				subSimpleController0->simpleGateList[(M2 - 1) / 2 * 3 + (M2 - 1) % 2 + 1]->hasinput0++;
				subSimpleController1->simpleGateList[(M2 - 1) / 2 * 3]->postGateList1.push_back(simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]);
				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[0]->hasinput1++;
				simplePseudoSuperNode2s[(M2 - 1)]->simpleGateList[2]->postGateList0.push_back(subSimpleController1->simpleGateList[(M2 - 1) / 2 * 3]);
				subSimpleController1->simpleGateList[(M2 - 1) / 2 * 3]->hasinput0++;


				// simplePseudoSuperNode2s[M2]
				simplePseudoSuperNode2s[M2] = getSimplePseudoSuperNode1(eug1->sn2List[M2]->poleList[0]);
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->isSN1 = true;


				simplePseudoSuperNode2s[M2]->simpleGateList[0]->controlNode->control = false;

				simpleController->supList.push_back(simplePseudoSuperNode2s[M2]->switchList[0]);
				simplePseudoSuperNode2s[M2]->switchList[0]->input1 = &SimpleGate::FALSE;
				simplePseudoSuperNode2s[M2]->switchList[0]->inputReady1 = true;
				simplePseudoSuperNode2s[M2]->switchList[0]->inputReady0 = true;


				simpleController->simpleGateList.push_back(simplePseudoSuperNode2s[M2]->simpleGateList[0]);

				simplePseudoSuperNode2s[M2]->simpleGateList[0]->input0 = &subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2]->output1;
				cout << simplePseudoSuperNode2s[M2]->switchList[0]->controlNode->id << "    ->   "
					<< subSimpleController0->switchList[M2 / 2 * 3 + M2 % 2]->controlNode->id << endl;

				subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2 + 1]->input1 = &simplePseudoSuperNode2s[M2]->simpleGateList[0]->output0;
				cout << subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2 + 1]->controlNode->id << "    ->   "
					<< simplePseudoSuperNode2s[M2]->simpleGateList[0]->controlNode->id << endl;


				subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2]->postGateList0.push_back(simplePseudoSuperNode2s[M2]->simpleGateList[0]);
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->hasinput0++;
				simplePseudoSuperNode2s[M2]->simpleGateList[0]->postGateList0.push_back(subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2 + 1]);
				subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2 + 1]->hasinput0++;
				cout << " subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2 + 1]->hasinput0 " << subSimpleController0->simpleGateList[M2 / 2 * 3 + M2 % 2 + 1]->hasinput0 << endl;
				cout << " simplePseudoSuperNode2s[M2]->simpleGateList[0]->hasinput0 " << simplePseudoSuperNode2s[M2]->simpleGateList[0]->hasinput0 << endl;

			}



			return simpleController;
		}
		*/

		//cout << "eug1->poleList.size()    =    " << eug1->poleList.size() << endl;
		return simpleController;
	}
	
	return nullptr;
}


// test:
// t1 = (p||q)->(r&&s)
// t2 = (p||q)^(r&&s)
//


void SimpleCircuit::testCircuit()
{
	//SimpleCircuit* simpleCircuit = new SimpleCircuit(SIMPLE_CIRCUIT_DEFAULT);

	EugNode** controlNodes = new EugNode*[7];
	for (size_t i = 0; i < 7; i++)
	{
		controlNodes[i] = new EugNode(EUG_NODE_DEFAULT, nullptr, 0u);
	}
	
	controlNodes[0]->dagNodeType = DAG_NODE_HEAD;
	controlNodes[1]->dagNodeType = DAG_NODE_HEAD;
	controlNodes[2]->dagNodeType = DAG_NODE_HEAD;
	controlNodes[3]->dagNodeType = DAG_NODE_HEAD;
	controlNodes[4]->dagNodeType = DAG_NODE_NORMAL;
	controlNodes[5]->dagNodeType = DAG_NODE_NORMAL;
	controlNodes[6]->dagNodeType = DAG_NODE_TAIL;

	controlNodes[0]->gateType = 1u;
	controlNodes[1]->gateType = 1u;
	controlNodes[2]->gateType = 1u;
	controlNodes[3]->gateType = 1u;
	controlNodes[4]->gateType = 7u;
	controlNodes[5]->gateType = 1u;
	controlNodes[6]->gateType = 13u;

	SimpleGate** simpleGates = new SimpleGate*[8];
	for (size_t i = 0; i < 8; i++)
	{
		simpleGates[i] = new SimpleUniversalGate(controlNodes[i]);
	}

	simpleGates[0]->postGateList0.push_back(simpleGates[4]);
	simpleGates[1]->postGateList1.push_back(simpleGates[4]);
	simpleGates[2]->postGateList0.push_back(simpleGates[5]);
	simpleGates[3]->postGateList1.push_back(simpleGates[5]);
	simpleGates[4]->postGateList0.push_back(simpleGates[6]);
	simpleGates[5]->postGateList1.push_back(simpleGates[6]);

	simpleGates[4]->input0 = &simpleGates[0]->output0;
	simpleGates[4]->input1 = &simpleGates[1]->output0;
	simpleGates[5]->input0 = &simpleGates[2]->output0;
	simpleGates[5]->input1 = &simpleGates[3]->output0;
	simpleGates[6]->input0 = &simpleGates[4]->output0;
	simpleGates[6]->input1 = &simpleGates[5]->output0;

	bool input0 = true;
	bool input1 = false;
	bool input2 = true;
	bool input3 = true;

	simpleGates[0]->input0 = &input0;
	simpleGates[0]->input1 = &input0;
	simpleGates[1]->input0 = &input1;
	simpleGates[1]->input1 = &input1;
	simpleGates[2]->input0 = &input2;
	simpleGates[2]->input1 = &input2;
	simpleGates[3]->input0 = &input3;
	simpleGates[3]->input1 = &input3;

	simpleGates[0]->inputReady0 = true;
	simpleGates[0]->inputReady1 = true;
	simpleGates[1]->inputReady0 = true;
	simpleGates[1]->inputReady1 = true;
	simpleGates[2]->inputReady0 = true;
	simpleGates[2]->inputReady1 = true;
	simpleGates[3]->inputReady0 = true;
	simpleGates[3]->inputReady1 = true;

	//simpleGates[0]->generateOutputChain();
	//simpleGates[1]->generateOutputChain();
	//simpleGates[2]->generateOutputChain();
	//simpleGates[3]->generateOutputChain();

	cout << simpleGates[6]->output0  << endl;
}


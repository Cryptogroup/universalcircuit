//#include "stdafx.h"
#include "SimpleXSwitch.h"


SimpleXSwitch::SimpleXSwitch(EugNode* controlNode)
	: SimpleGate(controlNode)
{
}


SimpleXSwitch::~SimpleXSwitch()
{
}

bool SimpleXSwitch::generateOutput()
{
	if ((inputReady0 && inputReady1) == false)
	{
		return false;
	}

	if (controlNode->control == true)
	{
		if (input1 == nullptr) {
			output0 = false;
			input1 = &output0;
		}
		output0 = *input1;
		output1 = *input0;
		
		
		cout << "=================ONE XSWITCH========== "<< endl; 
		cout << "controlNode->ID " << controlNode->id << endl;
	    cout << "controlNode->control " << controlNode->control << endl;
		cout << "InputReady0 " << inputReady0 << endl;
		cout << "InputReady1 " << inputReady1 << endl;
		cout << "Input0 of XSwitch: " << *input0 << endl;
		cout << "Input1 of XSwitch: " << *input1 << endl;
		cout << "output0 of XSwitch: " << output0 << endl;
		cout << "output1 of XSwitch: " << output1 << endl;
	   
	}
	else
	{
		output0 = *input0;
		output1 = *input1;
		
		/*
		cout << "=================ONE XSWITCH========== " << endl;
		cout << "controlNode->ID " << controlNode->id << endl;
		cout << "controlNode->control " << controlNode->control << endl;
		cout << "InputReady0 " << inputReady0 << endl;
		cout << "InputReady1 " << inputReady1 << endl;
		cout << "Input0 of XSwitch: " << *input0 << endl;
		cout << "Input1 of XSwitch: " << *input1 << endl;
		cout << "output0 of XSwitch: " << output0 << endl;
		cout << "output1 of XSwitch: " << output1 << endl;
	    */
	}
	inputReady1 = false;
	inputReady0 = false;
	SimpleGate::generateOutput();

	return true;
}

void SimpleXSwitch::clear()
{
	SimpleGate::clear();
}

//#include "stdafx.h"
#include "SimpleGate.h"
const bool SimpleGate::TRUE;
const bool SimpleGate::FALSE;

SimpleGate::SimpleGate(EugNode* controlNode)
{
	this->controlNode = controlNode;

	input0 = nullptr;
	input1 = nullptr;
	inputReady0 = false;
	inputReady1 = false;

	isSN1 = false;

	hasinput0 = 0u;
	hasinput1 = 0u;


	output0 = false;
	output1 = false;
	hasGeneratedOutput = false;
}


SimpleGate::~SimpleGate()
{
}


bool SimpleGate::generateOutput()
{
	hasGeneratedOutput = true;
	for (size_t i = 0; i < postGateList0.size(); i++)
	{
		if (postGateList0[i]->inputReady0 == true) 
		{
			
			cout << "        1111        postGateList0             " << endl;
			cout << "                     outlist      =  " << postGateList0[i]->controlNode->id << endl;
			cout << "        hasinput0      =  " << postGateList0[i]->hasinput0 << endl;
			cout << "        hasinput1      =  " << postGateList0[i]->hasinput1 << endl;
			
		}
		else 
		{
			postGateList0[i]->inputReady0 = true;
		}
	}
	for (size_t i = 0; i < postGateList1.size(); i++)
	{
		if (postGateList1[i]->inputReady1 == true)
		{
			
		    cout << "         1111       postGateList1             " << endl;
			cout << "                      outlist      =  " << postGateList1[i]->controlNode->id << endl;
			cout << "        inputReady0      =  " << postGateList1[i]->inputReady0 << endl;
			cout << "        inputReady1      =  " << postGateList1[i]->inputReady1 << endl;
			cout << "        hasinput0      =  " << postGateList1[i]->hasinput0 << endl;
			cout << "        hasinput1      =  " << postGateList1[i]->hasinput1 << endl;
			
		}
		else 
		{
			postGateList1[i]->inputReady1 = true;
		}
	}

	return true;
}
long long findnodeid(uint32_t num,vector< Node4Struct> nodelist)
{
	long long result = -1;
	for(uint32_t i=0;i<nodelist.size();i++)
	{
		if(nodelist[i].nodeid==num)
		{
			result =i;
			break;
		}
	}
	return result;
}
void SimpleGate::generateOutputChain(vector<Node4Struct> &nodelist)
{
	
	if (generateOutput())
	{ 
		

		/*	
		cout << "                computing the gate             " << endl;
		cout << "    1    computing      =  " << controlNode->id << endl;
		cout << "     controlNode->type      =  " << controlNode->type << endl;
		cout << "     controlNode->gateType   =  " << controlNode->gateType << endl;
		cout << "        inputReady0      =  " << inputReady0 << endl;
		cout << "        inputReady1      =  " << inputReady1 << endl;
		
 		*/	
		
	 	char addr[64] = {0};
		long long nodeid=-1,nodeAid=-1,nodeBid=-1;
		string s_addr1,s_addr2,s_addr3,s_addr4;

		
		
		cout << "=================ONE XSWITCH========== " << endl;
		cout << "controlNode->ID " << controlNode->id << endl;
		cout << "controlNode->control " << controlNode->control << endl;
		nodeid =findnodeid(controlNode->id,nodelist);
		cout<<"Judge nodeid:"<<nodeid<<endl;
		if(nodeid==-1)
		{
			cout<<"Create Node, nodeid:"<<nodeid<<endl;
			struct Node4Struct tempnode={-1,-1,-1,-1,-1,-1,-1,-1,-1,true};
			tempnode.nodeid=controlNode->id ;	
			nodelist.push_back(tempnode);
			nodeid=nodelist.size()-1;
			cout<<"Create Success, nodeid:"<<nodelist[nodeid].nodeid<<endl;
		}
		nodelist[nodeid].controlflag= controlNode->control;
		
	

		//outputprog<<controlNode->control<<endl;

		cout << "InputReady0 " << inputReady0 << endl;
		cout << "InputReady1 " << inputReady1 << endl;
		cout << "Input0 of XSwitch: " << *input0 << endl;
		cout << "Input1 of XSwitch: " << *input1 << endl;
		cout << "output0 of XSwitch: " << output0 << endl;
		cout << "output1 of XSwitch: " << output1 << endl;
		cout << "addition Input0 of XSwitch: " << input0 << endl;
		cout << "addition Input1 of XSwitch: " << input1 << endl;

		cout << "addition output0 of XSwitch: " << &output0 << endl;

		//judge which output is next Xswicth's input0,which output is next Xswicth's input1
		sprintf(addr,"%x",&output0);
		s_addr1 = addr;

		cout << "addition output1 of XSwitch: " << &output1 << endl;
		sprintf(addr,"%x",&output1);
		s_addr2 = addr;

		for(int i=0;i<postGateList0.size();i++)
		{
		cout<<"Saving Next NodeA"<<endl;
		//nodelist[nodeid].nextnodeA=postGateList0[i]->controlNode->id;
		nodeAid = findnodeid(postGateList0[i]->controlNode->id,nodelist);
		if(nodeAid==-1)
		{
			struct Node4Struct tempnode={-1,-1,-1,-1,-1,-1,-1,-1,-1,true};
			tempnode.nodeid=postGateList0[i]->controlNode->id;
			nodelist.push_back(tempnode);
			nodeAid=nodelist.size()-1;
		}
		sprintf(addr,"%x",postGateList0[i]->input0);
		s_addr3=addr;
		sprintf(addr,"%x",postGateList0[i]->input1);
		s_addr4=addr;
		if(s_addr3==s_addr1){
			nodelist[nodeid].nextnodeA=postGateList0[i]->controlNode->id;
			nodelist[nodeAid].prevnodeA=controlNode->id;
		}else if(s_addr3==s_addr2)
		{
			nodelist[nodeid].nextnodeB=postGateList0[i]->controlNode->id;
			nodelist[nodeAid].prevnodeA=controlNode->id;
		}
		else if(s_addr4==s_addr1){
			nodelist[nodeid].nextnodeA=postGateList0[i]->controlNode->id;
			nodelist[nodeAid].prevnodeB=controlNode->id;
		}else if(s_addr4==s_addr2){
			nodelist[nodeid].nextnodeB=postGateList0[i]->controlNode->id;
			nodelist[nodeAid].prevnodeB=controlNode->id;
		}
		cout<<"Next NodeA Saving End"<<endl;
		}


		for(int i=0;i<postGateList1.size();i++)
		{
			cout<<"Saving Next NodeB"<<endl;
		//nodelist[nodeid].nextnodeB=postGateList1[i]->controlNode->id;
		nodeBid = findnodeid(postGateList1[i]->controlNode->id,nodelist);
		if(nodeBid==-1)
		{
			struct Node4Struct tempnode={-1,-1,-1,-1,-1,-1,-1,-1,-1,true};
			tempnode.nodeid=postGateList1[i]->controlNode->id;
			nodelist.push_back(tempnode);
			nodeBid=nodelist.size()-1;
		}
		sprintf(addr,"%x",postGateList1[i]->input0);
		s_addr3=addr;
		sprintf(addr,"%x",postGateList1[i]->input1);
		s_addr4=addr;
		if(s_addr3==s_addr1)
		{
			nodelist[nodeid].nextnodeA=postGateList1[i]->controlNode->id;
			nodelist[nodeBid].prevnodeA=controlNode->id;
		}else if(s_addr3==s_addr2)
		{
			nodelist[nodeid].nextnodeB=postGateList1[i]->controlNode->id;
			nodelist[nodeBid].prevnodeA=controlNode->id;
		}else if(s_addr4==s_addr1){
			nodelist[nodeid].nextnodeA=postGateList1[i]->controlNode->id;
			nodelist[nodeBid].prevnodeB=controlNode->id;
		}else if(s_addr4==s_addr2){
			nodelist[nodeid].nextnodeB=postGateList1[i]->controlNode->id;
			nodelist[nodeBid].prevnodeB=controlNode->id;
		}
		cout<<"Next NodeB Saving End"<<endl;
		}
		//cout << "      postGateList0.size() =      " << postGateList0.size() << endl;
		//cout << "      postGateList1.size() =      " << postGateList1.size() << endl;
		

		
		for (size_t i = 0; i < postGateList0.size(); i++)
		{
			cout << "                postGateList0             " << endl;
			cout << "    1    outlist      =  " << postGateList0[i]->controlNode->id << endl;
			cout << "        inputReady0      =  " << postGateList0[i]->inputReady0 << endl;
			cout << "        inputReady1      =  " << postGateList0[i]->inputReady1 << endl;
			cout << "       Input0     =     " << *(postGateList0[i]->input0) << endl;
			cout << "       Input1     =     " << *(postGateList0[i]->input1) << endl;
			cout << "        hasinput0      =  " << hasinput0 << endl;
			cout << "        hasinput1      =  " << hasinput1 << endl;

			cout << "addition Input0 of XSwitch: " << postGateList0[i]->input0 << endl;
			cout << "addition Input1 of XSwitch: " << postGateList0[i]->input1 << endl;
			cout << "addition output0 of XSwitch: " << &(postGateList0[i]->output0) << endl;
			cout << "addition output1 of XSwitch: " << &(postGateList0[i]->output1) << endl;



		}
		for (size_t i = 0; i < postGateList1.size(); i++)
		{
			
		    cout << "                postGateList1             " << endl;
			cout << "   2     outlist      =  " << postGateList1[i]->controlNode->id << endl;
			cout << "        inputReady0      =  " << postGateList1[i]->inputReady0 << endl;
			cout << "        inputReady1      =  " << postGateList1[i]->inputReady1 << endl;
			if (postGateList1[i]->input0 != nullptr) {
				cout << "       Input0     =     " << *(postGateList1[i]->input0) << endl;
			}
			else {
				cout << "   No Input0     =     " << endl;
			}
			if (postGateList1[i]->input1 != nullptr) {
				cout << "       Input1     =     " << *(postGateList1[i]->input1) << endl;
			}
			cout << "        hasinput0      =  " << hasinput0 << endl;
			cout << "        hasinput1      =  " << hasinput1 << endl;
		  
			cout << "addition Input0 of XSwitch: " << postGateList1[i]->input0 << endl;
			cout << "addition Input1 of XSwitch: " << postGateList1[i]->input1 << endl;
			cout << "addition output0 of XSwitch: " << &(postGateList1[i]->output0) << endl;
			cout << "addition output1 of XSwitch: " << &(postGateList1[i]->output1) << endl;
 		

		}
		
		
		


		for (size_t i = 0; i < postGateList0.size(); i++)
		{
			/**
			if (isSN1 == true)
			{
				cout << "                computing the gate             " << endl;
				cout << "    1    computing      =  " << postGateList0[i]->controlNode->id << endl;
				cout << "        inputReady0      =  " << postGateList0[i]->inputReady0 << endl;
				cout << "        inputReady1      =  " << postGateList0[i]->inputReady1 << endl;

			}
			*/
			postGateList0[i]->generateOutputChain(nodelist);
		}
		for (size_t i = 0; i < postGateList1.size(); i++)
		{
			/**
			if (isSN1 == true)
			{
				cout << "                computing the gate             " << endl;
				cout << "    2    computing      =  " << postGateList1[i]->controlNode->id << endl;
				cout << "        inputReady0      =  " << postGateList1[i]->inputReady0 << endl;
				cout << "        inputReady1      =  " << postGateList1[i]->inputReady1 << endl;

			}
			*/
			postGateList1[i]->generateOutputChain(nodelist);
		}

	}
}

void SimpleGate::clear()
{
		inputReady0 = false;
		output0 = false;

		inputReady1 = false;
		output1 = false;
	
	hasGeneratedOutput = false;
}

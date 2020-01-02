
//#include "stdafx.h"
#include "EugComponent.h"

uint32_t EugComponent::getCompNextId()
{
	return compIdCounter++;
}

void EugComponent::removePoles()
{

}


void EugComponent::printEdges(/* Node4Side *NodeStruct*/)
{
	set<EugNode*> toPrint;
	set<EugNode*> printed;

	for (size_t i = 0; i < this->inList.size(); i++)
	{
		toPrint.insert(inList[i]);
		printed.insert(inList[i]);
	}

	while (!toPrint.empty())
	{
		EugNode* printNode = *toPrint.begin();
		toPrint.erase(toPrint.begin());

		if (printNode->posts[0] != nullptr)
		{
			cout << printNode->id << " -> " << printNode->posts[0]->id << endl;
			//if(printNode->id==89||printNode->posts[0]->id==89)exit(-1) ;
			/* 
			NodeStruct[printNode->id].nextNodeA = printNode->posts[0]->id ;
			NodeStruct[printNode->posts[0]->id].prevNodeA = printNode->id;
			NodeStruct[printNode->id].NullNodeFlag=false;
			NodeStruct[printNode->posts[0]->id].NullNodeFlag=false;
			*/
			if (printed.find(printNode->posts[0]) == printed.end()) // Cannot find in printed => Haven't been printed
			{
				toPrint.insert(printNode->posts[0]);
				printed.insert(printNode->posts[0]);
			}
		}

		if (printNode->posts[1] != nullptr)
		{
			cout << printNode->id << " -> " << printNode->posts[1]->id << endl;
			//if(printNode->id==89||printNode->posts[1]->id==89)exit(-1) ;
			/* 
			NodeStruct[printNode->id].nextNodeB = printNode->posts[1]->id ;
			NodeStruct[printNode->posts[1]->id].prevNodeB = printNode->id;
			NodeStruct[printNode->posts[1]->id].NullNodeFlag=false;
			*/
			if (printed.find(printNode->posts[1]) == printed.end()) // Cannot find in printed => Haven't been printed
			{
				toPrint.insert(printNode->posts[1]);
				printed.insert(printNode->posts[1]);
			}
		}
	}
}

void EugComponent::printPoles()
{
	
	cout << "Poles: " << endl;
	for (size_t i = 0; i < this->poleList.size(); i++)
	{
		cout << poleList[i]->id << " ";
	}
	cout << endl;

}

void EugComponent::printIns()
{
	
	cout << "Ins: " << endl;
	for (size_t i = 0; i < this->inList.size(); i++)
	{
		cout << inList[i]->id << " ";
	}
	cout << endl;
	
}


void EugComponent::setDag(Dag * dag)
{
#ifdef DEBUG
	//cout << "In setDat(...): Layer = " << this->layer << endl;
#endif // DEBUG
	this->myDag = dag;

	from.clear();
	to.clear();

	// Set pre-pole and post-pole of poles
	for (size_t i = 0; i < this->poleList.size(); i++)
	{
		if (this->myDag->nodeList[i]->posts[0] != nullptr)
		{
			poleList[i]->nextPole = poleList[this->myDag->nodeList[i]->posts[0]->id];
			poleList[this->myDag->nodeList[i]->posts[0]->id]->lastPole = poleList[i];
		}
	}

	//cout << "Setting DAG to: " << endl;
	dag->print();

	// Construct dags for subEugComponent

	//cout << "Creating Sub-DAG datastructure..." << endl;

	//cout << "dag->getSize() " << dag->getSize() << endl;

	if (dag->getSize() % 2 == 0) {
		size_t subDagSize = dag->getSize() / 2;

		Dag* Dag1 = new Dag(1, 1, subDagSize);
		Dag* Dag2 = new Dag(1, 1, subDagSize);

		// Initialize from and to for subDags
		
		for (size_t i = 0; i < dag->getSize(); i++)
		{
			if (dag->nodeList[i]->posts[0] == nullptr)
				continue;
			from.push_back(dag->nodeList[i]->id);
			to.push_back(dag->nodeList[i]->posts[0]->id);
		}

		for (size_t i = 0; i < to.size(); i++)
		{
			if (to[i] == dag->getSize()-1)
			{
				uint32_t temp = from[0];
				from[0] = from[i];
				from[i] = temp;
				temp = to[0];
				to[0] = to[i];
				to[i] = temp;
			}
		}

		for (size_t i = 0; i < from.size(); i++)
		{
			if (from[i] != UINT32_MAX)
			{
				//cout << "edgeEmbed: " << from[i] << " -> " << to[i] << endl;
				edgeEmbed(from[i], to[i], i, Dag1, Dag2);
			}
		}

		for (size_t i = 0; i < sn2List.size(); i++)
		{
			if (sn2List[i]->poleStyle == Sn2PoleStyle::POLE_STYLE_UNDECIDED)
				sn2List[i]->poleStyle = Sn2PoleStyle::POLE_STYLE_PARALLEL;
		}

		if (this->isSn2 == false)
		{
			this->subCompList[0]->setDag(Dag1);
			if(Dag2->getSize() > 2)
				this->subCompList[1]->setDag(Dag2);
		}

		for (size_t i = 0; i < poleList.size(); i++)
		{
			poleList[i]->gateType = dag->nodeList[i]->gateType;
			poleList[i]->dagNodeType = dag->nodeList[i]->type;
		}

		computeControl();
		//cout << "dag->getSize() % 2 == 0 finish  computeControl()" << endl;
	}
	else // dag->getSize() % 2 == 1
	{
		size_t subDagSize1 = (dag->getSize() + 1) / 2;
		size_t subDagSize2 = (dag->getSize() - 1) / 2;


		Dag* Dag1 = new Dag(1, 1, subDagSize1);
		Dag* Dag2 = new Dag(1, 1, subDagSize2);

		for (size_t i = 0; i < dag->getSize(); i++)
		{
			if (dag->nodeList[i]->posts[0] == nullptr)
				continue;
			from.push_back(dag->nodeList[i]->id);
			to.push_back(dag->nodeList[i]->posts[0]->id);
		}

		for (size_t i = 0; i < to.size(); i++)
		{
			if (to[i] == dag->getSize() - 1)
			{
				uint32_t temp = from[0];
				from[0] = from[i];
				from[i] = temp;
				temp = to[0];
				to[0] = to[i];
				to[i] = temp;
			}
		}

		for (size_t i = 0; i < from.size(); i++)
		{
			if (from[i] != UINT32_MAX)
			{
				//cout << "edgeEmbed: " << from[i] << " -> " << to[i] << endl;
				edgeEmbed(from[i], to[i], i, Dag1, Dag2);
			}
		}

		for (size_t i = 0; i < sn2List.size(); i++)
		{
			if (sn2List[i]->poleStyle == Sn2PoleStyle::POLE_STYLE_UNDECIDED)
				sn2List[i]->poleStyle = Sn2PoleStyle::POLE_STYLE_PARALLEL;
		}

		if (this->isSn2 == false)
		{
			this->subCompList[0]->setDag(Dag1);
			if (Dag2->getSize() > 1) 
				this->subCompList[1]->setDag(Dag2);
		}

		for (size_t i = 0; i < poleList.size(); i++)
		{
			poleList[i]->gateType = dag->nodeList[i]->gateType;
			poleList[i]->dagNodeType = dag->nodeList[i]->type;
		}

		computeControl();
		//cout << "dag->getSize() % 2 == 1  finish  computeControl()" << endl;
	}
}

void EugComponent::computeControl()
{
	if (isSn2 == false)
	{
		subCompList[0]->computeControl();
		

		uint32_t temp = poleList.size() % 2;
		//cout << "temp   " << temp << endl;

		if (poleList.size() > 3) 
			subCompList[1]->computeControl();

		if (temp == 1) 
		{
			sn2List[sn2List.size()-1]->poleList[0]->control = false;
		}

		for (size_t i = 0; i < sn2List.size()-temp; i++){
			if (sn2List[i]->inStyle == IN_STYLE_0)
			{
				sn2List[i]->normalList[0]->control = false;
			}
			else
			{
				sn2List[i]->normalList[0]->control = true;
			}
			
			if (sn2List[i]->poleStyle == POLE_STYLE_CASCADE)
			{
				sn2List[i]->normalList[1]->control = false;
			}
			else
			{
				sn2List[i]->normalList[1]->control = true;
			}

			if (sn2List[i]->outStyle == OUT_STYLE_0)
			{
				sn2List[i]->normalList[2]->control = true;
			}
			else
			{
				sn2List[i]->normalList[2]->control = false;
			}
		}
	}
}

void EugComponent::edgeEmbed(uint32_t pre, uint32_t post, size_t location, Dag* Dag1, Dag* Dag2)
{
	//this->poleList[pre]->nextPole[post];
	EugComponent* component = this->poleList[pre]->component;
	EugComponent* nextComponent = this->poleList[post]->component;
	if (pre == (post ^ 1u))
	{
		component->poleStyle = Sn2PoleStyle::POLE_STYLE_CASCADE;
		return;
	}
	from[location] = UINT32_MAX;
	to[location] = UINT32_MAX;

	//cout << "Linking poles: " << pre << "(" << this->poleList[pre]->id <<")" << " -> " << post << "(" << this->poleList[post]->id << ")" << " in ";
	
	if (pre % 2 == post % 2)
	{
		if (component->outStyle == Sn2OutStyle::OUT_STYLE_0)
		{
			nextComponent->inStyle = Sn2InStyle::IN_STYLE_0;
			if (pre % 2 == 0)
			{
				Dag1->addEdge(pre / 2, post / 2);
				//cout << "Left" << endl;
			}
			else
			{
				Dag2->addEdge(pre / 2, post / 2);
				//cout << "Right" << endl;
			}
		}
		else if(component->outStyle == Sn2OutStyle::OUT_STYLE_1)
		{
			nextComponent->inStyle = Sn2InStyle::IN_STYLE_1;
			if (pre % 2 == 0)
			{
				Dag2->addEdge(pre / 2, post / 2);
				//cout << "Right" << endl;
			}
			else
			{
				Dag1->addEdge(pre / 2, post / 2);
				//cout << "Left" << endl;
			}
		}
		else
		{
			if (nextComponent->inStyle == Sn2InStyle::IN_STYLE_0)
			{
				component->outStyle = Sn2OutStyle::OUT_STYLE_0;
				if (pre % 2 == 0)
				{
					Dag1->addEdge(pre / 2, post / 2);
					//cout << "Left" << endl;
				}
				else
				{
					Dag2->addEdge(pre / 2, post / 2);
					//cout << "Right" << endl;
				}
			}
			else if (nextComponent->inStyle == Sn2InStyle::IN_STYLE_1)
			{
				component->outStyle = Sn2OutStyle::OUT_STYLE_1;
				if (pre % 2 == 0)
				{
					Dag2->addEdge(pre / 2, post / 2);
					//cout << "Right" << endl;
				}
				else
				{
					Dag1->addEdge(pre / 2, post / 2);
					//cout << "Left" << endl;
				}
			}
			else
			{
				component->outStyle = Sn2OutStyle::OUT_STYLE_0;
				nextComponent->inStyle = Sn2InStyle::IN_STYLE_0;
				if (pre % 2 == 0)
				{
					Dag1->addEdge(pre / 2, post / 2);
					//cout << "Left" << endl;
				}
				else
				{
					Dag2->addEdge(pre / 2, post / 2);
					//cout << "Right" << endl;
				}
			}
		}
	}
	else
	{
		if (component->outStyle == Sn2OutStyle::OUT_STYLE_0)
		{
			nextComponent->inStyle = Sn2InStyle::IN_STYLE_1;
			if (pre % 2 == 0)
			{
				Dag1->addEdge(pre / 2, post / 2);
				//cout << "Left" << endl;
			}
			else
			{
				Dag2->addEdge(pre / 2, post / 2);
				//cout << "Right" << endl;
			}

		}
		else if(component->outStyle == Sn2OutStyle::OUT_STYLE_1)
		{
			nextComponent->inStyle = Sn2InStyle::IN_STYLE_0;
			if (pre % 2 == 1)
			{
				Dag1->addEdge(pre / 2, post / 2);
				//cout << "Left" << endl;
			}
			else
			{
				Dag2->addEdge(pre / 2, post / 2);
				//cout << "Right" << endl;
			}
		}
		else
		{
			if (nextComponent->inStyle == Sn2InStyle::IN_STYLE_0)
			{
				component->outStyle = Sn2OutStyle::OUT_STYLE_1;
				if (pre % 2 == 0)
				{
					Dag2->addEdge(pre / 2, post / 2);
					//cout << "Right" << endl;
				}
				else
				{
					Dag1->addEdge(pre / 2, post / 2);
					//cout << "Left" << endl;
				}
			}
			else if (nextComponent->inStyle == Sn2InStyle::IN_STYLE_1)
			{
				component->outStyle = Sn2OutStyle::OUT_STYLE_0;
				if (pre % 2 == 0)
				{
					Dag1->addEdge(pre / 2, post / 2);
					//cout << "Left" << endl;
				}
				else
				{
					Dag2->addEdge(pre / 2, post / 2);
					//cout << "Right" << endl;
				}
			}
			else
			{
				component->outStyle = Sn2OutStyle::OUT_STYLE_1;
				nextComponent->inStyle = Sn2InStyle::IN_STYLE_0;
				if (pre % 2 == 0)
				{
					Dag2->addEdge(pre / 2, post / 2);
					//cout << "Right" << endl;
				}
				else
				{
					Dag1->addEdge(pre / 2, post / 2);
					//cout << "Left" << endl;
				}
			}
		}
	}

	for (size_t conj1 = 0; conj1 < from.size(); conj1++)
	{
		if (from[conj1] == (pre ^ 1u))
		{
			edgeEmbed(pre ^ 1u, to[conj1], conj1, Dag1, Dag2);
		}
	}

	for (size_t conj2 = 0; conj2 < from.size(); conj2++)
	{
		if (to[conj2] == (post ^ 1u))
		{
			edgeEmbed(from[conj2], post ^ 1u, conj2, Dag1, Dag2);
		}
	}

	
}

uint32_t ** EugComponent::getAdjointMatrix()
{
	size_t size = EugComponent::allNodes.size();
	uint32_t** matrix = new uint32_t*[size];
	for (size_t i = 0; i < size; i++)
	{
		matrix[i] = new uint32_t[size];
		for (size_t j = 0; j < size; j++)
			matrix[i][j] = 0u;
	}

	for (size_t i = 0; i < size; i++)
	{
		if (EugComponent::allNodes[i]->posts[0] != nullptr)
			matrix[EugComponent::allNodes[i]->id][EugComponent::allNodes[i]->posts[0]->id] += 1;
		if (EugComponent::allNodes[i]->posts[1] != nullptr)
			matrix[EugComponent::allNodes[i]->id][EugComponent::allNodes[i]->posts[1]->id] += 1;

	}
	return matrix;
}

void EugComponent::updateUsedAdjointMatrix()
{	
	/*
	cout << "In Component :" << this->layer << endl;
	cout << "Poles: " << endl;
	for (size_t i = 0; i < poleList.size(); i++)
	{
		cout << poleList[i]->id << " ";
	}
	cout << endl;
	*/
	size_t size = EugComponent::allNodes.size();
	
	uint32_t temp = poleList.size() % 2;

	if (poleList.size() > 3)
	{
		for (size_t i = 0; i < this->sn2List.size() - temp; i++)
		{
			bool isPaIn = false;
			bool isPaOut = false;
			if (poleList[sn2List[i]->poleList[0]->poleId]->nextPole != nullptr)
			{
				isPaOut = true;
				//cout << "out " << sn2List[i]->poleList[0]->id << endl;
			}
			for (size_t k = 0; k < poleList.size(); k++)
			{
				if (poleList[k]->nextPole != nullptr&&poleList[k]->nextPole->id == sn2List[i]->poleList[0]->id)
				{
					isPaIn = true;
					//cout << "in " << sn2List[i]->poleList[0]->id <<endl;
					break;
				}
			}
			bool isPbIn = false;
			bool isPbOut = false;
			if (poleList[sn2List[i]->poleList[1]->poleId]->nextPole != nullptr)
			{
				isPbOut = true;
				//cout << "out " << sn2List[i]->poleList[1]->id << endl;
			}
			for (size_t k = 0; k < poleList.size(); k++)
			{
				if (poleList[k]->nextPole != nullptr&&poleList[k]->nextPole->id == sn2List[i]->poleList[1]->id)
				{
					isPbIn = true;
					//cout << "in " << sn2List[i]->poleList[1]->id << endl;
					break;
				}
			}

			if (this->isSn2 == true)
			{
				usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
				usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
				return;
			}

			EugComponent* leftSn2 = this->subCompList[0]->sn2List[i / 2];
			EugComponent* rightSn2 = this->subCompList[1]->sn2List[i / 2];



			switch (sn2List[i]->inStyle)
			{
			case IN_STYLE_0:
				if (isPaIn == true)
				{
					if (i % 2 == 0)
					{
						usedMatrix[leftSn2->poleList[0]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->poleList[0]->id]++;
					}
					else
					{
						usedMatrix[leftSn2->poleList[1]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->poleList[0]->id]++;
					}
				}
				if (isPbIn == true && sn2List[i]->poleStyle != POLE_STYLE_CASCADE)
				{
					if (i % 2 == 0)
					{
						usedMatrix[rightSn2->poleList[0]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
					}
					else
					{
						usedMatrix[rightSn2->poleList[1]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
					}
				}
				break;
			case IN_STYLE_1:
				if (isPaIn == true)
				{
					if (i % 2 == 0)
					{
						usedMatrix[rightSn2->poleList[0]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->poleList[0]->id]++;
					}
					else
					{
						usedMatrix[rightSn2->poleList[1]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->poleList[0]->id]++;
					}
				}

				if (isPbIn == true && sn2List[i]->poleStyle != POLE_STYLE_CASCADE)
				{
					if (i % 2 == 0)
					{
						usedMatrix[leftSn2->poleList[0]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
					}
					else
					{
						usedMatrix[leftSn2->poleList[1]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
					}
				}
				break;
			case IN_STYLE_UNDECIDED:
				break;
			default:
				break;
			}

			switch (sn2List[i]->poleStyle)
			{
			case POLE_STYLE_PARALLEL:
				break;
			case POLE_STYLE_CASCADE:
				usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
				usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
				break;
			case POLE_STYLE_UNDECIDED:
				break;
			default:
				break;
			}

			switch (sn2List[i]->outStyle)
			{
			case OUT_STYLE_0:
				if (isPaOut == true && sn2List[i]->poleStyle != POLE_STYLE_CASCADE)
				{
					if (i % 2 == 0)
					{
						usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][leftSn2->poleList[0]->id]++;
					}
					else
					{
						usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][leftSn2->poleList[1]->id]++;
					}
				}
				if (isPbOut == true)
				{
					if (i % 2 == 0)
					{
						usedMatrix[sn2List[i]->poleList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][rightSn2->poleList[0]->id]++;
					}
					else
					{
						usedMatrix[sn2List[i]->poleList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][rightSn2->poleList[1]->id]++;
					}
				}
				break;
			case OUT_STYLE_1:
				//cout << sn2List[i]->poleList[0]->id << " -> OK " << isPaOut << endl;
				//cout << sn2List[i]->poleList[1]->id << " -> OK " << isPbOut << endl;
				if (isPaOut == true && sn2List[i]->poleStyle != POLE_STYLE_CASCADE)
				{
					if (i % 2 == 0)
					{
						usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][rightSn2->poleList[0]->id]++;
					}
					else
					{
						usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][rightSn2->poleList[1]->id]++;
					}
				}
				if (isPbOut == true)
				{
					if (i % 2 == 0)
					{
						usedMatrix[sn2List[i]->poleList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][leftSn2->poleList[0]->id]++;
					}
					else
					{
						usedMatrix[sn2List[i]->poleList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][leftSn2->poleList[1]->id]++;
					}
				}

				break;
			case OUT_STYLE_UNDECIDED:
				break;
			default:
				break;
			}
		}

		this->subCompList[0]->updateUsedAdjointMatrix();
		this->subCompList[1]->updateUsedAdjointMatrix();
	}
	else //poleList.size() == 3
	{
		for (size_t i = 0; i < this->sn2List.size() - temp; i++)
		{
			bool isPaIn = false;
			bool isPaOut = false;
			if (poleList[sn2List[i]->poleList[0]->poleId]->nextPole != nullptr)
			{
				isPaOut = true;
				//cout << "out " << sn2List[i]->poleList[0]->id << endl;
			}
			for (size_t k = 0; k < poleList.size(); k++)
			{
				if (poleList[k]->nextPole != nullptr&&poleList[k]->nextPole->id == sn2List[i]->poleList[0]->id)
				{
					isPaIn = true;
					//cout << "in " << sn2List[i]->poleList[0]->id <<endl;
					break;
				}
			}
			bool isPbIn = false;
			bool isPbOut = false;
			if (poleList[sn2List[i]->poleList[1]->poleId]->nextPole != nullptr)
			{
				isPbOut = true;
				//cout << "out " << sn2List[i]->poleList[1]->id << endl;
			}
			for (size_t k = 0; k < poleList.size(); k++)
			{
				if (poleList[k]->nextPole != nullptr&&poleList[k]->nextPole->id == sn2List[i]->poleList[1]->id)
				{
					isPbIn = true;
					//cout << "in " << sn2List[i]->poleList[1]->id << endl;
					break;
				}
			}

			if (this->isSn2 == true)
			{
				usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
				usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
				return;
			}

			EugComponent* leftSn2 = this->subCompList[0]->sn2List[i / 2];
			//EugComponent* rightSn2 = this->subCompList[1]->sn2List[i / 2];



			switch (sn2List[i]->inStyle)
			{
			case IN_STYLE_0:
				if (isPaIn == true)
				{
					if (i % 2 == 0)
					{
						usedMatrix[leftSn2->poleList[0]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->poleList[0]->id]++;
					}
					else
					{
						usedMatrix[leftSn2->poleList[1]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->poleList[0]->id]++;
					}
				}
				if (isPbIn == true && sn2List[i]->poleStyle != POLE_STYLE_CASCADE)
				{
					if (i % 2 == 0)
					{
						//usedMatrix[rightSn2->poleList[0]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
					}
					else
					{
						//usedMatrix[rightSn2->poleList[1]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
					}
				}
				break;
			case IN_STYLE_1:
				if (isPaIn == true)
				{
					if (i % 2 == 0)
					{
						//usedMatrix[rightSn2->poleList[0]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->poleList[0]->id]++;
					}
					else
					{
						//usedMatrix[rightSn2->poleList[1]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->poleList[0]->id]++;
					}
				}

				if (isPbIn == true && sn2List[i]->poleStyle != POLE_STYLE_CASCADE)
				{
					if (i % 2 == 0)
					{
						usedMatrix[leftSn2->poleList[0]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
					}
					else
					{
						usedMatrix[leftSn2->poleList[1]->id][sn2List[i]->normalList[0]->id]++;
						usedMatrix[sn2List[i]->normalList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
					}
				}
				break;
			case IN_STYLE_UNDECIDED:
				break;
			default:
				break;
			}

			switch (sn2List[i]->poleStyle)
			{
			case POLE_STYLE_PARALLEL:
				break;
			case POLE_STYLE_CASCADE:
				usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
				usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->poleList[1]->id]++;
				break;
			case POLE_STYLE_UNDECIDED:
				break;
			default:
				break;
			}

			switch (sn2List[i]->outStyle)
			{
			case OUT_STYLE_0:
				if (isPaOut == true && sn2List[i]->poleStyle != POLE_STYLE_CASCADE)
				{
					if (i % 2 == 0)
					{
						usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][leftSn2->poleList[0]->id]++;
					}
					else
					{
						usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][leftSn2->poleList[1]->id]++;
					}
				}
				if (isPbOut == true)
				{
					if (i % 2 == 0)
					{
						usedMatrix[sn2List[i]->poleList[1]->id][sn2List[i]->normalList[2]->id]++;
						//usedMatrix[sn2List[i]->normalList[2]->id][rightSn2->poleList[0]->id]++;
					}
					else
					{
						usedMatrix[sn2List[i]->poleList[1]->id][sn2List[i]->normalList[2]->id]++;
						//usedMatrix[sn2List[i]->normalList[2]->id][rightSn2->poleList[1]->id]++;
					}
				}
				break;
			case OUT_STYLE_1:
				//cout << sn2List[i]->poleList[0]->id << " -> OK " << isPaOut << endl;
				//cout << sn2List[i]->poleList[1]->id << " -> OK " << isPbOut << endl;
				if (isPaOut == true && sn2List[i]->poleStyle != POLE_STYLE_CASCADE)
				{
					if (i % 2 == 0)
					{
						usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->normalList[2]->id]++;
						//usedMatrix[sn2List[i]->normalList[2]->id][rightSn2->poleList[0]->id]++;
					}
					else
					{
						usedMatrix[sn2List[i]->poleList[0]->id][sn2List[i]->normalList[1]->id]++;
						usedMatrix[sn2List[i]->normalList[1]->id][sn2List[i]->normalList[2]->id]++;
						//usedMatrix[sn2List[i]->normalList[2]->id][rightSn2->poleList[1]->id]++;
					}
				}
				if (isPbOut == true)
				{
					if (i % 2 == 0)
					{
						usedMatrix[sn2List[i]->poleList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][leftSn2->poleList[0]->id]++;
					}
					else
					{
						usedMatrix[sn2List[i]->poleList[1]->id][sn2List[i]->normalList[2]->id]++;
						usedMatrix[sn2List[i]->normalList[2]->id][leftSn2->poleList[1]->id]++;
					}
				}

				break;
			case OUT_STYLE_UNDECIDED:
				break;
			default:
				break;
			}
		}

		this->subCompList[0]->updateUsedAdjointMatrix();
		//this->subCompList[1]->updateUsedAdjointMatrix();
	}
}


EugComponent::EugComponent(size_t inNum, size_t outNum, size_t layer, bool isSn2,
	Sn2InStyle sn2InState = Sn2InStyle::IN_STYLE_NONESN2, 
	Sn2PoleStyle sn2PoleState = Sn2PoleStyle::POLE_STYLE_NONESN2, 
	Sn2OutStyle sn2OutState = Sn2OutStyle::OUT_STYLE_NONESN2, 
	uint32_t compId= UINT_MAX)
{
	this->inNum = inNum;
	this->outNum = outNum;
	this->layer = layer;
	this->isSn2 = isSn2;
	this->compId = compId;
	this->inStyle = sn2InState;
	this->poleStyle = sn2PoleState;
	this->outStyle = sn2OutState;

	myDag = nullptr;
}

EugComponent::~EugComponent()
{
	if (this->isSn2)
	{
		for (size_t i = 0; i < inList.size(); i++)
		{
			delete inList[i];
		}
		for (size_t i = 0; i < outList.size(); i++)
		{
			delete outList[i];
		}
		for (size_t i = 0; i < poleList.size(); i++)
		{
			delete poleList[i];
		}
		for (size_t i = 0; i < normalList.size(); i++)
		{
			delete normalList[i];
		}
	}
	else
	{
		delete this->subCompList[0];
		delete this->subCompList[1];
		for (size_t i = 0; i < sn2List.size(); i++)
		{
			delete sn2List[i];
		}
	}
}


EugComponent* EugComponent::getSuperNode1()
{
	EugComponent* SuperNode2 = new EugComponent(1, 1, 1, true, Sn2InStyle::IN_STYLE_UNDECIDED, Sn2PoleStyle::POLE_STYLE_UNDECIDED, Sn2OutStyle::OUT_STYLE_UNDECIDED, getCompNextId());
	//cout << "New SuperNode2 with id: " << compId << endl;

	EugNode* in0 = EugNode::getInNode(0, SuperNode2);
	EugNode* out0 = EugNode::getOutNode(0, SuperNode2);
	EugNode* pole0 = EugNode::getPoleNode(SuperNode2, 0u);

	//cout << "normal0->fanOut = " << normal0->fanOut << endl;

	SuperNode2->inList.push_back(in0);
	SuperNode2->outList.push_back(out0);
	SuperNode2->poleList.push_back(pole0);

	//cout << "Linking the nodes..." << endl;
	//cout << "Part " << 1 << endl;
	in0->linkTo(pole0);
	pole0->linkTo(out0);
	//cout << "Part " << 2 << endl;
	//cout << "normal0->fanOut = " << normal0->fanOut << endl;

	SuperNode2->sn2List.push_back(SuperNode2);

	return SuperNode2;
}


EugComponent* EugComponent::getSuperNode2()
{
	EugComponent* SuperNode2 = new EugComponent(2, 2, 1, true, Sn2InStyle::IN_STYLE_UNDECIDED, Sn2PoleStyle::POLE_STYLE_UNDECIDED, Sn2OutStyle::OUT_STYLE_UNDECIDED, getCompNextId());
	//cout << "New SuperNode2 with id: " << compId << endl;
	
	EugNode* in0 = EugNode::getInNode(0, SuperNode2);
	EugNode* in1 = EugNode::getInNode(1, SuperNode2);
	EugNode* out0 = EugNode::getOutNode(0, SuperNode2);
	EugNode* out1 = EugNode::getOutNode(1, SuperNode2);
	EugNode* pole0 = EugNode::getPoleNode(SuperNode2, 0u);
	EugNode* pole1 = EugNode::getPoleNode(SuperNode2, 1u);
	EugNode* normal0 = EugNode::getNormalNode(SuperNode2);
	EugNode* normal1 = EugNode::getNormalNode(SuperNode2);
	EugNode* normal2 = EugNode::getNormalNode(SuperNode2);

	//cout << "normal0->fanOut = " << normal0->fanOut << endl;

	SuperNode2->inList.push_back(in0);
	SuperNode2->inList.push_back(in1);
	SuperNode2->outList.push_back(out0);
	SuperNode2->outList.push_back(out1);
	SuperNode2->poleList.push_back(pole0);
	SuperNode2->poleList.push_back(pole1);
	SuperNode2->normalList.push_back(normal0);
	SuperNode2->normalList.push_back(normal1);
	SuperNode2->normalList.push_back(normal2); 

	//cout << "Linking the nodes..." << endl;
	//cout << "Part " << 1 << endl;
	in0->linkTo(normal0);
	in1->linkTo(normal0);
	//cout << "Part " << 2 << endl;
	//cout << "normal0->fanOut = " << normal0->fanOut << endl;
	normal0->linkTo(pole0);
	
	//cout << "Part " << 3 << endl;
	pole0->linkTo(normal1);
	normal0->linkTo(normal1);
	normal1->linkTo(pole1);
	normal1->linkTo(normal2);
	pole1->linkTo(normal2);
	normal2->linkTo(out0);
	normal2->linkTo(out1);

	SuperNode2->sn2List.push_back(SuperNode2);

	return SuperNode2;
}

EugComponent * EugComponent::getEug1(uint32_t N)
{
	EugComponent::maxLayer = N+1;
	return getEug1_2N(N);
}


EugComponent * EugComponent::getEug1_2N(uint32_t N)
{
#ifdef DEBUG
	//for (uint32_t i = N; i < maxLayer-1; i++) cout << "|  ";
	//cout << "Construct Eug with Node: " << pow(2, N) << endl;
#endif // DEBUG
	if (N == 1)
	{
#ifdef DEBUG
		//for (uint32_t i = N; i < maxLayer-1; i++) cout << "|  ";
#endif // DEBUG
		
		EugComponent* SN2 = getSuperNode2();
#ifdef DEBUG
		//cout << "End Constructed Eug with Node: " << pow(2, N) << endl;
#endif // DEBUG
		return SN2;
	}

	uint32_t m = uint32_t(pow(2, N-1));
	uint32_t n = m * 2;


	

	EugComponent* Eug1_2N = new EugComponent(n, n, N, false);
	
	// get 2 sub Eug1_2N with 1 layer less
	//for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
	//cout << "Constructing the subEugs of Eug1_2N with N = " << N << endl;
	EugComponent* EugM_0 = getEug1_2N(N - 1);
	EugComponent* EugM_1 = getEug1_2N(N - 1);
	Eug1_2N->subCompList.push_back(EugM_0);
	Eug1_2N->subCompList.push_back(EugM_1);


	// Construct inList and outList of Eug1_2N
	//for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
	//cout << "Constructing the inList and outList of Eug1_2N with N = " << N << endl;
	for (uint32_t i = 0; i < m; i++)
	{
		Eug1_2N->inList.push_back(EugM_0->inList[i]);
		Eug1_2N->inList.push_back(EugM_1->inList[i]);

		Eug1_2N->outList.push_back(EugM_0->outList[i]);
		Eug1_2N->outList.push_back(EugM_1->outList[i]);
	}

	// Initialize new Super Nodes
	EugComponent** SN2s = new EugComponent*[m];
	for (uint32_t i = 0; i < m; i++) 
	{
		SN2s[i] = getSuperNode2();
		Eug1_2N->sn2List.push_back(SN2s[i]);
		SN2s[i]->poleList[0]->poleId = 2 * i;
		SN2s[i]->poleList[1]->poleId = 2 * i + 1;
		//Construct the poleList of SuperNode2N
		Eug1_2N->poleList.push_back(SN2s[i]->poleList[0]);
		Eug1_2N->poleList.push_back(SN2s[i]->poleList[1]);
	}

	//Link the Nodes	
	//for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
	//cout << "Linking Nodes..." << endl;
	for (uint32_t i = 0; i < m; i++)
	{
		EugNode* to = SN2s[i]->inList[0]->posts[0];
		SN2s[i]->inList[0]->unlink();
		SN2s[i]->inList[1]->unlink();

		EugM_0->poleList[i]->linkTo(to);
		EugM_1->poleList[i]->linkTo(to);

		EugNode* p = SN2s[i]->outList[0]->pres[0];
			
		p->unlink();
		p->unlink();
		p->linkTo(EugM_0->poleList[i]);
		p->linkTo(EugM_1->poleList[i]);
	}
		
	//for (uint32_t i = N; i < maxLayer-1; i++) cout << "|  ";
	//cout << "End Constructed Eug with Node: " << pow(2, N) << endl;
	return Eug1_2N;
	
}


EugComponent * EugComponent::getEug_m(uint32_t m)
{
	uint32_t N = uint32_t(log(m-1) / log(2))+1;
	EugComponent::maxLayer = N + 1;
	return getEug1_m(m);
}


EugComponent * EugComponent::getEug1_m(uint32_t n)
{
	if (n == 2) 
	{
		EugComponent* SN2 = getSuperNode2();
		return SN2;
	}

	uint32_t N = uint32_t(log(n-1) / log(2))+1;


	if (n == 3) 
	{
		EugComponent* Eug1_2N = new EugComponent(n, n, N, false);
		//get 2 sub Eug1_2N with 1 layer less
	  //  for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
	   // cout << "Constructing the subEugs of Eug1_2N with N = " << N << endl;

		EugComponent* EugM_0 = getSuperNode2();
		Eug1_2N->subCompList.push_back(EugM_0);


		// Construct inList and outList of Eug1_2N
	    //for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
	  //  cout << "Constructing the inList and outList of Eug1_2N with N = " << N << endl;
		
		for (uint32_t i = 0; i < 2; i++)
		{
			Eug1_2N->inList.push_back(EugM_0->inList[i]);
			Eug1_2N->outList.push_back(EugM_0->outList[i]);
		}



		uint32_t m = uint32_t(n / 2);
		// Initialize new Super Nodes
		EugComponent** SN2s = new EugComponent*[m+1];
		for (uint32_t i = 0; i < m; i++)
		{
			SN2s[i] = getSuperNode2();
			
			Eug1_2N->inList.push_back(SN2s[i]->inList[1]);
			Eug1_2N->outList.push_back(SN2s[i]->outList[1]);

			Eug1_2N->sn2List.push_back(SN2s[i]);
			SN2s[i]->poleList[0]->poleId = 2 * i;
			SN2s[i]->poleList[1]->poleId = 2 * i + 1;
			//Construct the poleList of SuperNode2N
			Eug1_2N->poleList.push_back(SN2s[i]->poleList[0]);
			Eug1_2N->poleList.push_back(SN2s[i]->poleList[1]);
		}
		//cout << "n " << n << endl;
		//cout << "Eug1_2N->poleList.size()"<<  Eug1_2N->poleList.size() << endl;

		SN2s[m] = getSuperNode1();
		Eug1_2N->sn2List.push_back(SN2s[m]);
		SN2s[m]->poleList[0]->poleId = 2 * m;
		//Construct the poleList of SuperNode2N
		Eug1_2N->poleList.push_back(SN2s[m]->poleList[0]);

		//Link the Nodes	
		//for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
		//cout << "Linking Nodes..." << endl;
		for (uint32_t i = 0; i < m; i++)
		{
			EugNode* to = SN2s[i]->inList[0]->posts[0];
			SN2s[i]->inList[0]->unlink();
			SN2s[i]->inList[1]->unlink();

			EugM_0->poleList[i]->linkTo(to);

			EugNode* p = SN2s[i]->outList[0]->pres[0];

			p->unlink();
			p->unlink();
			p->linkTo(EugM_0->poleList[i]);
		}

		EugNode* to = SN2s[m]->inList[0]->posts[0];
		SN2s[m]->inList[0]->unlink();

		EugM_0->poleList[m]->linkTo(to);

		EugNode* p = SN2s[m]->outList[0]->pres[0];

		p->unlink();
		p->unlink();
		p->linkTo(EugM_0->poleList[m]);
		return Eug1_2N;
	}

	if (n % 2 == 0) 
	{
		EugComponent* Eug1_2N = new EugComponent(n, n, N, false);

		// get 2 sub Eug1_2N with 1 layer less
		//for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
		//cout << "Constructing the subEugs of Eug1_2N with N = " << N << endl;
		EugComponent* EugM_0 = getEug1_m(n / 2);
		EugComponent* EugM_1 = getEug1_m(n / 2);
		Eug1_2N->subCompList.push_back(EugM_0);
		Eug1_2N->subCompList.push_back(EugM_1);

		uint32_t m = uint32_t(n / 2);
		// Construct inList and outList of Eug1_2N
		//for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
		//cout << "Constructing the inList and outList of Eug1_2N with N = " << N << endl;
		for (uint32_t i = 0; i < m; i++)
		{
			Eug1_2N->inList.push_back(EugM_0->inList[i]);
			Eug1_2N->inList.push_back(EugM_1->inList[i]);

			Eug1_2N->outList.push_back(EugM_0->outList[i]);
			Eug1_2N->outList.push_back(EugM_1->outList[i]);
		}

		// Initialize new Super Nodes
		EugComponent** SN2s = new EugComponent*[m];
		for (uint32_t i = 0; i < m; i++)
		{
			SN2s[i] = getSuperNode2();
			Eug1_2N->sn2List.push_back(SN2s[i]);
			SN2s[i]->poleList[0]->poleId = 2 * i;
			SN2s[i]->poleList[1]->poleId = 2 * i + 1;
			//Construct the poleList of SuperNode2N
			Eug1_2N->poleList.push_back(SN2s[i]->poleList[0]);
			Eug1_2N->poleList.push_back(SN2s[i]->poleList[1]);
		}

		//Link the Nodes	
		//for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
		//cout << "Linking Nodes..." << endl;
		for (uint32_t i = 0; i < m; i++)
		{
			EugNode* to = SN2s[i]->inList[0]->posts[0];
			SN2s[i]->inList[0]->unlink();
			SN2s[i]->inList[1]->unlink();

			EugM_0->poleList[i]->linkTo(to);
			EugM_1->poleList[i]->linkTo(to);

			EugNode* p = SN2s[i]->outList[0]->pres[0];

			p->unlink();
			p->unlink();
			p->linkTo(EugM_0->poleList[i]);
			p->linkTo(EugM_1->poleList[i]);
		}

		//for (uint32_t i = N; i < maxLayer-1; i++) cout << "|  ";
		//cout << "End Constructed Eug with Node: " << pow(2, N) << endl;
		return Eug1_2N;
	}

	// n % 2 == 1
	EugComponent* Eug1_2N = new EugComponent(n, n, N, false);

	// get 2 sub Eug1_2N with 1 layer less
	//for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
	//cout << "Constructing the subEugs of Eug1_2N with N = " << N << endl;
	EugComponent* EugM_0 = getEug1_m((n + 1) / 2);
	EugComponent* EugM_1 = getEug1_m((n - 1) / 2);

	Eug1_2N->subCompList.push_back(EugM_0);
	Eug1_2N->subCompList.push_back(EugM_1);

	uint32_t m = uint32_t(n / 2);
	// Construct inList and outList of Eug1_2N
	//for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
	//cout << "Constructing the inList and outList of Eug1_2N with N = " << N << endl;
	for (uint32_t i = 0; i < m; i++)
	{
		Eug1_2N->inList.push_back(EugM_1->inList[i]);
		Eug1_2N->outList.push_back(EugM_1->outList[i]);
	}


	for (uint32_t i = 0; i < m+1; i++)
	{
		Eug1_2N->inList.push_back(EugM_0->inList[i]);
		Eug1_2N->outList.push_back(EugM_0->outList[i]);
	}

	// Initialize new Super Nodes
	EugComponent** SN2s = new EugComponent*[m+1];
	for (uint32_t i = 0; i < m; i++)
	{
		SN2s[i] = getSuperNode2();
		Eug1_2N->sn2List.push_back(SN2s[i]);
		SN2s[i]->poleList[0]->poleId = 2 * i;
		SN2s[i]->poleList[1]->poleId = 2 * i + 1;
		//Construct the poleList of SuperNode2N
		Eug1_2N->poleList.push_back(SN2s[i]->poleList[0]);
		Eug1_2N->poleList.push_back(SN2s[i]->poleList[1]);
	}

	SN2s[m] = getSuperNode1();
	Eug1_2N->sn2List.push_back(SN2s[m]);
	SN2s[m]->poleList[0]->poleId = 2 * m;
	//Construct the poleList of SuperNode2N
	Eug1_2N->poleList.push_back(SN2s[m]->poleList[0]);



	//Link the Nodes	
	//for (uint32_t i = N; i < maxLayer; i++) cout << "|  ";
	//cout << "Linking Nodes..." << endl;
	for (uint32_t i = 0; i < m; i++)
	{
		EugNode* to = SN2s[i]->inList[0]->posts[0];
		SN2s[i]->inList[0]->unlink();
		SN2s[i]->inList[1]->unlink();

		EugM_0->poleList[i]->linkTo(to);
		EugM_1->poleList[i]->linkTo(to);

		EugNode* p = SN2s[i]->outList[0]->pres[0];

		p->unlink();
		p->unlink();
		p->linkTo(EugM_0->poleList[i]);
		p->linkTo(EugM_1->poleList[i]);
	}

	EugNode* to = SN2s[m]->inList[0]->posts[0];
	SN2s[m]->inList[0]->unlink();

	EugM_0->poleList[m]->linkTo(to);

	EugNode* p = SN2s[m]->outList[0]->pres[0];

	p->unlink();
	p->unlink();
	p->linkTo(EugM_0->poleList[m]);

	//for (uint32_t i = N; i < maxLayer-1; i++) cout << "|  ";
	//cout << "End Constructed Eug with Node: " << pow(2, N) << endl;
	
	//uint32_t** adjMatrix = Eug1_2N->getAdjointMatrix();
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
*/
	return Eug1_2N;
}

#include "Node.h"

Node::Node(){

}

Node::Node(const unsigned short & p_maxDivisions, const unsigned short & p_maxObjects) :
	m_maxDivisions(p_maxDivisions),
	m_maxObjects(p_maxObjects)
{
	
}

Node::~Node(){
	for (int i = 0; i < 4; i++)
	{
		if (ptr_children[i] != nullptr)
		{
			delete ptr_children[i];
		}
	}
}
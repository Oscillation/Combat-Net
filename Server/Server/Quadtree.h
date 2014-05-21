#pragma once
#include "Node.h"

class Quadtree{
public:
	Quadtree();
	~Quadtree();

	Node* ptr_root;

private:
	///<summary>The amount of objects a node can hold before it needs to split into four cild-nodes.</summary>
	static const unsigned short m_maxObjects = 10;

	///<summary>Represents the maximum amount of divisions. 0 is the root node.</summary>
	static const unsigned short m_maxDivisions = 5;
};
#pragma once
#include "Node.h"
#include "..\..\Common\Map.h"

class Quadtree{
public:
	Quadtree();
	Quadtree(const Map & p_map);
	~Quadtree();

	Node m_root;

	void clean();

private:
	///<summary>Represents the amount of objects that a node can hold before it needs to split into four child-nodes.</summary>
	static const unsigned short m_maxObjects = 3;

	///<summary>Represents the maximum amount of divisions. 0 is the root node.</summary>
	static const unsigned short m_maxDivisions = 5;
};
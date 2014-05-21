#include "Quadtree.h"
#include <iostream>

Quadtree::Quadtree(){
	ptr_root = new Node(m_maxDivisions, m_maxObjects);
}

Quadtree::~Quadtree(){
	delete ptr_root;
}
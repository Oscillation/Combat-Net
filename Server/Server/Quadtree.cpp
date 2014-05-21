#include "Quadtree.h"
#include <iostream>

Quadtree::Quadtree(){

}

Quadtree::Quadtree(const Map & p_map){
	ptr_root = new Node(m_maxDivisions, m_maxObjects, sf::Rect<int>(0, 0, p_map.m_tiles.size()*64, p_map.m_tiles.back().size()*64));
}

Quadtree::~Quadtree(){
	delete ptr_root;
}
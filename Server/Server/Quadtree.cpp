#include "Quadtree.h"
#include <iostream>

Quadtree::Quadtree(){

}

Quadtree::Quadtree(const Map & p_map){
	m_root = Node(0, m_maxDivisions, m_maxObjects, sf::Rect<int>(0, 0, p_map.m_tiles.size()*64, p_map.m_tiles.back().size()*64));
}

Quadtree::~Quadtree(){
	clean();
}

void Quadtree::clean(){
	m_root.clean();
}
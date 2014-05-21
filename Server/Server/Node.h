#pragma once
#include "Object.h"
#include <vector>

struct Node{
	Node();
	Node(const unsigned short & p_divisions, const unsigned short & p_maxDivisions, const unsigned short & p_maxObjects, const sf::Rect<int> p_bounds);
	~Node();

	Node* ptr_children[4];

	void insert(const Object & p_object);

	sf::Rect<int> getBounds() const;

	void getObjects(std::vector<Object> & p_objects, const Object & p_object);

	void clean();

private:
	unsigned short m_divisions;

	unsigned short m_maxDivisions;
	unsigned short m_maxObjects;

	sf::Rect<int> m_bounds;

	std::vector<Object> m_objects;

	void split();
	
	int getIndex(const Object & p_object) const;
};
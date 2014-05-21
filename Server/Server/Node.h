#pragma once

struct Node{
	Node();
	Node(const unsigned short & p_maxDivisions, const unsigned short & p_maxObjects);
	~Node();

	Node* ptr_children[4];

private:
	unsigned short m_level;

	unsigned short m_maxDivisions;
	unsigned short m_maxObjects;
};
#include "Node.h"
#include <iostream>

Node::Node(){

}

Node::Node(const unsigned short & p_divisions, const unsigned short & p_maxDivisions, const unsigned short & p_maxObjects, const sf::Rect<int> p_bounds) :
	m_divisions(p_divisions),
	m_maxDivisions(p_maxDivisions),
	m_maxObjects(p_maxObjects),
	m_bounds(p_bounds)
{
	ptr_children[0] = nullptr;
	ptr_children[1] = nullptr;
	ptr_children[2] = nullptr;
	ptr_children[3] = nullptr;
}

Node::~Node(){
	clean();
}

void Node::insert(const Object & p_object){
	if (ptr_children[0] != nullptr)
	{
		int index = getIndex(p_object);

		if (index != -1)
		{
			ptr_children[index]->insert(p_object);

			return;
		}
	}

	m_objects.push_back(p_object);

	if (m_objects.size() > m_maxObjects && m_divisions < m_maxDivisions)
	{
		if (ptr_children[0] == nullptr)
		{
			split();
		}

		int i = 0;
		while (i < m_objects.size())
		{
			int index = getIndex(m_objects[i]);
			if (index != -1)
			{
				ptr_children[index]->insert(m_objects[i]);
				m_objects.erase(m_objects.begin() + i);
			}else
			{
				i++;
			}
		}
	}
}

sf::Rect<int> Node::getBounds() const{
	return m_bounds;
}

void Node::split(){
	ptr_children[0] = new Node(m_divisions + 1, m_maxDivisions, m_maxObjects, sf::Rect<int>(m_bounds.left, m_bounds.top, m_bounds.width/2, m_bounds.height/2));
	ptr_children[0]->m_objects.resize(0, Object());
	ptr_children[1] = new Node(m_divisions + 1, m_maxDivisions, m_maxObjects, sf::Rect<int>(m_bounds.left + m_bounds.width/2, m_bounds.top, m_bounds.width/2, m_bounds.height/2));
	ptr_children[1]->m_objects.resize(0, Object());
	ptr_children[2] = new Node(m_divisions + 1, m_maxDivisions, m_maxObjects, sf::Rect<int>(m_bounds.left, m_bounds.top + m_bounds.height/2, m_bounds.width/2, m_bounds.height/2));
	ptr_children[2]->m_objects.resize(0, Object());
	ptr_children[3] = new Node(m_divisions + 1, m_maxDivisions, m_maxObjects, sf::Rect<int>(m_bounds.left + m_bounds.width/2, m_bounds.top + m_bounds.height/2, m_bounds.width/2, m_bounds.height/2));
	ptr_children[3]->m_objects.resize(0, Object());
}

int Node::getIndex(const Object & p_object) const{
	int index = -1;

	int verticalMidpoint = m_bounds.left + (m_bounds.width/2);
	int horizontalMidpoint = m_bounds.top + (m_bounds.height/2);

	//The object can fit within the top quadrants
	bool topQuadrant = (p_object.getBounds().top < horizontalMidpoint && p_object.getBounds().top + p_object.getBounds().height < horizontalMidpoint);

	//The object can fit within the bottom quadrants
	bool bottomQuadrant = (p_object.getBounds().top > horizontalMidpoint && p_object.getBounds().top + p_object.getBounds().height < m_bounds.top + m_bounds.height);

	//The object can fit within the left quadrants
	if (p_object.getBounds().left < verticalMidpoint && p_object.getBounds().left + p_object.getBounds().width < verticalMidpoint)
	{
		if (topQuadrant)
		{
			index = 0;
		} else if (bottomQuadrant)
		{
			index = 2;
		}
	}

	//The object can fit within the right quadrants
	else if (p_object.getBounds().left > verticalMidpoint && p_object.getBounds().left + p_object.getBounds().width < m_bounds.left + m_bounds.width)
	{
		if (topQuadrant)
		{
			index = 1;
		} else if (bottomQuadrant)
		{
			index = 3;
		}
	}

	return index;
}

void Node::getObjects(std::vector<Object> & p_objects, const Object & p_object){
	int index = getIndex(p_object);
	if (index != -1 && ptr_children[0] != nullptr) {
		ptr_children[index]->getObjects(p_objects, p_object);
	}

	p_objects.insert(p_objects.begin() + p_objects.size(), m_objects.begin(), m_objects.end());
}

void Node::clean(){
	if (!m_objects.empty())
	{
		m_objects.clear();
	}
	for (int i = 0; i < 4; i++)
	{
		if (ptr_children[i] != nullptr)
		{
			ptr_children[i]->clean();
			ptr_children[i] = nullptr;
		}else{
			return;
		}
	}
}
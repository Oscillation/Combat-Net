#include "Object.h"

Object::Object()
{

}

Object::Object(const ObjectType::ObjectType & p_objectType, const sf::Rect<float> & p_bounds, const unsigned int & p_id) : 
	m_objectType(p_objectType),
	m_bounds(p_bounds),
	m_id(p_id)
{

}

Object::Object(const ObjectType::ObjectType & p_objectType, const sf::Rect<float> & p_bounds, const std::string & p_name) : 
	m_objectType(p_objectType),
	m_bounds(p_bounds),
	m_name(p_name)
{

}

Object::~Object(){

}

ObjectType::ObjectType Object::getObjectType() const{
	return m_objectType;
}

sf::Rect<float> Object::getBounds() const{
	return m_bounds;
}

unsigned int Object::getId() const{
	return m_id;
}

std::string Object::getName() const{
	return m_name;
}
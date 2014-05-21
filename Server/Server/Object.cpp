#include "Object.h"

Object::Object()
{

}

Object::Object(const ObjectType & p_objectType, const sf::Rect<float> & p_bounds) : 
	m_objectType(p_objectType),
	m_bounds(p_bounds)
{

}

Object::~Object(){

}

ObjectType Object::getObjectType() const{
	return m_objectType;
}

sf::Rect<float> Object::getBounds() const{
	return m_bounds;
}
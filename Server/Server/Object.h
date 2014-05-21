#pragma once
#include <SFML\Graphics\Rect.hpp>

enum ObjectType{
	Player,
	Projectile,
	Power,
};

struct Object{
	Object();
	Object(const ObjectType & p_objectType, const sf::Rect<float> & p_bounds);
	~Object();

	ObjectType getObjectType() const;
	sf::Rect<float> getBounds() const;
	
private:
	ObjectType m_objectType;
	sf::Rect<float> m_bounds;
};
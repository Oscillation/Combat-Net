#pragma once
#include <SFML\Graphics\Rect.hpp>

namespace ObjectType{
	enum ObjectType{
		Player,
		Projectile,
		Power,
	};
}

struct Object{
	Object();
	Object(const ObjectType::ObjectType & p_objectType, const sf::Rect<float> & p_bounds);
	~Object();

	ObjectType::ObjectType getObjectType() const;
	sf::Rect<float> getBounds() const;

private:
	ObjectType::ObjectType m_objectType;
	sf::Rect<float> m_bounds;
};
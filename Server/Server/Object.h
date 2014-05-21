#pragma once
#include <SFML\Graphics\Rect.hpp>
#include <string>

namespace ObjectType{
	enum ObjectType{
		Player,
		Projectile,
		Power,
	};
}

struct Object{
	Object();
	Object(const ObjectType::ObjectType & p_objectType, const sf::Rect<float> & p_bounds, const unsigned int & p_id);
	Object(const ObjectType::ObjectType & p_objectType, const sf::Rect<float> & p_bounds, const std::string & p_name);
	~Object();

	ObjectType::ObjectType getObjectType() const;
	sf::Rect<float> getBounds() const;

	///<summary>The id of either a power or a projectile.</summary>
	unsigned int getId() const;

	///<summary>The name of a player.</summary>
	std::string getName() const;

private:
	ObjectType::ObjectType m_objectType;

	sf::Rect<float> m_bounds;

	unsigned int m_id;
	std::string m_name;
};
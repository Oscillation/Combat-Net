#pragma once
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\Rect.hpp>

struct Circle : public sf::CircleShape{
	Circle();
	Circle(const sf::CircleShape & p_circle);

	template <typename T>
	bool intersects(const sf::Rect<T> & p_rect){
		sf::Vector2<float> distance;
		distance.x = std::abs(getPosition().x - p_rect.left);
		distance.y = std::abs(getPosition().y - p_rect.top);

		if (distance.x > (p_rect.width/2 + getRadius()))
		{
			return false;
		}
		if (distance.y > (p_rect.height/2 + getRadius()))
		{
			return false;
		}

		if (distance.x <= (p_rect.width/2))
		{
			return true;
		} 
		if (distance.y <= (p_rect.height/2))
		{
			return true;
		}

		float cornerDistance = std::pow((distance.x - p_rect.width/2), 2) + std::pow((distance.y - p_rect.height/2), 2);

		return (cornerDistance <= std::pow(getRadius(), 2));
	}

};
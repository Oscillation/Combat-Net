#pragma once
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Window\Mouse.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\RenderWindow.hpp>

class Resizable : public sf::Drawable
{
public:
	Resizable();
	Resizable(sf::Rect<int>* ptr_bounds);
	~Resizable();

	void update(const sf::RenderWindow & p_window);

	sf::Rect<int>*  resizable_ptr_bounds;

	void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;

private:
	sf::Vector2<int> m_position;

	bool pressed, intersecting, move, resize;

	sf::Rect<int> m_sides[4];

	void setSides();
};

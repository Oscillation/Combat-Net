#pragma once

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Font.hpp>

#include <SFML\System\String.hpp>

class Player : public sf::Drawable, public sf::Transformable
{
public:

	Player(sf::String p_name, sf::Font& p_font, bool p_remote = true);
	~Player();
	bool isRemote() const;

	float getRadius() const;

private:
	bool m_remote;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Text nameText;

	float m_radius;
};
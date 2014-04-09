#pragma once

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>

class Player : public sf::Drawable, public sf::Transformable
{
public:

	Player(bool p_remote = true);
	~Player();
	bool isRemote() const;

private:
	bool m_remote;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
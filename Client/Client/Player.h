#pragma once
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

#include <SFML\System\String.hpp>
#include <SFML\System\Time.hpp>

#include "..\..\Common\Protocol.h"
#include "..\..\Common\Score.h"
#include "..\..\Server\Server\Object.h"

class Player : public sf::Drawable, public sf::Transformable
{
public:

	Player(std::string p_name, sf::Font& p_font, bool p_remote = true);
	~Player();

	void update(sf::Time p_deltaTime, int p_elapsedGameTime);

	bool isRemote() const, isDead() const;

	float getRadius() const;

	void setTargetTime(int p_targetTime);
	void setTargetPosition(sf::Vector2f p_targetPos);

	int getHealth() const;
	void setHealth(const int & p_health);

	unsigned short m_team;

	Score m_score;

	operator Object() const;

private: 
	bool m_remote, m_dead;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Text nameText;

	float m_radius;

	int prevTime, targetTime;
	sf::Vector2f prevPos, targetPos;

	int m_health;
};
#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Network\Packet.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\System\String.hpp>
#include <SFML\System\Time.hpp>
#include <vector>

#include "Tile.h"
#include "GeneralMath.h"

struct Projectile : public sf::Transformable, public sf::Drawable{
	Projectile();
	Projectile(const int & p_id);
	~Projectile();

	int m_id;

	void update(sf::Time p_deltaTime, int p_elapsedGameTime);

	sf::Vector2<float> getVelocity() const;
	void setVelocity(const sf::Vector2<float> & p_velocity);

	sf::String getName() const;
	void setName(const sf::String & p_name);

	void setTargetTime(int p_targetTime);
	void setTargetPosition(sf::Vector2f p_targetPos);

	bool erase;

private:
	sf::Vector2<float> m_velocity;
	sf::String m_name;
	virtual void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;

	int prevTime, targetTime;
	sf::Vector2f prevPos, targetPos;
};
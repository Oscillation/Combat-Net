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
#include "..\Server\Server\Object.h"

struct Projectile : public sf::Transformable, public sf::Drawable{
	Projectile();
	Projectile(const int & p_id, const int& p_damage, const sf::Vector2<float> & p_position);
	~Projectile();

	int m_id;
	int m_damage;

	void update(sf::Time p_deltaTime, int p_elapsedGameTime);

	sf::Vector2<float> getVelocity() const;
	void setVelocity(const sf::Vector2<float> & p_velocity);

	std::string getName() const;
	void setName(const std::string & p_name);

	void setTargetTime(int p_targetTime);
	void setTargetPosition(sf::Vector2f p_targetPos);

	sf::RectangleShape m_rectangleShape;

	bool erase;

	///<summary>In order to remove any projectiles that may have been lost in outdated packets.</summary>
	bool m_updated;

	bool operator==(Projectile & p_projectile) const;

	sf::Vector2<float> m_targetPos;

	operator Object() const;

private:
	sf::Vector2<float> m_velocity;
	std::string m_name;
	virtual void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;

	int m_prevTime, m_targetTime;
	sf::Vector2f m_prevPos;
};

template <typename T>
sf::Packet& operator>>(sf::Packet & p_packet, sf::Vector2<T> & p_vec);

template <typename T>
sf::Packet& operator<<(sf::Packet & p_packet, const sf::Vector2<T> & p_vec);

sf::Packet& operator>>(sf::Packet & p_packet, Projectile & p_projectile);

sf::Packet& operator<<(sf::Packet & p_packet, Projectile & p_projectile);

sf::Packet& operator>>(sf::Packet & p_packet, std::vector<Projectile> & p_projectiles);

sf::Packet& operator<<(sf::Packet & p_packet, std::vector<Projectile> & p_projectiles);
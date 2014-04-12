#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Network\Packet.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

struct Projectile : public sf::Transformable, public sf::Drawable{
	Projectile();
	~Projectile();

	sf::Vector2<float> getVelocity() const;
	void setVelocity(const sf::Vector2<float> & p_velocity);

private:
	sf::Vector2<float> m_velocity;

	virtual void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;
};

sf::Packet& operator <<(sf::Packet& packet, const Projectile& projectile);
sf::Packet& operator >>(sf::Packet& packet, Projectile& projectile);
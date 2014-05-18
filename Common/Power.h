#pragma once
#include <functional>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

#include "..\Server\Server\Client.h"
#include "..\Server\Server\PowerTile.h"

struct Power : public sf::Drawable, public sf::Transformable{
	Power(const std::function<void(Client & p_client)> & p_power);
	Power();
	~Power();

	void activate(Client & p_client);

	std::function<void(Client & p_client)> m_power;

	PowerTile* ptr_tile;

private:
	void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;
};

sf::Packet& operator <<(sf::Packet& packet, const Power & p_power);
sf::Packet& operator >>(sf::Packet& packet, Power & p_power);
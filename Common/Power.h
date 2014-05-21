#pragma once
#include <functional>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>

#include "..\Server\Server\Client.h"
#include "..\Server\Server\PowerTile.h"
#include "..\Server\Server\Object.h"

enum PowerType{
	Health,
	Speed,
};

struct Power : public sf::Drawable, public sf::Transformable{
	Power(const std::function<void(Client & p_client)> & p_power, const PowerType & p_powerType);
	Power();
	~Power();

	void activate(Client & p_client);

	std::function<void(Client & p_client)> m_power;

	PowerTile* ptr_tile;

	PowerType m_powerType;

	bool operator==(Power & p_power) const;

	operator Object() const;

private:
	void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;
};

sf::Packet& operator <<(sf::Packet& packet, const Power & p_power);
sf::Packet& operator >>(sf::Packet& packet, Power & p_power);
sf::Packet& operator <<(sf::Packet& packet, const std::vector<Power> & p_powers);
sf::Packet& operator >>(sf::Packet& packet, std::vector<Power> & p_powers);
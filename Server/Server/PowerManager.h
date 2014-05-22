#pragma once
#include "..\..\Common\Power.h"
#include "PowerTile.h"
#include "Client.h"

#include <iostream>
#include <vector>
#include <SFML\System\Time.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

class PowerManager : public sf::Drawable, public sf::Transformable{
public:
	PowerManager();
	PowerManager(const std::vector<PowerTile> & p_powerTiles);
	~PowerManager();

	sf::Packet & update(const sf::Time & p_deltaTime);

	void activate(Power & p_power, Client & p_client);

	void erase(Power & p_power);

	std::vector<Power> m_powers;

	std::vector<Power>::iterator findId(const unsigned int & p_id);

	std::vector<PowerTile> m_powerTiles;

private:

	void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;

	unsigned int m_id;
};
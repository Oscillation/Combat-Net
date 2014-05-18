#pragma once
#include <SFML\System\Vector2.hpp>
#include <SFML\System\Time.hpp>

#include "..\..\Common\GeneralMath.h"
#include "..\..\Common\Tile.h"

struct PowerTile : public Tile{
	PowerTile(const unsigned short & p_x, const unsigned short & p_y);
	PowerTile();
	~PowerTile();

	bool update(const sf::Time & p_deltaTime), m_hasPower;

	///<summary>Time till the power spawns.</summary>
	float m_time;
};
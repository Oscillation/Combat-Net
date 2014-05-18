#include "PowerTile.h"

PowerTile::PowerTile(const unsigned short & p_x, const unsigned short & p_y) : m_time(math::random(0, 5)), m_hasPower(false) {
	this->m_x = p_x;
	this->m_y = p_y;
	this->m_type = TileType::Type::Power;
}

PowerTile::PowerTile() : m_time(math::random(0, 5)), m_hasPower(false) {
	this->m_x = 0;
	this->m_y = 0;
	this->m_type = TileType::Type::Power;
}

PowerTile::~PowerTile(){

}

bool PowerTile::update(const sf::Time & p_deltaTime){
	if (m_time <= 0)
	{
		return true;
	}else
	{
		m_time -= p_deltaTime.asSeconds();
		if (m_time <= 0)
		{
			return true;
		}
	}

	return false;
}
#include "PowerManager.h"

PowerManager::PowerManager(){

}

PowerManager::PowerManager(const std::vector<PowerTile> & p_powerTiles) : 
	m_powerTiles(p_powerTiles),
	m_id(0)
{

}

PowerManager::~PowerManager(){

}

sf::Packet & PowerManager::update(const sf::Time & p_deltaTime){
	sf::Packet packet;

	for (auto it = m_powerTiles.begin(); it != m_powerTiles.end(); ++it){
		if (!it->m_hasPower)
		{
			it->m_hasPower = it->update(p_deltaTime);
			if (it->m_hasPower)
			{
				int i = math::random(1, 100);

				if (i > 50)
				{
					Power power = Power(([this](Client & p_client) {
						if (p_client.getHealth() + 20 <= 100)
						{
							p_client.setHealth(p_client.getHealth() + 20);
						}else
						{
							p_client.setHealth(100);
						}
					}), PowerType::Health, m_id++);
					m_powers.push_back(power);
					m_powers.back().setPosition(it->m_x*64 + 16, it->m_y*64 + 16);
					m_powers.back().ptr_tile = &(*it);
					packet << cn::Power << power;
				}else
				{
					Power power = Power(([this](Client & p_client) {
						if (p_client.m_speedBoost < 100)
						{
							p_client.m_speedBoost += 10.f;
						}
					}), PowerType::Speed, m_id++);
					m_powers.push_back(power);
					m_powers.back().setPosition(it->m_x*64 + 16, it->m_y*64 + 16);
					m_powers.back().ptr_tile = &(*it);
					packet << cn::Power << power;
				}
			}
		}
	}

	return packet;
}

void PowerManager::activate(Power & p_power, Client & p_client){
	p_power.activate(p_client);
}

void PowerManager::erase(Power & p_power){
	for (int i = 0; i < m_powers.size(); i++)
	{
		if (m_powers[i] == p_power)
		{
			m_powers.erase(m_powers.begin() + i);
			return;
		}
	}
}

void PowerManager::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const{
	for (auto it = m_powers.begin(); it != m_powers.end(); ++it){
		p_states.transform *= it->getTransform();
		p_target.draw(*it, p_states);
	}
}

std::vector<Power>::iterator PowerManager::findId(const unsigned int & p_id){
	for(auto it = m_powers.begin(); it != m_powers.end(); ++it){
		if (it->getId() == p_id)
		{
			return it;
		}
	}
	return m_powers.end();
}
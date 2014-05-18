#include "PowerManager.h"

PowerManager::PowerManager(){

}

PowerManager::PowerManager(const std::vector<PowerTile> & p_powerTiles) : m_powerTiles(p_powerTiles){

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
				Power power = Power(Power([this](Client & p_client) {
					p_client.setHealth(p_client.getHealth() - 10);
				}));
				m_powers.push_back(power);
				m_powers.back().setPosition(it->m_x*64 + 16, it->m_y*64 + 16);
				m_powers.back().ptr_tile = &(*it);
				std::cout << "asdf\n";
				//packet << 0 << cn::Power << m_powers.back();
			}
		}
	}

	return packet;
}

void PowerManager::activate(Power & p_power, Client & p_client){
	p_power.activate(p_client);
}

void PowerManager::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const{
	for (auto it = m_powers.begin(); it != m_powers.end(); ++it){
		p_states.transform *= it->getTransform();
		p_target.draw(*it, p_states);
	}
}
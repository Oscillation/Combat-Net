#include "Power.h"

Power::Power(const std::function<void(Client & p_client)> & p_power, const PowerType & p_powerType) : m_power(p_power), ptr_tile(NULL), m_powerType(p_powerType){

}

Power::Power() : ptr_tile(NULL){

}

Power::~Power(){

}

void Power::activate(Client & p_client){
	if (ptr_tile != NULL)
	{
		
		m_power(p_client);

		ptr_tile->m_time = math::random(0, 5);
		ptr_tile->m_hasPower = false;
	}
}

void Power::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const{
	p_states.transform = getTransform();
	sf::RectangleShape rectShape = sf::RectangleShape(sf::Vector2<float>(32, 32));
	rectShape.setPosition(getPosition());
	rectShape.setFillColor(sf::Color::Magenta);
	p_target.draw(rectShape);
}

sf::Packet& operator <<(sf::Packet& packet, const Power & p_power){
	return packet << p_power.getPosition() << (int)p_power.m_powerType;
}

sf::Packet& operator >>(sf::Packet& packet, Power & p_power){
	sf::Vector2<float> pos;
	int type;
	packet >> pos >> type;
	p_power.setPosition(pos);
	p_power.m_powerType = (PowerType)type;
	return packet;
}

sf::Packet& operator <<(sf::Packet& packet, const std::vector<Power> & p_powers){
	packet << p_powers.size();
	for (int i = 0; i < p_powers.size(); i++)
	{
		packet << p_powers[i];
	}

	return packet;
}

sf::Packet& operator >>(sf::Packet& packet, std::vector<Power> & p_powers){
	int size;
	packet >> size;
	for (int i = 0; i < size; i++)
	{
		Power power = Power();
		packet >> power;
		p_powers.push_back(power);
	}

	return packet;
}
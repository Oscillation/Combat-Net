#include "Power.h"

Power::Power(const std::function<void(Client & p_client)> & p_power) : m_power(p_power), ptr_tile(NULL){

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
	p_target.draw(rectShape, p_states);
}

sf::Packet& operator <<(sf::Packet& packet, const Power & p_power){
	return packet << p_power.getPosition();
}

sf::Packet& operator >>(sf::Packet& packet, Power & p_power){
	sf::Vector2<float> pos;
	packet >> pos;
	p_power.setPosition(pos);
	return packet;
}
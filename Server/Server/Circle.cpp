#include "Circle.h"

Circle::Circle(){
}

Circle::Circle(const sf::CircleShape & p_circle){
	setOrigin(p_circle.getOrigin());
	setPosition(p_circle.getPosition());
	setRadius(p_circle.getRadius());
	setScale(p_circle.getScale());
	setFillColor(p_circle.getFillColor());
	setOutlineColor(p_circle.getOutlineColor());
	setOutlineThickness(p_circle.getOutlineThickness());
	setRotation(p_circle.getRotation());
	setTexture(p_circle.getTexture());
	setTextureRect(p_circle.getTextureRect());
}
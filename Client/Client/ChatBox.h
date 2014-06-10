#pragma once
#include "TextLog.h"
#include "Resizable.h"

class ChatBox : public sf::Drawable
{
public:
	ChatBox(const sf::Font & ptr_font, const sf::Rect<int> & p_bounds = sf::Rect<int>(20, 572, 256, 128));
	~ChatBox();

	void update(const sf::RenderWindow & p_window, const sf::Time & p_deltaTime);

private:
	sf::Rect<int> m_bounds;

	void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;

	Resizable m_resizable;

	TextLog m_textLog;

	float m_timer;
};
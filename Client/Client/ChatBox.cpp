#include "ChatBox.h"

ChatBox::ChatBox(const sf::Font* ptr_font, const sf::Rect<int> & p_bounds) :
	m_bounds(sf::Rect<int>(20, 572, 256, 128)),
	m_resizable(Resizable(&m_bounds))
{

}

ChatBox::~ChatBox()
{
	
}

void ChatBox::update(const sf::RenderWindow & p_window)
{
	m_resizable.update(p_window);// Move the box around and resize it.


	// Accept messages and call push_back from TextLog down here or something...........
}

void ChatBox::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const
{
	sf::RectangleShape rect = sf::RectangleShape(sf::Vector2<float>(m_bounds.width, m_bounds.height));
	rect.setPosition(m_bounds.left, m_bounds.top);
	rect.setFillColor(sf::Color(100, 100, 100, 200));

	p_target.draw(rect);
	p_target.draw(m_textLog);
	p_target.draw(m_resizable);
}
#include "ChatBox.h"

ChatBox::ChatBox(const sf::Font & p_font, const sf::Rect<int> & p_bounds) :
	m_bounds(sf::Rect<int>(20, 572, 256, 128)),
	m_resizable(Resizable(&m_bounds)),
	m_textLog(p_font, &m_bounds),
	m_timer(0.f)
{

}

ChatBox::~ChatBox()
{
	
}

void ChatBox::update(const sf::RenderWindow & p_window, const sf::Time & p_deltaTime)
{
	bool align = m_resizable.update(p_window);// Move the box around and resize it.

	if (align)
	{
		m_textLog.align(m_bounds);
	}

	m_timer += p_deltaTime.asSeconds();

	if (m_timer >= 1)
	{
		
		std::string string = "The quick brown fox jumped over the lazy dog.";
		sf::Text text;
		text.setFont(m_textLog.m_font);
		text.setString(string);
		m_textLog.push_back(text);
		m_timer = 0;
	}

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

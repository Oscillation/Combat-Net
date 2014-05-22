#include "Killfeed.h"

Killfeed::Killfeed(sf::Font & p_font)
{
	for (int i = 0; i < 4; ++i)
	{
		m_feed[i] = sf::Text("", p_font, 16);
		m_feed[i].setPosition(0, i * 20);
	}
}

Killfeed::~Killfeed()
{

}

void Killfeed::clear()
{
	for (int i = 0; i < 4; ++i)
	{
		m_feed[i].setString("");
	}
}

void Killfeed::update(const sf::Time& p_deltaTime)
{
	if (m_time.asSeconds() > 0)
	{
		m_time -= p_deltaTime;
	}
	else if (hasText())
	{
		sf::Color c = m_feed[0].getColor();
		c.a *= 0.9f;
		if (c.a < 5) 
		{
			for (int i = 0; i < 3; ++i)
			{
				m_feed[i].setString(m_feed[i+1].getString());
				m_feed[i].setPosition(-m_feed[i].getLocalBounds().width, i*20);
				m_feed[i].setColor(sf::Color::White);
			}
			m_feed[3].setString("");
			m_time = sf::seconds(20);
		}
		else
		{
			m_feed[0].setColor(c);	
		}
	}
}

void Killfeed::registerKill(const std::string & p_killer, const std::string & p_killed)
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_feed[i].getString() == "") {
			m_feed[i].setString(p_killer + " -> " + p_killed);
			m_feed[i].setPosition(-m_feed[i].getLocalBounds().width, i*20);
			m_feed[i].setColor(sf::Color::White);
			return;
		}
	}

	for (int i = 0; i < 3; ++i)
	{
		m_feed[i].setString(m_feed[i+1].getString());
		m_feed[i].setPosition(-m_feed[i].getLocalBounds().width, i*20);
	}
	m_feed[3].setString(p_killer + " -> " + p_killed);
	m_feed[3].setPosition(-m_feed[3].getLocalBounds().width, 3*20);
	m_feed[3].setColor(sf::Color::White);
	m_time = sf::seconds(20);
}

void Killfeed::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (int i = 0; i < 4; ++i)
	{
		target.draw(m_feed[i], states);
	}
}

bool Killfeed::hasText()
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_feed[i].getString() == "") {
			return true;
		}
	}	
	return false;
}
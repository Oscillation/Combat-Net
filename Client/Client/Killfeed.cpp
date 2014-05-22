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

void Killfeed::registerKill(const std::string & p_killer, const std::string & p_killed)
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_feed[i].getString() == "") {
			m_feed[i].setString(p_killer + " -> " + p_killed);
			m_feed[i].setPosition(-m_feed[i].getLocalBounds().width, i*20);
			return;
		}
	}

	for (int i = 3; i > 0; --i)
	{
		m_feed[i-1].setString(m_feed[i].getString());
		m_feed[i].setPosition(-m_feed[i].getLocalBounds().width, i*20);
	}
	m_feed[3].setString(p_killer + " -> " + p_killed);
	m_feed[3].setPosition(-m_feed[3].getLocalBounds().width, 3*20);
}

void Killfeed::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	for (int i = 0; i < 4; ++i)
	{
		target.draw(m_feed[i], states);
	}
}
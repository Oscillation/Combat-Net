#include "ScoreText.h"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

ScoreText::ScoreText(std::string const& p_name, bool p_special)
{

	m_name.setString(p_name);
	if (!p_special) {
		m_score.setString("0");
		m_kills.setString("0");
		m_deaths.setString("0");
	} else {
		m_score.setString("S");
		m_kills.setString("K");
		m_deaths.setString("D");
	}

	m_score.setPosition(450, 0);
	m_kills.setPosition(480, 0);
	m_deaths.setPosition(510, 0);

	m_name.setCharacterSize(14);
	m_score.setCharacterSize(14);
	m_kills.setCharacterSize(14);
	m_deaths.setCharacterSize(14);
}

ScoreText::~ScoreText()
{
}

void ScoreText::update(Score p_score, std::string p_name)
{
	m_name.setString(p_name);
	m_score.setString(std::to_string(p_score.m_points));
	m_kills.setString(std::to_string(p_score.m_kills));
	m_deaths.setString(std::to_string(p_score.m_deaths));
}

void ScoreText::setFont(sf::Font& p_font)
{
	m_name.setFont(p_font);
	m_score.setFont(p_font);
	m_kills.setFont(p_font);
	m_deaths.setFont(p_font);
}

void ScoreText::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(m_name, states);
	target.draw(m_score, states);
	target.draw(m_kills, states);
	target.draw(m_deaths, states);
}
#include "Scoreboard.h"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

Scoreboard::Scoreboard(std::map<std::string, std::unique_ptr<Player>>& p_players)
	:
	m_players(p_players),
	m_background(sf::Vector2f(600, 0)),
	m_namePanel(sf::Vector2f(560, 20)),
	m_namePanelText("NAME", true)
{
	m_background.setFillColor(sf::Color(128, 128, 128, 128));
	m_background.setOutlineColor(sf::Color::Black);
	m_background.setOutlineThickness(2.f);
	
	m_namePanel.setPosition(20.f, 10.f);
	m_namePanel.setFillColor(sf::Color(64, 64, 64, 128));
	m_namePanel.setOutlineColor(sf::Color::Black);
	m_namePanel.setOutlineThickness(1.f);

	m_namePanelText.setPosition(22.f, 11.f);

	this->setOrigin(m_background.getGlobalBounds().width/2, m_background.getGlobalBounds().height/2);
}

Scoreboard::~Scoreboard()
{
}

void Scoreboard::updateStats()
{
	// insert all players from a map, into a vector of std::pair<name, player>
	std::vector<std::pair<std::string, Player*>> sortedPlayers;
	for (auto i = m_players.begin(); i != m_players.end(); i++)
	{
		sortedPlayers.push_back(std::make_pair(i->first, i->second.get()));
	}
	// Sort that vector depending on the score of the players
	std::sort(sortedPlayers.begin(), sortedPlayers.end(), [=](std::pair<std::string, Player*> & a, std::pair<std::string, Player*> & b) {
		return a.second->m_score.m_points > b.second->m_score.m_points;
	});

	int count = 0;
	for (auto i = sortedPlayers.begin(); i != sortedPlayers.end(); i++, count++)
	{
		if (count >= m_scoreTexts.size())
		{
			m_scoreTexts.push_back(ScoreText(i->first, false));
			m_scoreTexts[count].setFont(*m_font);
		}
		m_scoreTexts[count].update(i->second->m_score, i->first);
		m_scoreTexts[count].setPosition(22.f, 35.f + count * 16.f);
		if (count % 2 == 0)
			m_scoreTexts[count].setBackgroundColor(sf::Color(255, 255, 255, 100));
		else
			m_scoreTexts[count].setBackgroundColor(sf::Color(100, 100, 100, 100));
	}
	m_scoreTexts.erase(m_scoreTexts.begin() + count, m_scoreTexts.end());
	m_namePanel.setSize(sf::Vector2f(560.f, 20.f));
	m_background.setSize(sf::Vector2f(600.f, 50.f + count * 16.f));
}

void Scoreboard::activate()
{
	m_active = true;
}

void Scoreboard::deactivate()
{
	m_active = false;
}

void Scoreboard::setFont(sf::Font& p_font)
{
	m_namePanelText.setFont(p_font);
	m_font = &p_font;
}

void Scoreboard::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	if (m_active)
	{
		target.draw(m_background, states);
		target.draw(m_namePanel, states);
		target.draw(m_namePanelText, states);

		for (auto i = m_scoreTexts.begin(); i != m_scoreTexts.end(); i++) 
		{
			target.draw(*i, states);
		}
	}
}
#include "Scoreboard.h"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

Scoreboard::Scoreboard(std::map<std::string, std::unique_ptr<Player>>& p_players)
	:
	m_players(p_players),
	m_background(sf::Vector2f(600, 200)),
	m_namePanel(sf::Vector2f(560, 20))
{
	m_background.setFillColor(sf::Color(128, 128, 128, 200));
	m_background.setOutlineColor(sf::Color::Black);
	m_background.setOutlineThickness(2.f);
	
	m_namePanel.setPosition(20.f, 10.f);
	m_namePanel.setFillColor(sf::Color(64, 64, 64, 128));
	m_namePanel.setOutlineColor(sf::Color::Black);
	m_namePanel.setOutlineThickness(1.f);

	m_namePanelText.setPosition(22.f, 11.f);
	m_namePanelText.setString("Name	K / D / Score");
	m_namePanelText.setCharacterSize(14);

	this->setOrigin(m_background.getGlobalBounds().width/2, m_background.getGlobalBounds().height/2);
}

Scoreboard::~Scoreboard()
{
}

std::string createScoreString(std::pair<std::string, Player*> i)
{
	return i.first 
			+ " " + std::to_string(i.second->m_score.m_points)
			+ " " + std::to_string(i.second->m_score.m_kills)
			+ " " + std::to_string(i.second->m_score.m_deaths);
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
			m_scoreTexts.push_back(sf::Text());
			m_scoreTexts[count].setFont(*m_namePanelText.getFont());
			m_scoreTexts[count].setCharacterSize(12);
		}
		m_scoreTexts[count].setString(createScoreString(*i));
		m_scoreTexts[count].setPosition(20.f, 30.f + count * 16.f);
	}
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
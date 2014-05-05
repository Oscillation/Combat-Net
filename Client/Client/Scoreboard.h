#pragma once

#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Transformable.hpp>

#include <vector>
#include <algorithm>
#include <map>
#include <memory>

#include "Player.h"

class Scoreboard : public sf::Drawable, public sf::Transformable
{
public:
	Scoreboard(std::map<std::string, std::unique_ptr<Player>>& p_players);
	~Scoreboard();

	void updateStats();

	void activate();
	void deactivate();

	void setFont(sf::Font& p_font);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const; 

private:
	std::map<std::string, std::unique_ptr<Player>>& m_players;
	std::vector<sf::Text> m_scoreTexts;

	sf::RectangleShape m_background;
	sf::RectangleShape m_namePanel;
	sf::Text m_namePanelText;

	bool m_active;
};
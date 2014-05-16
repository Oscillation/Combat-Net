#pragma once

#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Text.hpp>
#include "..\..\Common\Score.h"

class ScoreText : public sf::Drawable, public sf::Transformable
{
public:
	ScoreText(std::string const& p_name, bool p_special);
	~ScoreText();

	void update(Score p_score, std::string p_name);
	void setFont(sf::Font& p_font);

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Text m_name;
	sf::Text m_score;
	sf::Text m_kills;
	sf::Text m_deaths;
};
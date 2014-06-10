#pragma once
#include <SFML\Graphics\Rect.hpp>
#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

class TextLog : public sf::Drawable
{
public:
	TextLog();
	TextLog(const sf::Font & p_font, sf::Rect<int>* ptr_bounds);
	~TextLog();

	void push_back(const sf::Text & p_text);

	sf::Rect<int>* ptr_bounds;

	sf::Font m_font;

	void draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const;

	void align(const sf::Rect<int> & p_rect);

private:
	std::vector<sf::Text> m_log;

	sf::String wrapText(const sf::String & p_string, const unsigned int & p_width, const sf::Font & font, const unsigned int charicterSize, const bool bold = false);
};

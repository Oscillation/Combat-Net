#include "TextLog.h"

TextLog::TextLog()
{

}

TextLog::TextLog(const sf::Font* ptr_font) :
	ptr_font(ptr_font)
{

}

TextLog::~TextLog()
{

}

void TextLog::push_back(const sf::Text & p_text)
{
	m_log.push_back(p_text);
	m_log.back().setFont(*ptr_font);
	m_log.back().setString(wrapText(m_log.back().getString(), textLog_ptr_bounds->width, *ptr_font, m_log.back().getCharacterSize()));
	for (auto it = m_log.begin(); it != m_log.end() - 1; ++it)
	{
		it->setPosition(it->getPosition().x, it->getPosition().y - m_log.back().getLocalBounds().height);
		if ((it->getPosition().y + it->getLocalBounds().height) < textLog_ptr_bounds->top)
		{
			m_log.erase(it);
		}
	}
}

sf::String TextLog::wrapText(const sf::String & p_string, const unsigned int & p_width, const sf::Font & font, const unsigned int charicterSize, const bool bold)
{
	unsigned int currentOffset = 0;
	bool firstWord = true;
	unsigned int wordBeginning = 0;

	sf::String string = p_string;

	for (unsigned int i = 0; i < string.getSize(); ++i)
	{
		char currentChar = string[i];
		if (currentChar == '\n')
		{
			currentOffset = 0;
			firstWord = true;
			continue;
		} else if (currentChar == ' ')
		{
			wordBeginning = i;
			firstWord = false;
		}

		auto glyph = font.getGlyph(currentChar, charicterSize, bold);
		currentOffset += glyph.advance;

		if (!firstWord && currentOffset > p_width)
		{
			i = wordBeginning;
			string[i] = '\n';
			firstWord = true;
			currentOffset = 0;
		}
	}

	return string;
}

void TextLog::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const
{
	for (auto it = m_log.begin(); it != m_log.end(); ++it)
	{
		p_target.draw(*it, p_states);
	}
}

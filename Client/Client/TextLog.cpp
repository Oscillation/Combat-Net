#include "TextLog.h"

TextLog::TextLog()
{

}

TextLog::TextLog(const sf::Font & p_font, sf::Rect<int>* ptr_bounds) :
	m_font(p_font),
	ptr_bounds(ptr_bounds)
{
	
}

TextLog::~TextLog()
{

}

void TextLog::push_back(const sf::Text & p_text)
{
	m_log.push_back(p_text);
	
	m_log.back().setFont(m_font);

	m_log.back().setScale(sf::Vector2<float>(0.5f, 0.5f));

	sf::String string = wrapText(m_log.back().getString(), ptr_bounds->width, m_font, m_log.back().getCharacterSize()*(m_log.back().getScale().y));

	m_log.back().setString(string);

	m_log.back().setPosition(ptr_bounds->left, ptr_bounds->top + ptr_bounds->height - m_log.back().getLocalBounds().height*(m_log.back().getScale().y) - m_log.back().getCharacterSize()*(m_log.back().getScale().y));

	for (int i = 0; i < m_log.size(); i++)
	{
		if (i != m_log.size() - 1)
		{
			m_log[i].setPosition(m_log[i].getPosition().x, m_log[i].getPosition().y - m_log[i].getLocalBounds().height*(m_log.back().getScale().y));
		}
		
		if ((m_log[i].getPosition().y + m_log[i].getLocalBounds().height*(m_log.back().getScale().y)) < ptr_bounds->top)
		{
			m_log.erase(m_log.begin() + i);
		}
	}
}

sf::String TextLog::wrapText(const sf::String & p_string, const unsigned int & p_width, const sf::Font & font, const unsigned int charicterSize, const bool bold)
{
	unsigned int currentOffset = 0;
	bool firstWord = true;
	unsigned int wordBeginning = 0;

	std::string string = p_string.toAnsiString();

	for (unsigned int i = 0; i < string.size(); ++i)
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

	sf::String sfString = sf::String(string);

	return sfString;
}

void TextLog::draw(sf::RenderTarget & p_target, sf::RenderStates p_states) const
{
	for (auto it = m_log.begin(); it != m_log.end(); ++it)
	{
		p_target.draw(*it, p_states);
	}
}

void TextLog::align(const sf::Rect<int> & p_rect)
{
	for (int i = m_log.size() - 1; i > 0; i--)
	{
		sf::String string = wrapText(m_log[i].getString(), p_rect.width, m_font, m_log[i].getCharacterSize());
		m_log[i].setString(string);

		m_log[i].setPosition(p_rect.left, p_rect.top + p_rect.height - ((i - 1 >= 0) ? (m_log[i - 1].getLocalBounds().height + m_log[i - 1].getLocalBounds().top):0));
	}
}

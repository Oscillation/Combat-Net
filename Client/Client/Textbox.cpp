#include "Textbox.h"

#include <SFML\Graphics\RenderTarget.hpp>

namespace gui
{
	Textbox::Textbox(std::string p_text = "")
	{
		m_active = false;
		m_background.setOutlineColor(sf::Color::Black);
		m_background.setOutlineThickness(2.f);

		m_text.setPosition(10, 5);
		m_text.setCharacterSize(16);
		m_text.setStyle(sf::Text::Bold);
		m_empty = true;
		m_emptyText = p_text;
		m_text.setString(p_text);
		m_text.setColor(sf::Color(64, 64, 64));
	}

	Textbox::~Textbox()
	{

	}

	void Textbox::activate()
	{
		m_active = true;
		m_background.setFillColor(sf::Color::White);
	}

	void Textbox::deactivate()
	{
		m_active = false;
		m_background.setFillColor(sf::Color(100, 100, 100));
	}

	bool Textbox::isActive() const
	{
		return m_active;
	}

	void Textbox::handleEvent(sf::Event const& event)
	{
		if (m_active)
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::BackSpace)
				{
					if (!m_empty)
					{
						std::string str = m_text.getString();
						if (!str.empty())
							str.erase(str.end() - 1);

						m_text.setString(str);

						if (m_text.getString().isEmpty())
						{
							m_empty = true;
							m_text.setColor(sf::Color(64, 64, 64));
							m_text.setString(m_emptyText);
						}
					}
				}
			}
			else if (event.type == sf::Event::TextEntered)
			{
				if (event.text.unicode != 0x8 && event.text.unicode != 0x9) // backspace, tab
				{
					if (m_empty)
					{
						m_text.setString("");
						m_text.setColor(sf::Color::Black);
						m_empty = false;
					}
					m_text.setString(m_text.getString() + static_cast<char>(event.text.unicode));
				}
			}
		}
	}

	void Textbox::setSize(sf::Vector2f p_size)
	{
		m_background.setSize(p_size);
		setOrigin(m_background.getLocalBounds().width/2, m_background.getLocalBounds().height/2);
	}

	void Textbox::setFont(sf::Font& p_font)
	{
		m_text.setFont(p_font);
	}

	void Textbox::setEmptyText(std::string p_text)
	{
		m_emptyText = p_text;
	}

	std::string Textbox::getValue() const
	{
		if (m_empty)
			return "";
		else
			return m_text.getString();
	}

	sf::FloatRect Textbox::getRectangle() const
	{
		return sf::FloatRect(getPosition() - getOrigin(), m_background.getSize());
	}

	void Textbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform = getTransform();
		
		target.draw(m_background, states);
		target.draw(m_text, states);
	}
}
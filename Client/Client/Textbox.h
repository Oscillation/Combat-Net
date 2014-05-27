#pragma once

#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\RectangleShape.hpp>
#include <SFML\Window\Event.hpp>
#include <SFML\System\Vector2.hpp>

namespace gui
{
	class Textbox : public sf::Transformable, public sf::Drawable
	{
	public:
		Textbox(const std::string & p_text);
		~Textbox();

		void activate();
		void deactivate();

		bool isActive() const;

		void handleEvent(sf::Event const& event);
		std::string getValue() const;

		void setSize(sf::Vector2f p_size);
		void setFont(sf::Font& p_font);

		sf::FloatRect getRectangle() const;
		void setEmptyText(std::string p_text);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		sf::Text m_text;
		sf::RectangleShape m_background;
		bool m_active;
		bool m_empty;
		std::string m_emptyText;
	};
}
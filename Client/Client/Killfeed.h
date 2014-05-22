#pragma once

#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Transformable.hpp>
#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Font.hpp>
#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

class Killfeed : public sf::Transformable, public sf::Drawable
{
	public:
		Killfeed(sf::Font & p_font);
		~Killfeed();

		void registerKill(const std::string & p_killer, const std::string & p_killed);

	private:	
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		sf::Text m_feed[4];
};

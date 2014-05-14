#pragma once

#include "Component.h"

#include <functional>

#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

namespace gui
{
	class Button : public Component
	{
		public:
									Button(sf::Vector2f size, sf::Font* font, bool fade = true);
			virtual					~Button();

			bool					isSelectable() const; // Returns true
			
			void					setText(std::string text);

			virtual void			activate();

			virtual void			handleEvent(sf::Event const& event);

			virtual void			setAction(std::function<void()> const& action);
			
			virtual void			select();
			virtual void			deselect();

		private:
			virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;

		private:		
			std::function<void()>		mAction;	// Holy fuck I love this
			sf::Text					mText;
			sf::RectangleShape			mBackground;
	};
}
#pragma once

#include "Component.h"

#include <functional>

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Text.hpp>

namespace gui
{
	class Button : public Component
	{
		public:
									Button(sf::Texture* texture, sf::Font* font, bool fade = true);
			virtual					~Button();

			bool					isSelectable() const; // Returns true
			
			void					setText(std::string text);

			virtual void			activate();

			virtual void			handleEvent(sf::Event const& event);

			virtual void			setAction(std::function<void()> const& action);
			
			virtual void			select();
			virtual void			deselect();

			bool					fade_in();

		private:
			virtual void			draw(sf::RenderTarget& target, sf::RenderStates states) const;

		private:		
			std::function<void()>		mAction;	// Holy fuck I love this
			sf::Sprite					mSprite;
			sf::Texture*				mTexture;	// This is here for the toggleable buttons
			sf::Text					mText;
	};
}
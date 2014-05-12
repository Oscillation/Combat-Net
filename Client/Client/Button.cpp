#include "Button.h"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

#include <SFML\Window\Event.hpp>

gui::Button::Button(sf::Texture* texture, sf::Font* font, bool fade)
	:
	mTexture(texture),
	mText(),
	mSprite()
{
	mSprite.setTexture(*texture);
	mText.setFont(*font);
	mText.setCharacterSize(22);
	deactivate();
	mText.setColor(sf::Color::Black);

	if (fade)
		mSprite.setColor(sf::Color(255, 255, 255, 0));

	setOrigin(mSprite.getLocalBounds().width/2, mSprite.getLocalBounds().height/2);
}

gui::Button::~Button()
{
}

bool gui::Button::isSelectable() const
{
	return true;
}

void gui::Button::setText(std::string text)
{
	// Set the string
	mText.setString(text);
	mText.setOrigin(mText.getLocalBounds().width/2, mText.getLocalBounds().height);

	// Set the position of the text 
	mText.setPosition(sf::Vector2f(mTexture->getSize() / 2u));
}

void gui::Button::activate()
{
	Component::activate();
	mAction();
	Component::deactivate();
}

void gui::Button::handleEvent(sf::Event const& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Button::Left)
		{
			int x = event.mouseButton.x;
			int y = event.mouseButton.y;

			sf::FloatRect bounds = mSprite.getLocalBounds();
			bounds.left = getPosition().x - bounds.width / 2;
			bounds.top = getPosition().y - bounds.height / 2;

			if (bounds.intersects(sf::FloatRect(x, y, 1,1)))
			{
				activate();
			}
		}
	}

	if (event.type == sf::Event::MouseMoved)
	{
		int x = event.mouseMove.x;
		int y = event.mouseMove.y;

		sf::FloatRect bounds = mSprite.getLocalBounds();
		bounds.left = getPosition().x - bounds.width / 2;
		bounds.top = getPosition().y - bounds.height / 2;

		if (bounds.intersects(sf::FloatRect(x, y, 1,1)))
		{
			select();
		}
		else 
		{
			deselect();
		}
	}
}

void gui::Button::setAction(std::function<void()> const& action) 
{
	mAction = action;
}

void gui::Button::select()
{
	Component::select();
	mText.setColor(sf::Color::White);
}

bool gui::Button::fade_in()
{
	sf::Color c = mSprite.getColor();
	c.a += 5;

	mSprite.setColor(c);

	return c.a >= 255;
}

void gui::Button::deselect()
{
	Component::deselect();
	mText.setColor(sf::Color::Black);
}

void gui::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
	target.draw(mText, states);
}
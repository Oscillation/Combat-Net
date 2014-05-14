#include "Button.h"

#include <SFML\Graphics\RenderTarget.hpp>
#include <SFML\Graphics\RenderStates.hpp>

#include <SFML\Window\Event.hpp>

gui::Button::Button(sf::Vector2f size, sf::Font* font, bool fade)
	:
	mText(),
	mBackground(size)
{
	mText.setFont(*font);
	mText.setCharacterSize(22);
	deactivate();
	mText.setColor(sf::Color::Black);
	
	mBackground.setFillColor(sf::Color::Red);
	mBackground.setOutlineColor(sf::Color::Black);
	mBackground.setOutlineThickness(2.f);
	setOrigin(mBackground.getLocalBounds().width/2, mBackground.getLocalBounds().height/2);
	mText.setStyle(sf::Text::Bold);
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
	mText.setPosition(sf::Vector2f(mBackground.getSize().x / 2, mBackground.getSize().y/2));
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

			sf::FloatRect bounds = mBackground.getLocalBounds();
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

		sf::FloatRect bounds = mBackground.getLocalBounds();
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

void gui::Button::deselect()
{
	Component::deselect();
	mText.setColor(sf::Color::Black);
}

void gui::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mBackground, states);
	target.draw(mText, states);
}
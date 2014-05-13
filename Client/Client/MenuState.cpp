#include "MenuState.h"

#include <SFML\Graphics\RenderWindow.hpp>

MenuState::MenuState(StateStack& stateStack, Context context, States::ID id)
	:
	State(stateStack, context, id)
{
}

MenuState::~MenuState()
{
}

bool MenuState::update(sf::Time& p_deltaTime)
{
	return false;
}

bool MenuState::handleEvents(const sf::Event& event)
{
	return false;
}

void MenuState::draw()
{
	getContext().window->clear(sf::Color::Green);
}
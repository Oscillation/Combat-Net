#include "MenuState.h"

#include <SFML\Graphics\RenderWindow.hpp>

MenuState::MenuState(StateStack& stateStack, Context& context, States::ID id)
	:
	State(stateStack, context, id)
{
}

MenuState::~MenuState()
{
}

bool MenuState::update(sf::Time& p_deltaTime)
{
	State::Context* con = getContext();
	if (con->address)
		delete con->address;
	con->address = new sf::IpAddress("127.0.0.1");
	con->port = 2828;
	requestStackPush(States::Game);
	return false;
}

bool MenuState::handleEvents(const sf::Event& event)
{
	return false;
}

void MenuState::draw()
{
	getContext()->window->clear(sf::Color::Green);
}
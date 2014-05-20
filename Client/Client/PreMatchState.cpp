#include "PreMatchState.h"

PreMatchState::PreMatchState(StateStack& stateStack, Context& context, States::ID id)
	:
	State(stateStack, context, id)
{
	m_statusText.setFont(*context.font);
	m_statusText.setColor(sf::Color::Red);
	m_statusText.setPosition(100, 100);
	m_statusText.setString("Waiting for match...");
}

PreMatchState::~PreMatchState()
{
}

bool PreMatchState::update(sf::Time & p_deltaTime)
{
	return false;
}

bool PreMatchState::draw()
{
	getContext()->window->clear(sf::Color::Black);
	getContext()->window->draw(m_statusText);
	return false;
}

bool PreMatchState::handleEvents(const sf::Event& event)
{
	return false;
}
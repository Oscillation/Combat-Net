#pragma once

#include "State.h"
#include "Scoreboard.h"

#include <SFML\Graphics\Text.hpp>

class PreMatchState : public State
{
public:
	PreMatchState(StateStack& stateStack, Context& context, States::ID id);
	~PreMatchState();

	bool update(sf::Time & p_deltaTime);
	bool draw();
	bool handleEvents(const sf::Event& event);

private:
	sf::Text m_statusText;
	Scoreboard m_scoreboard;
};
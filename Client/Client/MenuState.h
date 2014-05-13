#pragma once

#include "State.h"
#include "Button.h"

class MenuState : public State
{
public:
	MenuState(StateStack& stateStack, Context context, States::ID id);
	virtual ~MenuState();

	bool update(sf::Time& p_deltaTime);
	bool handleEvents(const sf::Event& event);
	void draw();

private:
};
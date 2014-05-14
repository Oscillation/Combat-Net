#pragma once

#include "State.h"
#include "Button.h"
#include "Textbox.h"

class MenuState : public State
{
public:
	MenuState(StateStack& stateStack, Context& context, States::ID id);
	virtual ~MenuState();

	bool update(sf::Time& p_deltaTime);
	bool handleEvents(const sf::Event& event);
	bool draw();

private:
	gui::Button m_button;
	gui::Textbox m_ipbox;
	gui::Textbox m_namebox;
};
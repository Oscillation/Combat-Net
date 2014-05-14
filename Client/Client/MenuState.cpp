#include "MenuState.h"

#include <SFML\Graphics\RenderWindow.hpp>

MenuState::MenuState(StateStack& stateStack, Context& context, States::ID id)
	:
	State(stateStack, context, id),
	m_button(sf::Vector2f(100, 40), context.font, false),
	m_ipbox()
{
	m_button.setPosition(400, 400);
	m_button.setAction([this]() {
		State::Context* con = getContext();
		if (con->address)
			delete con->address;
		con->address = new sf::IpAddress(m_ipbox.getValue());
		con->username = m_namebox.getValue();
		con->port = 2828;
		requestStackPush(States::Game);
	});
	m_button.setText("Connect");

	m_ipbox.setPosition(400, 350);
	m_ipbox.setSize(sf::Vector2f(400, 30));
	m_ipbox.setFont(*context.font);
	m_ipbox.deactivate();

	m_namebox.setPosition(400, 300);
	m_namebox.setSize(sf::Vector2f(400, 30));
	m_namebox.setFont(*context.font);
	m_namebox.activate();
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
	if (event.type == sf::Event::MouseButtonPressed)
	{
		sf::FloatRect mousePos(sf::Vector2f(event.mouseButton.x, event.mouseButton.y), sf::Vector2f(1,1));
		sf::FloatRect namerect = m_namebox.getRectangle();
		sf::FloatRect iprect = m_ipbox.getRectangle();
		if (mousePos.intersects(iprect))
		{
			m_ipbox.activate();
			m_namebox.deactivate();
		}
		else if (mousePos.intersects(namerect))
		{
			m_namebox.activate();
			m_ipbox.deactivate();
		}
	}
	else if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.code == sf::Keyboard::Tab)
		{
			if (m_ipbox.isActive())
			{
				m_ipbox.deactivate();
				m_namebox.activate();
			}
			else 
			{
				m_namebox.deactivate();
				m_ipbox.activate();
			}
		}
		else if (event.key.code == sf::Keyboard::Return)
		{
			m_button.activate();
		}
	}
	
	m_button.handleEvent(event);
	m_ipbox.handleEvent(event);
	m_namebox.handleEvent(event);
	return false;
}

bool MenuState::draw()
{
	getContext()->window->clear(sf::Color(64, 64, 64));
	getContext()->window->draw(m_button);
	getContext()->window->draw(m_ipbox);
	getContext()->window->draw(m_namebox);
	return true;
}
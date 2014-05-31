#include "MenuState.h"

#include <SFML\Graphics\RenderWindow.hpp>

MenuState::MenuState(StateStack& stateStack, Context& context, States::ID id)
	:
	State(stateStack, context, id),
	m_button(sf::Vector2f(100, 40), context.font, false),
	m_ipbox("ip:port"),
	m_namebox("username")
{
	m_button.setPosition(1280/2, 400);
	m_button.setAction([this]() {
		State::Context* con = getContext();
		std::string address = m_ipbox.getValue();
		int pos = -1;
		pos = address.find(':');
		if (pos != -1)
		{
			std::string port = address.substr(pos + 1);
			address = address.substr(0, pos);
			con->port = std::atoi(port.c_str());
		}
		else 
		{
			con->port = 2828;
		}
		con->address = new sf::IpAddress(address);
		con->username = m_namebox.getValue();
		requestStackPush(States::Game);
	});
	m_button.setText("Connect");

	m_ipbox.setSize(sf::Vector2f(400, 30));
	m_ipbox.setPosition(1280/2, 350);
	m_ipbox.setFont(*context.font);
	m_ipbox.deactivate();

	m_namebox.setSize(sf::Vector2f(400, 30));
	m_namebox.setPosition(1280/2, 300);
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
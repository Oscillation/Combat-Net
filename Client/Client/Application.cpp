#include "Application.h"

#include <SFML\Network\IpAddress.hpp>
#include "MenuState.h"
#include "MultiplayerGame.h"
#include "PreMatchState.h"

Application::Application()
	:
	m_window(sf::VideoMode(1280, 720), "Combat-Net", sf::Style::Close),
	m_socket(),
	m_stateStack(State::Context(m_window, m_socket, m_font, sf::IpAddress::None, 2828)),
	m_running(true),
	m_active(true)
{
}

Application::~Application()
{
	m_window.close();
	m_socket.unbind();
}

void Application::run()
{
	initialize();

	sf::Clock dt;
	sf::Time lag;
	sf::Time updateTime = sf::seconds(1.f/60.f);

	while (m_running)
	{
		lag += dt.getElapsedTime();
		dt.restart();

		handleEvents();
		draw();
		while (lag >= updateTime)
		{
			update(updateTime);
			lag -= updateTime;
		}
	}
}

void Application::initialize()
{
	m_font.loadFromFile("Segan-Light.ttf");

	m_stateStack.registerState<MenuState>(States::Menu);
	m_stateStack.registerState<MultiplayerGame>(States::Game);
	m_stateStack.registerState<PreMatchState>(States::PreMatch);
	
	m_stateStack.pushState(States::Menu);
}

void Application::update(sf::Time dt)
{
	m_stateStack.update(dt);
}

void Application::handleEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
			m_running = false;
		}else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_stateStack.clearStates();
			m_stateStack.pushState(States::ID::Menu);
		}else if (event.type == sf::Event::GainedFocus){
			m_active = true;
		} else if (event.type == sf::Event::LostFocus) {
			m_active = false;
		}
		m_stateStack.handleEvent(event);
	}
}

void Application::draw()
{
	m_window.clear();
	m_stateStack.draw();
	m_window.display();
}
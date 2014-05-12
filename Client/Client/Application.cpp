#include "Application.h"

Application::Application()
	:
	m_window(sf::VideoMode(1280, 720), "Combat Net", sf::Style::Close),
	m_socket(),
	m_stateStack(State::Context(m_window, m_socket)),
	m_running(true)
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

}

void Application::update(sf::Time dt)
{
}

void Application::handleEvents()
{

}

void Application::draw()
{

}
#include "MultiplayerGame.h"

#include <iostream>

MultiplayerGame::MultiplayerGame() : 
	m_window(sf::VideoMode(1280, 720), "Combat Net", sf::Style::Close),
	m_running(true)
{
	run();
}

MultiplayerGame::~MultiplayerGame()
{
	m_window.close();
}

void MultiplayerGame::run()
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
		render();
		while (lag >= updateTime)
		{
			update(updateTime);
			lag -= updateTime;
		}
	}
}

void MultiplayerGame::initialize()
{

}

void MultiplayerGame::handleEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)))
		{
			m_running = false;
		}
	}
}

void MultiplayerGame::update(sf::Time & p_deltaTime)
{

}

void MultiplayerGame::render()
{
	m_window.clear();
	m_window.display();
}
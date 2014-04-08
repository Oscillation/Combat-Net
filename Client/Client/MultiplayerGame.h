#pragma once

#include <SFML\System\Time.hpp>
#include <SFML\System\Clock.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Window\Event.hpp>

class MultiplayerGame
{
public:
	MultiplayerGame();
	~MultiplayerGame();

private:
	void run();
	void initialize();

	void update(sf::Time & p_deltaTime);
	void render();

	void handleEvents();

private:

	sf::RenderWindow m_window;
	bool m_running;
};
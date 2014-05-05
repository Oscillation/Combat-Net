#include <iostream>

#include "MultiplayerGame.h"

int main(int argc, char* argv[])
{
	MultiplayerGame game;
	game.run(sf::IpAddress(argv[1]), 2828);
	
	return 0;
}
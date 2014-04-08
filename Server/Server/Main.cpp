#include <iostream>
#include <memory>
#include "Server.h"

int main(int argc, char* argv[]){

	std::cout << "Starting server on: " << sf::IpAddress::getPublicAddress()  << ":" << std::atoi(argv[1]) << "...\n";
	Server* server(new Server(std::atoi(argv[1])));
	delete server;
	return 0;
}
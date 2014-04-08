#include <iostream>
#include "Server.h"

int main(int argc, char* argv[]){

	std::cout << "Starting server on: " << sf::IpAddress::getPublicAddress()  << ":" << std::atoi(argv[1]) << "...\n";
	Server server(std::atoi(argv[1]));
	return 0;
}
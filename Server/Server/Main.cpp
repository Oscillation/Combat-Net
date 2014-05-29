#include <iostream>
#include <memory>
#include "Server.h"

int main(int argc, char* argv[]){
	Server* server = new Server();
	delete server;
	return 0;
}
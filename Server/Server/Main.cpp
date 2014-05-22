#include <iostream>
#include <memory>
#include "Server.h"

#define projectileSpeed 50

int main(int argc, char* argv[]){
	Server* server = new Server();
	delete server;
	return 0;
}
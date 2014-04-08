Combat-Net
==========

SERVER 

UdpSocket listens for inc connections
Map of (Name, Client) (CLIENT is (sf::IPAdress, unsinged short port)

when a connection is made:
	client struct is created and added to map
	sends a playerConnect packet to all clients

UPDATE LOOP
while (run)
{
	getClientUpdates(); // non blocking, sf::SocketSelector 
	processUpdates();
	constructPackets();
	sendClients();
}


CLIENT 
UdpSocket // nonblocking
Server ip/port

UPDATE LOOP

while (run)
{
	getInput();
	getServerUpdate();
	constructPackets();
	sendServer();

	draw();
}

PACKET PROTOCOL


enum PacketType
{
	PlayerConnect, (playerID, pos)
	PlayerMove,	   (playerID, pos)
	PlayerDisconnect, (playerID)
	PlayerInput, (playerID, inputs)
	PlayerAttack, (playerID, bullet)
}

packet << PacketType << playerID << data;

Le fed concept: when you get a kill, maxHp goes down, attackspeed or smthing goes up

packet << 12 << "hello";

int a;
string potato;
packet >> a >> potato;


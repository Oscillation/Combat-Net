#pragma once
#include <vector>
#include <SFML\Network\Packet.hpp>

enum Type{
	Floor,
	Wall,
};

struct Tile{
	Tile(const unsigned short & p_x, const unsigned short & p_y, const Type & p_type);
	Tile();
	~Tile();

	int m_x, m_y;

	Type m_type;
};

class Map{
public:
	Map();
	Map(const std::string & p_path);
	~Map();

	std::vector<std::vector<Tile>> m_tiles;
};

	sf::Packet& operator <<(sf::Packet& packet, const Type& type);
	sf::Packet& operator >>(sf::Packet& packet, Type& type);
	sf::Packet& operator <<(sf::Packet& packet, const Tile& tile);
	sf::Packet& operator >>(sf::Packet& packet, Tile& tile);
	sf::Packet& operator <<(sf::Packet& packet, const std::vector<std::vector<Tile>>& tiles);
	sf::Packet& operator >>(sf::Packet& packet, std::vector<std::vector<Tile>>& tiles);
	sf::Packet& operator <<(sf::Packet& packet, const Map& map);
	sf::Packet& operator >>(sf::Packet& packet, Map& map);
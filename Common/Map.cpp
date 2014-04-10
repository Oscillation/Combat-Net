#include "Map.h"

Tile::Tile(const unsigned short & p_x, const unsigned short & p_y, const Type & p_type) : m_x(p_x), m_y(p_y), m_type(p_type){

}

Tile::Tile() : m_type(Floor){

}

Tile::~Tile(){

}

Map::Map(){
	m_tiles.resize(16, std::vector<Tile>(16, Tile()));
	for (int x = 0, y = 0; x < m_tiles.size(); x++){
		for (int y = 0; y < m_tiles[x].size(); y++){
			m_tiles[x][y].m_x = x;
			m_tiles[x][y].m_y = y;
		}
	}
}

Map::Map(const std::string & p_path){
	m_tiles.resize(16, std::vector<Tile>(16, Tile()));
	for (int x = 0, y = 0; x < m_tiles.size(); x++){
		for (int y = 0; y < m_tiles[x].size(); y++){
			m_tiles[x][y].m_x = x;
			m_tiles[x][y].m_y = y;
		}
	}

	std::ifstream read;

	int height = 0;

	read.open(p_path);
	if (read.is_open())
	{
		std::string line;

		while (!read.eof())
		{
			std::getline(read, line);
			height++;
		}
	}
	read.close();

	read.open(p_path);
	if (read.is_open())
	{
		std::string line;

		while (!read.eof())
		{
			for (int y = 0; y < height; y++)
			{
				std::getline(read, line);
				for (int x = 0; x < line.length(); x++)
				{
					switch (line[x])
					{
					case '#':
						m_tiles[x][y].m_type = Floor;
						break;
					case 'x':
						m_tiles[x][y].m_type = Wall;
						break;
					case '\n':
						y++;
						std::getline(read, line);
						break;
					}
				}
			}
		}
	}
}

Map::~Map(){

}
#include <iostream>

bool Map::intersectsWall(const sf::Vector2<float> & p_position, const unsigned short & p_dir) {
	sf::Rect<int> rect = sf::Rect<int>(p_position.x - 20, p_position.y + 20, 40, 40);

	switch (p_dir)
	{
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		break;
	}
	return false;
}

sf::Packet& operator<<(sf::Packet& packet, const Type& type){
	int i = type;
	return packet << i;
}

sf::Packet& operator>>(sf::Packet& packet, Type& type){
	int i;
	packet >> i;
	type = (Type)i;
	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const Tile& tile){
	return packet << tile.m_x << tile.m_y << tile.m_type;
}

sf::Packet& operator>>(sf::Packet& packet, Tile& tile){
	return packet >> tile.m_x >> tile.m_y >> tile.m_type;
}

sf::Packet& operator<<(sf::Packet& packet, const std::vector<std::vector<Tile>>& tiles){
	for (auto x = tiles.begin(); x != tiles.end(); ++x){
		for (auto y = x->begin(); y != x->end(); ++y){
			packet << *y;
		}
	}
	return packet;
}

sf::Packet& operator>>(sf::Packet& packet, std::vector<std::vector<Tile>>& tiles){
	for (auto x = tiles.begin(); x != tiles.end(); ++x){
		for (auto y = x->begin(); y != x->end(); ++y){
			packet >> *y;
		}
	}
	return packet;
}

sf::Packet& operator<<(sf::Packet& packet, const Map& map){
	return packet << map.m_tiles;
}

sf::Packet& operator>>(sf::Packet& packet, Map& map){
	return packet >> map.m_tiles;
}
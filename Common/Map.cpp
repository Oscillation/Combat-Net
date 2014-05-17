#include "Map.h"

template <typename  T>
bool math::circleIntersectsRect(const sf::Vector2<T> & p_pos, const float & p_radius, const sf::Rect<T> & p_rect){
	sf::Vector2<float> distance;
	distance.x = std::abs(p_pos.x - p_rect.left);
	distance.y = std::abs(p_pos.y - p_rect.top);

	if (distance.x > (p_rect.width/2 + p_radius))
	{
		return false;
	}
	if (distance.y > (p_rect.height/2 + p_radius))
	{
		return false;
	}

	if (distance.x <= (p_rect.width/2))
	{
		return true;
	} 
	if (distance.y <= (p_rect.height/2))
	{
		return true;
	}

	float cornerDistance = std::pow((distance.x - p_rect.width/2), 2) + std::pow((distance.y - p_rect.height/2), 2);

	return (cornerDistance <= std::pow(p_radius, 2));
}

Tile::Tile(const unsigned short & p_x, const unsigned short & p_y, const Type & p_type) : m_x(p_x), m_y(p_y), m_type(p_type){

}

Tile::Tile() : m_type(Floor){

}

Tile::~Tile(){

}

Map::Map(){
}

Map::Map(const std::string & p_path){

	std::ifstream read;

	unsigned int height = 0, width = 0;

	read.open(p_path);
	if (read.is_open())
	{
		std::string line;

		while (!read.eof())
		{
			std::getline(read, line);
			if (line.length() > width)
			{
				width = line.length();
			}
			height++;
		}
	}
	read.close();

	m_tiles.resize(width, std::vector<Tile>(height, Tile()));
	for (unsigned int x = 0, y = 0; x < m_tiles.size(); x++){
		for (y = 0; y < m_tiles[x].size(); y++){
			m_tiles[x][y].m_x = x;
			m_tiles[x][y].m_y = y;
		}
	}

	read.open(p_path);
	if (read.is_open())
	{
		std::string line;

		while (!read.eof())
		{
			for (unsigned int y = 0, x = 0; y < height; y++)
			{
				std::getline(read, line);
				for (x = 0; x < line.length(); x++)
				{
					switch (line[x])
					{
					case ' ':
						m_tiles[x][y].m_type = Floor;
						break;
					case 'x':
						m_tiles[x][y].m_type = Wall;
						break;
					case 'S':
						m_tiles[x][y].m_type = Spawn;
						m_spawnPositions.push_back(sf::Vector2<int>(x, y));
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

bool Map::intersectsWall(const sf::Vector2<float> & p_position) {
	unsigned int x = p_position.x/64 - 1, y = p_position.y/64 - 1;
	for (x = p_position.x/64 - 1, y = p_position.y/64 - 1; x < p_position.x/64 + 1; x++)
	{
		for (y = p_position.y/64 - 1; y < p_position.y/64 + 1; y++)
		{
			if (x >= 0 && x < m_tiles.size() && y >= 0 && y < m_tiles.begin()->size())
			{
				if (m_tiles[x][y].m_type == Wall)
				{
					if (math::circleIntersectsRect(sf::Vector2<float>(p_position.x - 33, p_position.y - 33), 17, sf::Rect<float>(x*64, y*64, 64, 64)))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool Map::intersectsWall(const sf::Rect<float> & p_position) {
	unsigned int x = p_position.left/64 - 1, y = p_position.top/64 - 1;
	for (x = p_position.left/64 - 1, y = p_position.top/64 - 1; x < p_position.left/64 + 1; x++)
	{
		for (y = p_position.top/64 - 1; y < p_position.top/64 + 1; y++)
		{
			if (x >= 0 && x < m_tiles.size() && y >= 0 && y < m_tiles.begin()->size())
			{
				if (m_tiles[x][y].m_type == Wall)
				{
					if (p_position.intersects(sf::Rect<float>(x*64, y*64, 64, 64)))
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

sf::Vector2<float> Map::getIntersectingWall(const sf::Vector2<float> & p_position){
	unsigned int x = p_position.x/64 - 1, y = p_position.y/64 - 1;
	for (x = p_position.x/64 - 1, y = p_position.y/64 - 1; x < p_position.x/64 + 1; x++)
	{
		for (y = p_position.y/64 - 1; y < p_position.y/64 + 1; y++)
		{
			if (x >= 0 && x < m_tiles.size() && y >= 0 && y < m_tiles.begin()->size())
			{
				if (m_tiles[x][y].m_type == Wall)
				{
					if (math::circleIntersectsRect(sf::Vector2<float>(p_position.x - 33, p_position.y - 33), 17, sf::Rect<float>(x*64, y*64, 64, 64)))
					{
						return sf::Vector2<float>(x*64, y*64);
					}
				}
			}
		}
	}
}

sf::Vector2<float> Map::getIntersectingWall(const sf::Rect<float> & p_position){
	unsigned int x = p_position.left/64 - 1, y = p_position.top/64 - 1;
	for (x = p_position.left/64 - 1, y = p_position.top/64 - 1; x < p_position.left/64 + 1; x++)
	{
		for (y = p_position.top/64 - 1; y < p_position.top/64 + 1; y++)
		{
			if (x >= 0 && x < m_tiles.size() && y >= 0 && y < m_tiles.begin()->size())
			{
				if (m_tiles[x][y].m_type == Wall)
				{
					if (p_position.intersects(sf::Rect<float>(x*64, y*64, 64, 64)))
					{
						return sf::Vector2<float>(x*64, y*64);
					}
				}
			}
		}
	}
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
	return packet << map.m_tiles.size() << map.m_tiles.back().size() << map.m_tiles;
}

sf::Packet& operator>>(sf::Packet& packet, Map& map){
	int height = 0, width = 0;
	packet >> width >> height;
	map.m_tiles.resize(width, std::vector<Tile>(height, Tile()));
	return packet >> map.m_tiles;
}
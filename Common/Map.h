#pragma once
#include <vector>
#include <fstream>
#include <SFML\Network\Packet.hpp>
#include <SFML\Graphics\Rect.hpp>

#include "Tile.h"
#include "..\Server\Server\PowerTile.h"
#include "GeneralMath.h"

class Map{
public:
	Map();
	Map(const std::string & p_path);
	~Map();

	std::vector<std::vector<Tile>> m_tiles;
	std::vector<PowerTile> getPowerTiles() const;

	bool intersectsWall(const sf::Vector2<float> & p_position);
	bool intersectsWall(const sf::Vector2<float> & p_p1, const sf::Vector2<float> & p_p2);
	bool intersectsWall(const sf::Rect<float> & p_position);

	sf::Vector2<float> getIntersectingWall(const sf::Vector2<float> & p_position);
	sf::Vector2<float> getIntersectingWall(const sf::Rect<float> & p_position);

	std::vector<sf::Vector2<int>> m_spawnPositions;
};

	sf::Packet& operator <<(sf::Packet& packet, const TileType::Type & type);
	sf::Packet& operator >>(sf::Packet& packet, TileType::Type & type);
	sf::Packet& operator <<(sf::Packet& packet, const Tile & tile);
	sf::Packet& operator >>(sf::Packet& packet, Tile & tile);
	sf::Packet& operator <<(sf::Packet& packet, const std::vector<std::vector<Tile>> & tiles);
	sf::Packet& operator >>(sf::Packet& packet, std::vector<std::vector<Tile>> & tiles);
	sf::Packet& operator <<(sf::Packet& packet, const Map & map);
	sf::Packet& operator >>(sf::Packet& packet, Map & map);
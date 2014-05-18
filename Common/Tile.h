#pragma once

namespace TileType{
	enum Type{
		Floor,
		Wall,
		Spawn,
		Power,
	};
}

struct Tile{
	Tile(const unsigned short & p_x, const unsigned short & p_y, const TileType::Type & p_type);
	Tile();
	~Tile();

	int m_x, m_y;

	TileType::Type m_type;
};
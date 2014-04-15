#pragma once

enum Type{
	Floor,
	Wall,
	Spawn,
};

struct Tile{
	Tile(const unsigned short & p_x, const unsigned short & p_y, const Type & p_type);
	Tile();
	~Tile();

	int m_x, m_y;

	Type m_type;
};
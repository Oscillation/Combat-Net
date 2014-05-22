#pragma once
#include <map>
#include <vector>
#include "..\..\Common\Protocol.h"

struct Match
{
	int pointsToWin;
	cn::MatchType type;
	std::vector<std::string> maps;
	bool active;
	int maxPlayers;
	int teamAmount;

	///<summary>Team tag, size.</summary>
	std::map<unsigned short, unsigned int> m_teams;
};
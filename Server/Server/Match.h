#pragma once
#include <map>
#include "..\..\Common\Protocol.h"

struct Match
{
	int pointsToWin;
	cn::MatchType type;
	std::string map;
	bool active;

	///<summary>Team tag, size.</summary>
	std::map<unsigned short, unsigned int> m_teams;
};
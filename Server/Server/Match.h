#pragma once

#include "..\..\Common\Protocol.h"

struct Match
{
	int pointsToWin;
	cn::MatchType type;
	std::string map;
	bool active;
};
#pragma once
#include "GeneralMath.h"

int math::random(int low, int high){
	if (low > high){
		int temp = low;
		low = high;
		high = temp;
	}
	return low + (std::rand() % (high - low + 1));
}

int math::random(int low, int high, int exclude){
	if (low > high){
		int temp = low;
		low = high;
		high = temp;
	}
	int i = low + (std::rand() % (high - low + 1));
	while (i == exclude)
	{
		i = random(low, high, exclude);
	}
	return i;
}

int math::random(int low, int high, std::vector<int> exclude){
	if (low > high){
		int temp = low;
		low = high;
		high = temp;
	}
	int i = low + (std::rand() % (high - low + 1));
	for (int j = 0; j < exclude.size(); j++)
	{
		if (i == exclude[j])
		{
			random(low, high, exclude);
		}
	}
	return i;
}

float math::toDegrees(float x)
{
	return x * 180 / std::_Pi;
}

float math::toRadians(float x)
{
	return x / 180 * std::_Pi;
}


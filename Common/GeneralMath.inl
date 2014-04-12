#pragma once

#include "GeneralMath.h"

template <typename T>
float math::distance(sf::Vector2<T> const& lhs, sf::Vector2<T> const& rhs){
	return (float)std::sqrt(std::pow(std::abs(rhs.x - lhs.x), 2) + std::pow(std::abs(lhs.y - rhs.y), 2));
}

template <typename T>
bool math::LineIntersectsLine(const sf::Vector2<T> & l1p1, const sf::Vector2<T> & l1p2, const sf::Vector2<T> & l2p1, const sf::Vector2<T> & l2p2)
{
	float q = (l1p1.y - l2p1.y) * (l2p2.x - l2p1.x) - (l1p1.x - l2p1.x) * (l2p2.y - l2p1.y);
	float d = (l1p2.x - l1p1.x) * (l2p2.y - l2p1.y) - (l1p2.y - l1p1.y) * (l2p2.x - l2p1.x);

	if( d == 0 )
	{
		return false;
	}

	float r = q / d;

	q = (l1p1.y - l2p1.y) * (l1p2.x - l1p1.x) - (l1p1.x - l2p1.x) * (l1p2.y - l1p1.y);
	float s = q / d;

	if( r < 0 || r > 1 || s < 0 || s > 1 )
	{
		return false;
	}

	return true;
}

template <typename T>
bool math::LineIntersectsRect(const sf::Vector2<T> &  p1, const sf::Vector2<T> &  p2, const sf::Rect<T> & r)
{
	return LineIntersectsLine(p1, p2, sf::Vector2<T>(r.left, r.top), sf::Vector2<T>(r.left + r.width, r.top)) ||
		LineIntersectsLine(p1, p2, sf::Vector2<T>(r.left + r.width, r.top), sf::Vector2<T>(r.left + r.width, r.top + r.height)) ||
		LineIntersectsLine(p1, p2, sf::Vector2<T>(r.left + r.width, r.top + r.height), sf::Vector2<T>(r.left, r.top + r.height)) ||
		LineIntersectsLine(p1, p2, sf::Vector2<T>(r.left, r.top + r.height), sf::Vector2<T>(r.left, r.top)) ||
		(r.contains(p1) || r.contains(p2));
}
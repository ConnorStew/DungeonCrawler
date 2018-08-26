#include "stdafx.h"
#include "Tile.h"

Tile::Tile(std::pair<int, int> location, int index) : sf::RectangleShape(), Node(location, index) {}

bool operator==(const Tile& lhs, const Tile& rhs) {
	return lhs.location == rhs.location;
}
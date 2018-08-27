#include "stdafx.h"
#include "Tile.h"

Tile::Tile(std::pair<int, int> gridLoc, std::pair<float, float> worldLoc, bool filled, sf::Vector2f size) : sf::RectangleShape(size), Node(gridLoc, filled) {
	this->worldLoc = worldLoc;
}

std::pair<float, float> Tile::getWorldLoc(){
	return worldLoc;
}

bool operator==(const Tile& lhs, const Tile& rhs) {
	return lhs.gridLocation == rhs.gridLocation;
}

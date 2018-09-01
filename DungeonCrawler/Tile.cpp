#include "stdafx.h"
#include "Tile.h"

Tile::Tile(int gridX, int gridY, float worldX, float worldY, bool filled, sf::Vector2f tileSize) : sf::RectangleShape(tileSize), Node(gridX, gridY, filled) {
	this->worldX = worldX;
	this->worldY = worldY;
}

bool operator==(const Tile& lhs, const Tile& rhs) {
	return (lhs.gridX == rhs.gridX) && (lhs.gridY == rhs.gridY);
}

std::ostream& operator<<(std::ostream& os, const Tile& tile) {
	Node node = (Node)tile;
	os << "x: " << std::to_string(node.getGridX()) << ", " << "y: " << std::to_string(node.getGridY());
	return os;
}

float Tile::getWorldX() {
	return worldX;
}

float Tile::getWorldY() {
	return worldY;
}

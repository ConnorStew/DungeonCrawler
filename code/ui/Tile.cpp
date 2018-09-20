#include "stdafx.h"
#include "Tile.h"

Tile::Tile(int gridX, int gridY, float worldX, float worldY, bool filled, sf::Vector2f tileSize) : sf::RectangleShape(tileSize), Node(gridX, gridY, worldX, worldY, filled) {
	this->size = tileSize;
}

bool operator==(const Tile& lhs, const Tile& rhs) {
	return (lhs.gridX == rhs.gridX) && (lhs.gridY == rhs.gridY);
}

std::ostream& operator<<(std::ostream& os, const Tile& tile) {
	Node node = (Node)tile;
	os << "x: " << std::to_string(node.getGridX()) << ", " << "y: " << std::to_string(node.getGridY());
	return os;
}

sf::Vector2f Tile::getSize() {
	return size;
}
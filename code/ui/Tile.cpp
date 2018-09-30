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
	os << "grid x: " << std::to_string(node.getGridX()) << ", " << "grid y: " << std::to_string(node.getGridY())
	<< " world x:" << std::to_string(node.getWorldX()) << ", " << "world y:" << std::to_string(node.getWorldY());
	return os;
}

sf::Vector2f Tile::getSize() {
	return size;
}
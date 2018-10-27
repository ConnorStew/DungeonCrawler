#include "stdafx.h"
#include "Tile.h"

const sf::Color Tile::DEFAULT_COLOR = sf::Color(100, 100, 100, 255);
const sf::Color Tile::FILLED_COLOR = sf::Color::White;

Tile::Tile(int gridX, int gridY, float worldX, float worldY, bool filled, sf::Vector2f tileSize) : sf::RectangleShape(tileSize), Node(gridX, gridY, worldX, worldY, filled) {
	this->size = tileSize;
	setFillColor(Tile::DEFAULT_COLOR);
	if (filled)
		fill();

	//draw the border
	border = sf::RectangleShape(tileSize);
	border.setFillColor(sf::Color::Transparent);
	border.setOutlineColor(sf::Color::Black);
	border.setOutlineThickness(2);
	border.setPosition(sf::Vector2f(worldX, worldY));
}

sf::RectangleShape& Tile::getBorder() {
	return border;
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

void Tile::fill() {
	setFilled(true);
	setFillColor(Tile::FILLED_COLOR);
}

void Tile::clear() {
	setFilled(false);
	setFillColor(Tile::DEFAULT_COLOR);
}
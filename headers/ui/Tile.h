#pragma once
#ifndef TILE_H
#define TILE_H

#include "stdafx.h"
#include "Node.h"

class Tile : public sf::RectangleShape, public Node {
	public:
		Tile(int gridX, int gridY, float worldX, float worldY, bool filled, sf::Vector2f tileSize);
		
		friend bool operator==(const Tile& lhs, const Tile& rhs);
		friend std::ostream& operator<<(std::ostream& os, const Tile& dt);
};

#endif
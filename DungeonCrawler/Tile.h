#pragma once
#ifndef TILE_H
#define TILE_H

#include "stdafx.h"
#include "Node.h"

class Tile : public sf::RectangleShape, public Node {
	public:
		Tile(std::pair<int,int> gridLoc, std::pair<float, float> worldLoc, bool filled, sf::Vector2f tileSize);
		friend bool operator==(const Tile& lhs, const Tile& rhs);
		std::pair<float, float> getWorldLoc();
	private:
		std::pair<float, float> worldLoc;
};

#endif
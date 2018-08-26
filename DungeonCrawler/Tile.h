#pragma once
#ifndef TILE_H
#define TILE_H

#include "stdafx.h"
#include "Node.h"

class Tile : public sf::RectangleShape, public Node {
	public:
		Tile(std::pair<int, int> location, int index);
		friend bool operator==(const Tile& lhs, const Tile& rhs);
};

#endif
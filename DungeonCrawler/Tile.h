#pragma once
#ifndef TILE_H
#define TILE_H

#include "Key.h"
#include "Node.h"

class Tile : public Node, public sf::CircleShape {
	private:
		float width, height, worldX, worldY;
		Key* key;
		bool filled;
		
	public:
		Tile(float graphX, float graphY);

};

#endif
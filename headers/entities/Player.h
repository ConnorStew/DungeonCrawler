#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "Entity.h"
#include "Tile.h"

using std::string;
using std::vector;
using std::shared_ptr;

class Player : public Entity {
public:
	Player(string spriteLocation, string friendlyName, int gridX, int gridY, int width, int height, TileMap* map);
	void update();
private:
	void moveGrid(int xIncrease, int yIncrease);
	void move(int xIncrease, int yIncrease);
	void moveEntity(sf::Vector2f pos);
	shared_ptr<Tile> colliding();
	enum Direction {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};
	
};

#endif
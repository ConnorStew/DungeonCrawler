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
	Player(string spriteLocation, int gridX, int gridY, int width, int height, Graph<Tile>& graph);
	void updateControls();
private:
	void move(int xIncrease, int yIncrease);
};

#endif
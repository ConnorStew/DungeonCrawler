#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "stdafx.h"
#include "Entity.h"
#include "Tile.h"

using std::string;

class Player : public Entity {
public:
	Player(string spriteLocation, int gridX, int gridY, int width, int height, Graph<Tile>& graph);
	void updateControls();

private:
	Graph<Tile>& graph;

	void move(int xIncrease, int yIncrease);
};

#endif
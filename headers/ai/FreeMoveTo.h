#pragma once
#ifndef FREE_MOVE_TO_H
#define FREE_MOVE_TO_H

#include "stdafx.h"
#include "Routine.h"
#include "Tile.h"

class FreeMoveTo : public Routine {
private:
	vector<shared_ptr<Tile>> pathList;
	int destX;
	int destY;

public:
	FreeMoveTo(int destX, int destY);

	void act(Entity * entity);
	std::string getName();
};

#endif
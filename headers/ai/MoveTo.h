#pragma once
#ifndef MOVETO_H
#define MOVETO_H

#include "stdafx.h"
#include "Routine.h"
#include "Tile.h"

class MoveTo : public Routine {
private:
	vector<shared_ptr<Tile>> pathList;
	int destX;
	int destY;

public:
	MoveTo(int destX, int destY);

	void act(Entity * entity);
	std::string getName();
};

#endif
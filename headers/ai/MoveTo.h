#pragma once
#ifndef MOVETO_H
#define MOVETO_H

#include "stdafx.h"
#include "Routine.h"
#include "Tile.h"

class MoveTo : public Routine {
private:
	vector<sf::Vector2f> pathList;
	sf::Vector2f target;

public:
	MoveTo(sf::Vector2f target);

	void act(Entity * entity);
	std::string getName();
};

#endif
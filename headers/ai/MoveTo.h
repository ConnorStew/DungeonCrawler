#pragma once
#ifndef MOVETO_H
#define MOVETO_H

#include "stdafx.h"
#include "Routine.h"
#include "Tile.h"
#include "World.h"

class MoveTo : public Routine {
private:
	vector<sf::Vector2f> pathList;
	sf::Vector2f target;
	Entity* targetEntity = nullptr;
	World* world = nullptr;

public:
	MoveTo(sf::Vector2f target);
	MoveTo(Entity* targetEntity);
	void act(Entity *entity);
	std::string getName();
};

#endif
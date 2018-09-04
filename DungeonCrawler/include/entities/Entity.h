#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "stdafx.h"

class Entity : public sf::Sprite {
private:
	int gridX;
	int gridY;

public:
	Entity(int gridX, int gridY);
	int getGridX();
	int getGridY();
	void setGridX(int gridX);
	void setGridY(int gridY);
};

#endif
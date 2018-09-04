#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "stdafx.h"

using std::string;

class Entity : public sf::Sprite {
protected:
	int gridX;
	int gridY;
	string spriteLocation;
	sf::Texture texture;

public:
	Entity(string spriteLocation, int gridX, int gridY, int width, int height);
	int getGridX();
	int getGridY();
	void setGridX(int gridX);
	void setGridY(int gridY);
};

#endif
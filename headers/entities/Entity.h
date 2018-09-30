#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "stdafx.h"
#include "Tile.h"
#include "Routine.h"
#include "TileMap.h"

using std::string;
using std::vector;
using std::shared_ptr;

class Entity : public sf::Sprite {
protected:
	int gridX;
	int gridY;
	string spriteLocation;
	sf::Texture texture;
	TileMap* map;
	bool pathing;
	Routine* routine;
	string friendlyName;
	sf::RectangleShape boundingBox;
	int bbXOffset;
	int bbYOffset;
	int width;
	int height;

public:
	Entity(string spriteLocation, string friendlyName, int gridX, int gridY, int width, int height, TileMap* map);
	int getGridX();
	int getGridY();
	sf::RectangleShape getBoundingBox();
	sf::Vector2f getCenter();
	void setGridX(int gridX);
	void setGridY(int gridY);
	void setRoutine(Routine* routine);
	void update();

	TileMap* getMap();
	string getFriendlyName();
	~Entity();
};

#endif
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

class World;

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
	int moveSpeed;

	void moveEntity(sf::Vector2f pos);
	shared_ptr<Tile> colliding(sf::Vector2f increase);

public:
	Entity(string spriteLocation, string friendlyName, int gridX, int gridY, int width, int height, TileMap* map, int moveSpeed);
	int getGridX();
	int getGridY();
	sf::RectangleShape getBoundingBox();
	sf::Vector2f getCenter();
	void setGridX(int gridX);
	void setGridY(int gridY);
	void setRoutine(Routine* routine);
	void update();
	void move(float xIncrease, float yIncrease);
	void moveTowards(sf::Vector2f targetPos);

	TileMap* getMap();
	string getFriendlyName();
	int getMoveSpeed();
	~Entity();

private:

	enum Direction {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};
};

#endif
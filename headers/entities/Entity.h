#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "stdafx.h"
#include "Tile.h"
#include "Routine.h"

using std::string;
using std::vector;
using std::shared_ptr;

class Entity : public sf::Sprite {
protected:
	int gridX;
	int gridY;
	string spriteLocation;
	sf::Texture texture;
	Graph<Tile>* graph;
	bool pathing;
	Routine* routine;
	string friendlyName;
public:
	Entity(string spriteLocation, string friendlyName, int gridX, int gridY, int width, int height, Graph<Tile>* graph);
	int getGridX();
	int getGridY();
	void setGridX(int gridX);
	void setGridY(int gridY);
	void setRoutine(Routine* routine);
	void update();

	Graph<Tile>* getGraph();
	string getFriendlyName();
};

#endif
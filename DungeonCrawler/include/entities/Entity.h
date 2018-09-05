#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "stdafx.h"
#include "Tile.h"

using std::string;

class Entity : public sf::Sprite {
protected:
	int gridX;
	int gridY;
	string spriteLocation;
	sf::Texture texture;
	Graph<Tile>& graph;
	bool pathing;
	vector<shared_ptr<Tile>> pathList;

public:
	Entity(string spriteLocation, int gridX, int gridY, int width, int height, Graph<Tile>& graph);
	int getGridX();
	int getGridY();
	void setGridX(int gridX);
	void setGridY(int gridY);

	void path(vector<shared_ptr<Tile>> path);
	bool isPathing();
	void advancePath();
};

#endif
#include "stdafx.h"
#include "Entity.h"

using std::string;

Entity::Entity(string spriteLocation, string friendlyName, int gridX, int gridY, int width, int height, Graph<Tile> * graph) {
	this->spriteLocation = spriteLocation;
	this->gridX = gridX;
	this->gridY = gridY;
	this->friendlyName = friendlyName;
	this->graph = graph;

	if (!texture.loadFromFile(spriteLocation))
		std::cout << "Can't load sprite at " << spriteLocation << std::endl;

	setTexture(texture);
	setScale(width / getLocalBounds().width, height / getLocalBounds().height);

	shared_ptr<Tile> entityTile = graph->at(gridX, gridY);
	setPosition(entityTile->getWorldX(), entityTile->getWorldY());
}

int Entity::getGridX() {
	return gridX;
}

int Entity::getGridY() {
	return gridY;
}

void Entity::setGridX(int gridX) {
	this->gridX = gridX;
}

void Entity::setGridY(int gridY) {
	this->gridY = gridY;
}

void Entity::setRoutine(Routine* routine) {
	this->routine = routine;
}

Graph<Tile>* Entity::getGraph() {
	return graph;
}

void Entity::update() {
	if (routine != nullptr)
		routine->act(this);

	shared_ptr<Tile> entityTile = graph->at(gridX,gridY);
	setPosition(entityTile->getWorldX(), entityTile->getWorldY());
}

string Entity::getFriendlyName() {
	return friendlyName;
}
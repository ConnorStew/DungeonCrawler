#include "stdafx.h"
#include "Entity.h"

Entity::Entity(int gridX, int gridY) {
	this->gridX = gridX;
	this->gridY = gridY;
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

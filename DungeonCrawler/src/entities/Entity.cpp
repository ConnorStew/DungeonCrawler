#include "stdafx.h"
#include "Entity.h"

using std::string;

Entity::Entity(string spriteLocation, int gridX, int gridY, int width, int height) {
	this->spriteLocation = spriteLocation;
	this->gridX = gridX;
	this->gridY = gridY;

	if (!texture.loadFromFile(spriteLocation))
		std::cout << "Can't load sprite at " << spriteLocation << std::endl;

	setTexture(texture);
	setScale(width / getLocalBounds().width, height / getLocalBounds().height);
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

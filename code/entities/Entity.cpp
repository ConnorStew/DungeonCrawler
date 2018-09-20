#include "stdafx.h"
#include "Entity.h"
#include "TileMap.h"

using std::string;

Entity::Entity(string spriteLocation, string friendlyName, int gridX, int gridY, int width, int height, TileMap* map) {
	this->spriteLocation = spriteLocation;
	this->gridX = gridX;
	this->gridY = gridY;
	this->friendlyName = friendlyName;
	this->map = map;

	//const float percentDifference = 0.80;
	const float percentDifference = 1.0;
	float percentWidth = (width * percentDifference);
	float percentHeight = (height * percentDifference);
	bbXOffset = width - percentWidth;
	bbYOffset = height - percentHeight;

	if (!texture.loadFromFile(spriteLocation))
		std::cout << "Can't load sprite at " << spriteLocation << std::endl;

	setTexture(texture);
	setScale(width / getLocalBounds().width, height / getLocalBounds().height);

	shared_ptr<Tile> entityTile = map->at(gridX, gridY);
	setPosition(entityTile->getWorldX(), entityTile->getWorldY());

	boundingBox = sf::RectangleShape(sf::Vector2f(percentWidth,percentHeight));
	boundingBox.setPosition(getPosition().x + bbXOffset / 2, getPosition().y + bbYOffset / 2);
	boundingBox.setFillColor(sf::Color::Red);
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

TileMap* Entity::getMap() {
	return map;
}

void Entity::update() {
	if (routine != nullptr) {
		if (routine->getState() == Routine::SUCCESS || routine->getState() == Routine::FAILURE) {
			delete routine;
		} else {
			routine->act(this);
		}		
	}
}

string Entity::getFriendlyName() {
	return friendlyName;
}

sf::RectangleShape Entity::getBoundingBox() {
	return boundingBox;
}

Entity::~Entity() {
	if (routine != nullptr)
		delete routine;
}
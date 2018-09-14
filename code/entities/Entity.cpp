#include "stdafx.h"
#include "Entity.h"

using std::string;

Entity::Entity(string spriteLocation, int gridX, int gridY, int width, int height, Graph<Tile>& graph) : graph(graph) {
	this->spriteLocation = spriteLocation;
	this->gridX = gridX;
	this->gridY = gridY;

	if (!texture.loadFromFile(spriteLocation))
		std::cout << "Can't load sprite at " << spriteLocation << std::endl;

	setTexture(texture);
	setScale(width / getLocalBounds().width, height / getLocalBounds().height);

	shared_ptr<Tile> entityTile = graph[gridX][gridY];
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

void Entity::setRoutine(Routine * routine) {
	this->routine = routine;
}

bool Entity::isPathing() {
	return pathing;
}

void Entity::advancePath() {
	//if (!pathList.empty()) {
	//	shared_ptr<Tile> tile = pathList.back();
	//	pathList.pop_back();
	//	std::cout << "Adding pathing to " << *tile << std::endl;
	//	gridX = tile->getGridX();
	//	gridY = tile->getGridY();
	//	setPosition(tile->getWorldX(), tile->getWorldY());
	//} else {
	//	pathing = false;
	//}
}

Graph<Tile> Entity::getGraph() {
	return graph;
}

void Entity::path(vector<shared_ptr<Tile>> newPath) {
	//if (!newPath.empty()) {
	//	std::cout << "Adding new path." << std::endl;
	//	pathing = true;
	//	pathList = newPath;
	//}
}

void Entity::update() {
	if (routine != nullptr) {
		//if (routine->getState() == NULL)
		//	routine->setState(Routine::RUNNING);

		routine->act(*this);
	}


	shared_ptr<Tile> entityTile = graph[gridX][gridY];
	setPosition(entityTile->getWorldX(), entityTile->getWorldY());
}
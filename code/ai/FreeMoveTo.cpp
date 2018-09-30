#include "stdafx.h"
#include "FreeMoveTo.h"
#include "TileMap.h"
#include "Tile.h"
#include "Entity.h"

FreeMoveTo::FreeMoveTo(int destX, int destY) {
	//state = RUNNING;
	this->destX = destX;
	this->destY = destY;
}

void FreeMoveTo::act(Entity * entity) {
	if (state == UNINITIALISED) {
		state = RUNNING;

		TileMap* map = entity->getMap();

        shared_ptr<Tile> tile = map->findNode(entity->getCenter());

		int entityX = tile->getGridX();
		int entityY = tile->getGridY();

		pathList = map->aStar(entityX, entityY, destX, destY);
		std::cout << entity->getFriendlyName() << ": beginning pathing to " << destX << ", " << destY << std::endl;
		if (pathList.empty()) {
			std::cout << entity->getFriendlyName() << ":Cannot find path to " << destX << ", " << destY << std::endl;
			state = FAILURE;
		}
	}

	if (state == RUNNING) {
		shared_ptr<Tile> tile = pathList.back();
		pathList.pop_back();

		//std::cout << "\t" << tile->getGridX() << ", " << tile->getGridY() << std::endl;

		entity->setGridX(tile->getGridX());
		entity->setGridY(tile->getGridY());
		entity->setPosition(tile->getWorldX(), tile->getWorldY());
		if (tile->getGridX() == destX && tile->getGridY() == destY) {
			std::cout << entity->getFriendlyName() << ": made it to " << destX << ", " << destY << std::endl;
			state = SUCCESS;
		} else if (pathList.empty()) {
			state = FAILURE;
		}
	}

}	

std::string FreeMoveTo::getName() {
    return "FreeMoveTo (" + std::to_string(destX) + ")" + "(" + std::to_string(destY) + ")";
}
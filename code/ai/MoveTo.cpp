#include "stdafx.h"
#include "MoveTo.h"
#include "Graph.h"
#include "Tile.h"
#include "Entity.h"

MoveTo::MoveTo(int destX, int destY) {
	//state = RUNNING;
	this->destX = destX;
	this->destY = destY;
}

void MoveTo::act(Entity * entity) {
	if (state == UNINITIALISED) {
		state = RUNNING;

		Graph<Tile>* graph = entity->getGraph();
		int entityX = entity->getGridX();
		int entityY = entity->getGridY();

		pathList = graph->aStar(entityX, entityY, destX, destY);
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

std::string MoveTo::getName() {
    return "MoveTo (" + std::to_string(destX) + ")" + "(" + std::to_string(destY) + ")";
}
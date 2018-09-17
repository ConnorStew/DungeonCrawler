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
	std::cout << "In move to" << std::endl;

	if (state != RUNNING) {
		state = RUNNING;

		std::cout << "not running in move to" << std::endl;

		

		Graph<Tile>* graph = entity->getGraph();
		std::cout << "past graph" << std::endl;
		int entityX = entity->getGridX();
		int entityY = entity->getGridY();


		if (graph == nullptr) {
			std::cout << "null graph" << std::endl;
		}

		pathList = graph->aStar(entityX, entityY, destX, destY);
		std::cout << entity->getFriendlyName() << ": beginning pathing to " << destX << ", " << destY << ";" << std::endl;
		std::cout << "past entity" << std::endl;
		if (pathList.empty()) {
			std::cout << entity->getFriendlyName() << ":Cannot find path to " << destX << ", " << destY << std::endl;
			state = FAILURE;
		}
	}

	if (state == RUNNING) {
		std::cout << "running in move to" << std::endl;
		shared_ptr<Tile> tile = pathList.back();
		pathList.pop_back();

		std::cout << "\t" << tile->getGridX() << ", " << tile->getGridY() << std::endl;

		std::cout << "running in move to - entity" << std::endl;
		if (tile.get() == nullptr) {
			std::cout << "null tile" << std::endl;
		}
		if (entity == nullptr) {
			std::cout << "null entity" << std::endl;
		}
		//seg fault error is here -----
		entity->setGridX(tile->getGridX());
		std::cout << "set grid x" << std::endl;
		entity->setGridY(tile->getGridY());
		entity->setPosition(tile->getWorldX(), tile->getWorldY());
		std::cout << "running in move to - entity done" << std::endl;
		if (tile->getGridX() == destX && tile->getGridY() == destY) {
			state = SUCCESS;
		} else if (pathList.empty()) {
			state = FAILURE;
		}
	}

}	

std::string MoveTo::getName() {
    return "MoveTo (" + std::to_string(destX) + ")" + "(" + std::to_string(destY) + ")";
}
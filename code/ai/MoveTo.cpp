#include "stdafx.h"
#include "MoveTo.h"
#include "Graph.h"
#include "Tile.h"
#include "Entity.h"

MoveTo::MoveTo(int destX, int destY) {
	this->destX = destX;
	this->destY = destY;
}

void MoveTo::act(Entity &entity) {
	if (state != RUNNING) {
		state = RUNNING;

		Graph<Tile> graph = entity.getGraph();
		pathList = graph.aStar(entity.getGridX(), entity.getGridY(), destX, destY);
		std::cout << "Adding new path." << std::endl;

		if (pathList.empty()) {
			std::cout << "No path avialible." << std::endl;
			state = FAILURE;
		}
	}

	if (state == RUNNING) {
		shared_ptr<Tile> tile = pathList.back();
		pathList.pop_back();

		std::cout << "Adding pathing to " << *tile << std::endl;

		entity.setGridX(tile->getGridX());
		entity.setGridY(tile->getGridY());
		entity.setPosition(tile->getWorldX(), tile->getWorldY());

		if (tile->getGridX() == destX && tile->getGridY() == destY) {
			state = SUCCESS;
		}
	}
}
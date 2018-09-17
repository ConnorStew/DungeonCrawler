#include "stdafx.h"
#include "Wander.h"
#include "Entity.h"
#include "Graph.h"
#include "Tile.h"

using std::shared_ptr;

void Wander::validateMove(Entity * entity, int xIncrease, int yIncrease) {
	Graph<Tile>* graph = entity->getGraph();
	int targetX = entity->getGridX() + xIncrease;
	int targetY = entity->getGridY() + yIncrease;
	shared_ptr<Tile> targetTile = graph->at(targetX, targetY);

	if (targetTile != nullptr && !targetTile->getFilled()) {
		entity->setGridX(targetX);
		entity->setGridY(targetY);
	}
}

Wander::Wander() {
	state = RUNNING;
	rng.seed(std::random_device()());
	dist6 = std::uniform_int_distribution<std::mt19937::result_type>(0, 3);
}

/// <summary>
/// RNG stuff from : https://stackoverflow.com/questions/13445688/how-to-generate-a-random-number-in-c
/// </summary>
/// <param name="entity"></param>
void Wander::act(Entity * entity) {
	if (state == RUNNING) {
		int direction = dist6(rng);
		switch (direction) {
			case 0: //left
				validateMove(entity, -1, 0);
				break;
			case 1: //right
				validateMove(entity, 1, 0);
				break;
			case 2: //up
				validateMove(entity, 0, 1);
				break;
			case 3: //down
				validateMove(entity, 0, -1);
				break;
		}
	}
}

std::string Wander::getName() {
    return "Wander";
}
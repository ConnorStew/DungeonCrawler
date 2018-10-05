#include "stdafx.h"
#include "MoveTo.h"
#include "TileMap.h"
#include "Tile.h"
#include "Entity.h"

using std::endl;
using std::cout;

MoveTo::MoveTo(sf::Vector2f target) {
	this->target = target;
}

void MoveTo::act(Entity * entity) {
	if (state == UNINITIALISED) {
		state = RUNNING;

		TileMap* map = entity->getMap();
		pathList = map->aStar(entity->getPosition(), target);
		cout << entity->getFriendlyName() << ": beginning pathing to " << target.x << ", " << target.y << endl;
		if (pathList.empty()) {
			cout << entity->getFriendlyName() << ":Cannot find path to " << target.x << ", " << target.y << endl;
			state = FAILURE;
		}
	}

	if (state == RUNNING) {
		if (pathList.empty()) {
			cout << "Reached position!" << endl;
			state = SUCCESS;
			return;
		}

		sf::Vector2f targetPos = pathList.back();
		cout << entity->getFriendlyName() << ": moving towards " << targetPos.x << ", " << targetPos.y << endl;

		entity->moveTowards(targetPos);

		if (entity->getPosition() == targetPos) {
			cout << entity->getFriendlyName() << ": reached" << targetPos.x << ", " << targetPos.y << endl;
			pathList.pop_back();
		}
	}

}	

std::string MoveTo::getName() {
    return "MoveTo (" + std::to_string(target.x) + ")" + "(" + std::to_string(target.y) + ")";
}
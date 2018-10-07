#include "stdafx.h"
#include "MoveTo.h"
#include "TileMap.h"
#include "Tile.h"
#include "Entity.h"
#include "World.h"

using std::endl;
using std::cout;

MoveTo::MoveTo(sf::Vector2f target) {
	this->target = target;
	this->targetEntity = nullptr;
}

MoveTo::MoveTo(Entity * targetEntity) {
	this->targetEntity = targetEntity;
	this->target = sf::Vector2f(0,0);
}

void MoveTo::act(Entity * entity) {

	//cout << entity->getFriendlyName() << "target: " << target.x << ", " << target.y << endl;
	//cout << entity->getFriendlyName() << "entity: " << targetEntity->getPosition().x << ", " << targetEntity->getPosition().y << endl;

	if (targetEntity != nullptr && (targetEntity->getPosition().x != target.x || targetEntity->getPosition().y != target.y)) {
		state = UNINITIALISED;
		target = targetEntity->getPosition();
	}

	if (state == UNINITIALISED) {
		state = RUNNING;

		TileMap* map = entity->getMap();
		pathList = map->aStar(entity->getPosition(), target);
		//cout << entity->getFriendlyName() << ": beginning pathing to " << target.x << ", " << target.y << endl;
		if (pathList.empty()) {
			//cout << entity->getFriendlyName() << ":Cannot find path to " << target.x << ", " << target.y << endl;
			state = UNINITIALISED;
		}
	}

	if (state == RUNNING) {
		if (pathList.empty()) {
			//cout << "Reached position!" << endl;
			//state = SUCCESS;
			return;
		}

		sf::Vector2f targetPos = pathList.back();
		//cout << "path list size: " << pathList.size() << endl;
		//cout << entity->getFriendlyName() << ": moving towards " << targetPos.x << ", " << targetPos.y << endl;
		entity->moveTowards(targetPos);

		if (entity->getPosition() == targetPos) {
			//cout << entity->getFriendlyName() << ": at" << entity->getPosition().x << ", " << entity->getPosition().y << endl;
			//cout << entity->getFriendlyName() << ": reached" << targetPos.x << ", " << targetPos.y << endl;
			//out << "path list size before: " << pathList.size() << endl;
			pathList.pop_back();
			//cout << "path list size after: " << pathList.size() << endl;
		}

		//cout << endl;
	}

}	

std::string MoveTo::getName() {
    return "MoveTo (" + std::to_string(target.x) + ")" + "(" + std::to_string(target.y) + ")";
}
#include "stdafx.h"
#include "Entity.h"
#include "TileMap.h"
#include "World.h"

using std::string;

Entity::Entity(string spriteLocation, string friendlyName, int gridX, int gridY, int width, int height, TileMap* map, int moveSpeed) {
	this->moveSpeed = moveSpeed;
	this->spriteLocation = spriteLocation;
	this->gridX = gridX;
	this->gridY = gridY;
	this->friendlyName = friendlyName;
	this->map = map;
	this->width = width;
	this->height = height;
	this->routine = nullptr;

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

void Entity::moveEntity(sf::Vector2f pos) {
	setPosition(pos);
	boundingBox.setPosition(pos.x + bbXOffset / 2, pos.y + bbYOffset / 2);
}

shared_ptr<Tile> Entity::colliding(sf::Vector2f increase) {
	// for (const auto& entry : map->getNodes()) {
	// 	shared_ptr<Tile> tile = entry.second;
	// 	if (tile->getFilled()) {
	// 		if (tile->getGlobalBounds().intersects(boundingBox.getGlobalBounds())) {
	// 			return tile;
	// 		}
	// 	}
	// }
	sf::FloatRect rect = sf::FloatRect(sf::Vector2f(getPosition().x + increase.x, getPosition().y + increase.y), boundingBox.getSize());
	vector<shared_ptr<Tile>> tiles = map->getSurroundingNodes(getCenter());
	for (std::shared_ptr<Tile> tile : tiles) {
		if (tile->getFilled()) {
			if (tile->getGlobalBounds().intersects(rect)) {
				return tile;
			}
		}
	}

	return nullptr;
}

void Entity::moveTowards(sf::Vector2f targetPos) {
	sf::Vector2f entityPos = getPosition();
	float xIncrease = 0;
	float yIncrease = 0;
	float xDiff = targetPos.x - entityPos.x;
	float yDiff = targetPos.y - entityPos.y;

	if (xDiff != 0) {
		xIncrease = moveSpeed;
	}

	if (yDiff != 0) {
		yIncrease = moveSpeed;
	}

	if (xDiff < 0) {
		xIncrease = -xIncrease;
	}

	if (yDiff < 0) {
		yIncrease = -yIncrease;
	}

	if (yDiff > 0 && yDiff < moveSpeed) {
		yIncrease = yDiff;
	}

	if (yDiff < 0 && yDiff > -moveSpeed) {
		yIncrease = yDiff;
	}

	if (xDiff > 0 && xDiff < moveSpeed) {
		xIncrease = xDiff;
	}

	if (xDiff < 0 && xDiff > -moveSpeed) {
		xIncrease = xDiff;
	}

	move(xIncrease, yIncrease);
}

//https://gamedev.stackexchange.com/questions/69339/2d-aabbs-and-resolving-multiple-collisions
void Entity::move(float xIncrease, float yIncrease) {
	//cout << "moving with x/y :" << xIncrease << ", " << yIncrease << endl;

	shared_ptr<Tile> collidedTile = colliding(sf::Vector2f(xIncrease, 0));

	//if moving the x axis caused a collision
	if (collidedTile != nullptr) {
		float rectX = getBoundingBox().getPosition().x;
		float tileX = collidedTile->getWorldX();
		float tileWidth = collidedTile->getSize().x;

		//move along x axis
		if (rectX < tileX) { //right collision
			//cout << "right collision!" << endl;
			moveEntity(sf::Vector2f(collidedTile->getPosition().x - width, getPosition().y));
		} else if (rectX > tileX) { //left collision
			//cout << "left collision!" << endl;
			moveEntity(sf::Vector2f(collidedTile->getPosition().x + tileWidth, getPosition().y));
		}

		//collidedTile->setFillColor(sf::Color::Blue);
	} else {
		moveEntity(sf::Vector2f(getPosition().x + xIncrease, getPosition().y));
	}


	shared_ptr<Tile> collidedTileY = colliding(sf::Vector2f(0, yIncrease));

	//if moving the x axis caused a collision
	if (collidedTileY != nullptr) {
		float rectY = getBoundingBox().getPosition().y;
		float tileY = collidedTileY->getWorldY();
		float tileHeight = collidedTileY->getSize().y;

		//move along y axis
		if (rectY < tileY) { //top collision
			//cout << "top collision!" << endl;
			moveEntity(sf::Vector2f(getPosition().x, collidedTileY->getPosition().y - height));
		} else if (rectY > tileY) { //bottom collision
			//cout << "bottom collision!" << endl;
			moveEntity(sf::Vector2f(getPosition().x, collidedTileY->getPosition().y + tileHeight));
		}
		
		//collidedTileY->setFillColor(sf::Color::Magenta);
	} else {
		moveEntity(sf::Vector2f(getPosition().x, getPosition().y + yIncrease));
	}
}

int Entity::getMoveSpeed() {
	return moveSpeed;
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
		} else if (routine->getState() == Routine::UNINITIALISED || routine->getState() == Routine::RUNNING) {
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

sf::Vector2f Entity::getCenter() {
	sf::Vector2f position = getPosition();
	return sf::Vector2f(position.x + width / 2, position.y + height / 2);
}

Entity::~Entity() {
	if (routine != nullptr)
		delete routine;
}
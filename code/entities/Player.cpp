#include "stdafx.h"
#include "Player.h"
#include "Tile.h"

using std::shared_ptr;
using std::vector;



Player::Player(string spriteLocation, string friendlyName, int gridX, int gridY, int width, int height, Graph<Tile>* graph) : Entity(spriteLocation, friendlyName, gridX, gridY, width, height, graph) {
	pathing = false;
}

void Player::moveEntity(sf::Vector2f pos) {
	setPosition(pos);
	boundingBox.setPosition(pos.x + bbXOffset / 2, pos.y + bbYOffset / 2);
}

void Player::moveGrid(int xIncrease, int yIncrease) {
	shared_ptr<Tile> tile = graph->at(gridX + xIncrease,gridY + yIncrease);
	if (tile != nullptr && !tile->getFilled()) {
		gridX = gridX + xIncrease;
		gridY = gridY + yIncrease;

		shared_ptr<Tile> entityTile = graph->at(gridX, gridY);
		moveEntity(entityTile->getPosition());
	}
}

//https://gamedev.stackexchange.com/questions/69339/2d-aabbs-and-resolving-multiple-collisions
void Player::move(int xIncrease, int yIncrease) {
	//move along x axis
	moveEntity(sf::Vector2f(getPosition().x + xIncrease, getPosition().y));
	shared_ptr<Tile> collidedTile = colliding();

	//if moving the x axis caused a collision
	if (collidedTile != nullptr) {
		float rectX = getBoundingBox().getPosition().x;
		float tileX = collidedTile->getWorldX();
		float tileWidth = collidedTile->getSize().x;

		//move along x axis
		if (rectX < tileX) { //right collision
			cout << "right collision!" << endl;
			moveEntity(sf::Vector2f(collidedTile->getPosition().x - tileWidth, getPosition().y));
		} else if (rectX > tileX) { //left collision
			cout << "left collision!" << endl;
			moveEntity(sf::Vector2f(collidedTile->getPosition().x + tileWidth, getPosition().y));
		}

		collidedTile->setFillColor(sf::Color::Blue);
	}


	//move along y axis
	moveEntity(sf::Vector2f(getPosition().x, getPosition().y + yIncrease));
	shared_ptr<Tile> collidedTileY = colliding();

	//if moving the x axis caused a collision
	if (collidedTileY != nullptr) {
		float rectY = getBoundingBox().getPosition().y;
		float tileY = collidedTileY->getWorldY();
		float tileHeight = collidedTileY->getSize().y;

		//move along y axis
		if (rectY < tileY) { //top collision
			cout << "top collision!" << endl;
			moveEntity(sf::Vector2f(getPosition().x, collidedTileY->getPosition().y - tileHeight));
		} else if (rectY > tileY) { //bottom collision
			cout << "bottom collision!" << endl;
			moveEntity(sf::Vector2f(getPosition().x, collidedTileY->getPosition().y + tileHeight));
		}
		
		collidedTileY->setFillColor(sf::Color::Magenta);
	}
}

void Player::update() {
	Entity::update();
	const int MOVE_AMOUNT = 10;
	bool wDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
	bool aDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
	bool sDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
	bool dDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
	bool controlDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);

	//player controls
	if (wDown)
		move(0, -MOVE_AMOUNT);

	if (sDown && !controlDown)
		move(0, MOVE_AMOUNT);

	if (aDown)
		move(-MOVE_AMOUNT, 0);

	if (dDown)
		move(MOVE_AMOUNT, 0);
}

shared_ptr<Tile> Player::colliding() {
	for (const auto& entry : graph->getNodes()) {
		shared_ptr<Tile> tile = entry.second;
		if (tile->getFilled()) {
			if (tile->getGlobalBounds().intersects(boundingBox.getGlobalBounds())) {
				return tile;
			}
		}
	}

	return nullptr;
}
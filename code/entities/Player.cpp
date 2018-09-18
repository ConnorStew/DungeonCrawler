#include "stdafx.h"
#include "Player.h"
#include "Tile.h"

using std::shared_ptr;
using std::vector;

Player::Player(string spriteLocation, string friendlyName, int gridX, int gridY, int width, int height, Graph<Tile>* graph) : Entity(spriteLocation, friendlyName, gridX, gridY, width, height, graph) {
	pathing = false;
}

void Player::moveGrid(int xIncrease, int yIncrease) {
	shared_ptr<Tile> tile = graph->at(gridX + xIncrease,gridY + yIncrease);
	if (tile != nullptr && !tile->getFilled()) {
		gridX = gridX + xIncrease;
		gridY = gridY + yIncrease;

		shared_ptr<Tile> entityTile = graph->at(gridX, gridY);
		setPosition(entityTile->getWorldX(), entityTile->getWorldY());
	}
}

void Player::move(int xIncrease, int yIncrease) {
	gridX = gridX + xIncrease;
	gridY = gridY + yIncrease;
	setPosition(gridX, gridY);
}

void Player::update() {
	Entity::update();
	const int MOVE_AMOUNT = 5;
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
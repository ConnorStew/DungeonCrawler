#pragma once

#include "stdafx.h"
#include "Player.h"
#include "Tile.h"

using std::shared_ptr;
using std::vector;

Player::Player(string spriteLocation, int gridX, int gridY, int width, int height, Graph<Tile>& graph) : Entity(spriteLocation, gridX, gridY, width, height), graph(graph) {
	pathing = false;
}

void Player::updateControls() {
	bool wDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
	bool aDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
	bool sDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
	bool dDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);
	bool controlDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);

	//player controls
	if (wDown)
		move(0, -1);

	if (sDown && !controlDown)
		move(0, 1);

	if (aDown)
		move(-1, 0);

	if (dDown)
		move(1, 0);

	shared_ptr<Tile> playerTile = graph[gridX][gridY];
	setPosition(playerTile->getWorldX(), playerTile->getWorldY());
}

void Player::path(vector<shared_ptr<Tile>> pathList) {
	if (!pathList.empty()) {
		pathing = true;
		this->pathList = pathList;
	}
}

void Player::move(int xIncrease, int yIncrease) {
	shared_ptr<Tile> tile = graph[gridX + xIncrease][gridY + yIncrease];
	if (tile != nullptr && !tile->getFilled()) {
		gridX = gridX + xIncrease;
		gridY = gridY + yIncrease;
	}
}

bool Player::isPathing() {
	return pathing;
}

void Player::advancePath() {
	if (!pathList.empty()) {
		shared_ptr<Tile> tile = pathList.back();
		pathList.pop_back();

		gridX = tile->getGridX();
		gridY = tile->getGridY();
		setPosition(tile->getWorldX(), tile->getWorldY());
	} else {
		pathing = false;
	}
}
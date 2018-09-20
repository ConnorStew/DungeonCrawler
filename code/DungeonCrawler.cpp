#include "stdafx.h"
#include "Tile.h"
#include "DungeonCrawler.h"
#include "Entity.h"
#include "TileMap.h"
#include "Player.h"
#include "MoveTo.h"
#include "Sequence.h"
#include "Wander.h"
#include "Selector.h"

using std::shared_ptr;
using std::vector;

/// <summary> The games gameMap-> </summary>
TileMap* gameMap = new TileMap("res/map.json");

/// <summary> The SFML window. </summary>
sf::RenderWindow window(sf::VideoMode(gameMap->getWidth(), gameMap->getHeight()), "Dungeon Crawler");

/// <summary> Whether the gameMap is currently running the A* algoritm. </summary>
bool started = false;

/// <summary> The clock to count FPS. </summary>
sf::Clock fpsClock;

/// <summary> The clock to count FPS. </summary>
sf::Clock updateClock;

sf::Clock skeletonClock;

const bool DRAW_PATH = true;

shared_ptr<Tile> targetTile = nullptr;

Player player("res/mage.png", "Player", 5, 5, gameMap->getTileSize().x, gameMap->getTileSize().y, gameMap);
Entity skeleton("res/skeleton.png", "Skeleton", 28, 28, gameMap->getTileSize().x, gameMap->getTileSize().y, gameMap);

void update() {

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
	bool eDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
	bool qDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
	bool controlDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
	bool yDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y);
	bool sDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
	bool pDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);

	if (controlDown && sDown)
		gameMap->save();

	for (int x = 0; x < gameMap->getSize(); x++) {
		for (int y = 0; y < gameMap->getSize(); y++) {
			shared_ptr<Tile> tile = gameMap->at(x,y);

			if (eDown && tile->getGlobalBounds().contains(mousePos)) {
				tile->setFilled(true);
				gameMap->clearConnections(x, y);
			}

			if (qDown && tile->getGlobalBounds().contains(mousePos)) {
				tile->setFilled(false);
				gameMap->addConnections(x, y);
			}

			if (pDown) {
				gameMap->findNode(player.getPosition());
			}

			if (yDown && tile->getGlobalBounds().contains(mousePos)) {
				std::cout << *tile << std::endl; //dereference pointer
			}

			if (spaceDown && tile->getGlobalBounds().contains(mousePos)) {
				//draw connections
				vector<shared_ptr<Tile>> &connections = gameMap->adj(x, y);
				for (unsigned int i = 0; i < connections.size(); i++) {
					shared_ptr<Tile> connectedTile = connections.at(i);
					sf::Vertex line[] =
					{
						sf::Vertex(sf::Vector2f(tile->getWorldX() + gameMap->getTileSize().x / 2, tile->getWorldY() + gameMap->getTileSize().y / 2), sf::Color(0, 255, 221,255)),
						sf::Vertex(sf::Vector2f(connectedTile->getWorldX() + gameMap->getTileSize().x / 2, connectedTile->getWorldY() + gameMap->getTileSize().y / 2),  sf::Color(0, 255, 221,255))
					};

					window.draw(line, 2, sf::Lines);
				}
			}
		}
	}
}

void render() {

	//framerate
	//float framerate = 1.f / fpsClock.getElapsedTime().asSeconds();
	//fpsClock.restart();
	//std::cout << std::to_string(framerate) << std::endl;

	for (auto const& tileEntry : gameMap->getNodes()) {
		shared_ptr<Tile> tile = tileEntry.second;
		//tile->setFillColor(sf::Color(100, 100, 100, 255)); //default color

		// if (tile->getFilled())
		// 	tile->setFillColor(sf::Color::White);
	}

	if (DRAW_PATH) {
		for (shared_ptr<Tile> tile : gameMap->getOpenList())
			tile->setFillColor(sf::Color::Green);

		for (shared_ptr<Tile> tile : gameMap->getClosedList())
			tile->setFillColor(sf::Color::Red);

		for (shared_ptr<Tile> tile : gameMap->getPathList())
			tile->setFillColor(sf::Color::Magenta);
	}

	for (auto const& tileEntry : gameMap->getNodes())
		window.draw(*tileEntry.second);

	
	window.draw(player.getBoundingBox());
	window.draw(player);
	window.draw(skeleton);
	window.display();
}

int main() {
	updateClock.restart();
	skeletonClock.restart();

	// player.setRoutine(new Sequence {
	// 	new MoveTo(21, 25),
	// 	new MoveTo(21, 24),
	// 	new Sequence {
	// 		new MoveTo(8, 28),
	// 		new MoveTo(16, 1),
	// 		new MoveTo(2, 22)
	// 	}gameMap
	// 	//new Wander()
	// });

	// skeleton.setRoutine(new Sequence{
	// 	new MoveTo(12,12),
	// 	new Wander()
	// });

	while (window.isOpen()) {
		window.clear();
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		if (updateClock.getElapsedTime().asMilliseconds() > 50) {
			player.update();
			updateClock.restart();
		}

		if (skeletonClock.getElapsedTime().asSeconds() > 1) {
			skeleton.update();
			skeletonClock.restart();
		}
		
		update();
		render();
	}

	delete gameMap;
	
	return 0;
}
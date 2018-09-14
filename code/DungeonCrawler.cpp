#include "stdafx.h"
#include "Tile.h"
#include "DungeonCrawler.h"
#include "Entity.h"
#include "TileMap.h"
#include "Player.h"
#include "MoveTo.h"
#include "Sequence.h"
#include "Wander.h"
#include "Graph.h"

using std::shared_ptr;
using std::vector;

/// <summary> Mutex for to allow for editing vectors during multithreading. </summary>
//std::mutex mutex;

/// <summary> Graph which stores tiles. </summary>
Graph<Tile> graph;

/// <summary> The games map. </summary>
TileMap gameMap("res/map.json", graph);

/// <summary> The SFML window. </summary>
sf::RenderWindow window(sf::VideoMode(gameMap.getWidth(), gameMap.getHeight()), "Dungeon Crawler");

/// <summary> Whether the graph is currently running the A* algoritm. </summary>
bool started = false;

/// <summary> The clock to count FPS. </summary>
sf::Clock fpsClock;

/// <summary> The clock to count FPS. </summary>
sf::Clock updateClock;

sf::Clock skeletonClock;

const bool DRAW_PATH = false;

shared_ptr<Tile> targetTile = nullptr;

Player player("res/mage.png", 5, 5, gameMap.getTileSize().x, gameMap.getTileSize().y, graph);
Entity skeleton("res/skeleton.png", 28, 28, gameMap.getTileSize().x, gameMap.getTileSize().y, graph);

void update() {

	sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
	bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
	bool eDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
	bool qDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
	bool fDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);
	bool controlDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
	bool nDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N);
	bool mDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M);
	//bool enterDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
	bool yDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y);
	bool sDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
	bool leftClick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	if (updateClock.getElapsedTime().asMilliseconds() > 50) {
		if (player.isPathing()) {
			player.advancePath();
		}

		updateClock.restart();
	}

	if (controlDown && sDown)
		gameMap.save();

	for (int x = 0; x < gameMap.getSize(); x++) {
		for (int y = 0; y < gameMap.getSize(); y++) {
			shared_ptr<Tile> tile = graph[x][y];

			if (leftClick && tile->getGlobalBounds().contains(mousePos)) {
				player.path(graph.aStar(player.getGridX(), player.getGridY(), x, y));
			}

			if (eDown && tile->getGlobalBounds().contains(mousePos)) {
				tile->setFilled(true);
				graph.clearConnections(x, y);
			}

			if (qDown && tile->getGlobalBounds().contains(mousePos)) {
				tile->setFilled(false);
				graph.addConnections(x, y, gameMap.usingDiagonalMovement());
			}

			if (nDown && tile->getGlobalBounds().contains(mousePos)) {
				gameMap.setStart(tile);
			}

			if (mDown && tile->getGlobalBounds().contains(mousePos)) {
				gameMap.setEnd(tile);
			}

			if (yDown && tile->getGlobalBounds().contains(mousePos)) {
				std::cout << *tile << std::endl; //dereference pointer
			}

			// if (enterDown && !started) {
			// 	graph.aStar(gameMap.getStart()->getGridX(), gameMap.getStart()->getGridY(), gameMap.getEnd()->getGridX(), gameMap.getEnd()->getGridY());
			// 	started = true;
			// }

			if (fDown) {
				graph.clear();
				started = false;
			}

			if (spaceDown && tile->getGlobalBounds().contains(mousePos)) {
				//draw connections
				vector<shared_ptr<Tile>> &connections = graph.adj(x, y);
				for (unsigned int i = 0; i < connections.size(); i++) {
					shared_ptr<Tile> connectedTile = connections.at(i);
					sf::Vertex line[] =
					{
						sf::Vertex(sf::Vector2f(tile->getWorldX() + gameMap.getTileSize().x / 2, tile->getWorldY() + gameMap.getTileSize().y / 2), sf::Color(0, 255, 221,255)),
						sf::Vertex(sf::Vector2f(connectedTile->getWorldX() + gameMap.getTileSize().x / 2, connectedTile->getWorldY() + gameMap.getTileSize().y / 2),  sf::Color(0, 255, 221,255))
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

	for (auto const& tileEntry : graph.getNodes()) {
		shared_ptr<Tile> tile = tileEntry.second;
		tile->setFillColor(sf::Color(100, 100, 100, 255)); //default color

		if (DRAW_PATH) {
			if (tile == gameMap.getStart())
				tile->setFillColor(sf::Color::Yellow);

			if (tile == gameMap.getEnd())
				tile->setFillColor(sf::Color::Blue);
		}

		if (tile->getFilled())
			tile->setFillColor(sf::Color::White);
	}

	if (DRAW_PATH) {
		for (shared_ptr<Tile> tile : graph.getOpenList())
			if (tile != gameMap.getStart() && tile != gameMap.getEnd()) //don't cover start and end
				tile->setFillColor(sf::Color::Green);

		for (shared_ptr<Tile> tile : graph.getClosedList())
			if (tile != gameMap.getStart() && tile != gameMap.getEnd()) //don't cover start and end
				tile->setFillColor(sf::Color::Red);

		for (shared_ptr<Tile> tile : graph.getPathList())
			if (tile != gameMap.getStart() && tile != gameMap.getEnd()) //don't cover start and end
				tile->setFillColor(sf::Color::Magenta);
	}

	for (auto const& tileEntry : graph.getNodes())
		window.draw(*tileEntry.second);


	window.draw(player);
	window.draw(skeleton);
	window.display();
}

int main() {
	updateClock.restart();
	skeletonClock.restart();

	player.setRoutine(new Sequence { 
		new MoveTo(21, 25), 
		new Sequence {
			new MoveTo(8, 8),
			new MoveTo(21,28),
			new MoveTo(2,22)
		}
	});

	skeleton.setRoutine(new Wander());

	while (window.isOpen()) {
		window.clear();
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		
		if (updateClock.getElapsedTime().asMilliseconds() > 10) {
			player.update();
			updateClock.restart();
		}

		if (skeletonClock.getElapsedTime().asSeconds() > 1) {
			skeleton.update();
			skeletonClock.restart();
		}
		
		//update();
		render();
	}
	
	return 0;
}
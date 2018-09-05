#include "stdafx.h"
#include "Tile.h"
#include <thread>
#include <chrono>
#include <vector>
#include "DungeonCrawler.h"
#include <fstream>
#include <iomanip>
#include "Entity.h"
#include "TileMap.h"
#include "Player.h"

using std::shared_ptr;
using std::vector;

/// <summary> Mutex for to allow for editing vectors during multithreading. </summary>
std::mutex mutex;

/// <summary> Graph which stores tiles. </summary>
Graph<Tile> graph;

/// <summary> The games map. </summary>
TileMap map("resources/map.json", graph);

/// <summary> The SFML window. </summary>
sf::RenderWindow window(sf::VideoMode(map.getWidth(), map.getHeight()), "Dungeon Crawler");

/// <summary> Whether the graph is currently running the A* algoritm. </summary>
bool started = false;

/// <summary> The clock to count FPS. </summary>
sf::Clock fpsClock;

/// <summary> The clock to count FPS. </summary>
sf::Clock updateClock;

sf::Clock skeletonClock;

const bool DRAW_PATH = false;

shared_ptr<Tile> targetTile = nullptr;
int main() {
	Player player("resources/mage.png", 5, 5, map.getTileSize().x, map.getTileSize().y, graph);
	Entity skeleton("resources/skeleton.png", 28, 28, map.getTileSize().x, map.getTileSize().y, graph);

	updateClock.restart();
	skeletonClock.restart();

	while (window.isOpen()) {
		window.clear();

		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
		bool eDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
		bool qDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
		bool fDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);
		bool controlDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
		bool nDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N);
		bool mDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M);
		bool enterDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
		bool yDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y);
		bool sDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
		bool leftClick = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

		//framerate
		float framerate = 1.f / fpsClock.getElapsedTime().asSeconds();
		fpsClock.restart();
		//std::cout << std::to_string(framerate) << std::endl;

		
		for (auto const& tileEntry : graph.getNodes()) {
			shared_ptr<Tile> tile = tileEntry.second;
			tile->setFillColor(sf::Color::Color(100, 100, 100, 255)); //default color

			if (DRAW_PATH) {
				if (tile == map.getStart())
					tile->setFillColor(sf::Color::Yellow);

				if (tile == map.getEnd())
					tile->setFillColor(sf::Color::Blue);
			}

			if (tile->getFilled())
				tile->setFillColor(sf::Color::White);
		}

		if (DRAW_PATH) {
			for (shared_ptr<Tile> tile : graph.getOpenList())
				if (tile != map.getStart() && tile != map.getEnd()) //don't cover start and end
					tile->setFillColor(sf::Color::Green);

			for (shared_ptr<Tile> tile : graph.getClosedList())
				if (tile != map.getStart() && tile != map.getEnd()) //don't cover start and end
					tile->setFillColor(sf::Color::Red);

			for (shared_ptr<Tile> tile : graph.getPathList())
				if (tile != map.getStart() && tile != map.getEnd()) //don't cover start and end
					tile->setFillColor(sf::Color::Magenta);
		}

		for (auto const& tileEntry : graph.getNodes())
			window.draw(*tileEntry.second);

		shared_ptr<Tile> playerTile = graph[player.getGridX()][player.getGridY()];
		if (skeletonClock.getElapsedTime().asMilliseconds() > 100) {

			if (skeleton.isPathing()) {
				skeleton.advancePath();
				targetTile = playerTile;
			}

			skeletonClock.restart();
		}

		if (updateClock.getElapsedTime().asMilliseconds() > 50) {
			if (player.isPathing()) {
				player.advancePath();
			} else {
				player.updateControls();
			}

			updateClock.restart();
		}

		
		if (targetTile != playerTile) {
			shared_ptr<Tile> skeletonTile = graph[skeleton.getGridX()][skeleton.getGridY()];
			targetTile = playerTile;
			skeleton.path(graph.aStar(skeletonTile, playerTile));
		}


		if (controlDown && sDown)
			map.save();

		for (int x = 0; x < map.getSize(); x++) {
			for (int y = 0; y < map.getSize(); y++) {
				shared_ptr<Tile> tile = graph[x][y];

				if (leftClick && tile->getGlobalBounds().contains(mousePos)) {
					shared_ptr<Tile> playerTile = graph[player.getGridX()][player.getGridY()];
					player.path(graph.aStar(playerTile, tile));
				}

				if (eDown && tile->getGlobalBounds().contains(mousePos)) {
					tile->setFilled(true);
					graph.clearConnections(x, y);
				}

				if (qDown && tile->getGlobalBounds().contains(mousePos)) {
					tile->setFilled(false);
					graph.addConnections(x, y, map.usingDiagonalMovement());
				}

				if (nDown && tile->getGlobalBounds().contains(mousePos)) {
					map.setStart(tile);
				}

				if (mDown && tile->getGlobalBounds().contains(mousePos)) {
					map.setEnd(tile);
				}

				if (yDown && tile->getGlobalBounds().contains(mousePos)) {
					std::cout << *tile << std::endl; //dereference pointer
				}

				if (enterDown && !started) {
					graph.aStar(map.getStart(), map.getEnd());
					started = true;
				}

				if (fDown) {
					graph.clear();
					started = false;
				}

				if (spaceDown && tile->getGlobalBounds().contains(mousePos)) {
					//draw connections
					vector<shared_ptr<Tile>> &connections = graph.adj(x, y);
					for (int i = 0; i < connections.size(); i++) {
						shared_ptr<Tile> connectedTile = connections.at(i);
						sf::Vertex line[] =
						{
							sf::Vertex(sf::Vector2f(tile->getWorldX() + map.getTileSize().x / 2, tile->getWorldY() + map.getTileSize().y / 2), sf::Color::Color(0, 255, 221,255)),
							sf::Vertex(sf::Vector2f(connectedTile->getWorldX() + map.getTileSize().x / 2, connectedTile->getWorldY() + map.getTileSize().y / 2),  sf::Color::Color(0, 255, 221,255))
						};

						window.draw(line, 2, sf::Lines);
					}
				}
			}
		}
		
		window.draw(player);
		window.draw(skeleton);
		
		window.display();
	}
	
	return 0;
}
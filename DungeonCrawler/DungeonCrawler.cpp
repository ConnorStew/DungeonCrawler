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

int main() {
	sf::Texture texture;
	if (!texture.loadFromFile("resources/mage.png"))
		std::cout << "Can't load texture." << std::endl;

	Entity player(5,5);
	player.setTexture(texture);
	player.setScale(
		map.getTileSize().x / player.getLocalBounds().width,
		map.getTileSize().y / player.getLocalBounds().height);

	//add connections
	for (int x = 0; x < map.getSize(); x++)
		for (int y = 0; y < map.getSize(); y++)
			graph.addConnections(x, y, map.usingDiagonalMovement());
	
	sf::Clock clock;
	while (window.isOpen()) {
		window.clear();

		//display FPS
		float framerate = 1.f / clock.getElapsedTime().asSeconds();
		clock.restart();
		std::cout << std::to_string(framerate) << std::endl;

		//color tiles
		for (auto const& tileEntry : graph.getNodes()) {
			shared_ptr<Tile> tile = tileEntry.second;
			tile->setFillColor(sf::Color::Color(100, 100, 100, 255)); //default color

			if (tile == map.getStart())
				tile->setFillColor(sf::Color::Yellow);

			if (tile == map.getEnd())
				tile->setFillColor(sf::Color::Blue);

			if (tile->getFilled())
				tile->setFillColor(sf::Color::White);
		}

		for (shared_ptr<Tile> tile : graph.getOpenList()) //color open list
			if (tile != map.getStart() && tile != map.getEnd()) //don't cover start and end
				tile->setFillColor(sf::Color::Green);

		for (shared_ptr<Tile> tile : graph.getClosedList()) //color closed list
			if (tile != map.getStart() && tile != map.getEnd()) //don't cover start and end
				tile->setFillColor(sf::Color::Red);

		for (shared_ptr<Tile> tile : graph.getPathList()) //color path list
			if (tile != map.getStart() && tile != map.getEnd()) //don't cover start and end
				tile->setFillColor(sf::Color::Magenta);

		//draw tiles
		for (auto const& tileEntry : graph.getNodes())
			window.draw(*tileEntry.second);

		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();
		
		bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
		bool eDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
		bool qDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
		bool fDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);
		bool controlDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
		bool nDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N);
		bool mDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M);
		bool enterDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
		bool yDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y);

		bool wDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
		bool aDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
		bool sDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
		bool dDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

		//player controls
		if (wDown)
			if (player.getGridY() - 1 < map.getSize() && player.getGridY() - 1 >= 0)
				if (!graph[player.getGridX()][player.getGridY() - 1]->getFilled())
					player.setGridY(player.getGridY() - 1);

		if (sDown && !controlDown)
			if (player.getGridY() + 1 < map.getSize() && player.getGridY() + 1 >= 0)
				if (!graph[player.getGridX()][player.getGridY() + 1]->getFilled())
					player.setGridY(player.getGridY() + 1);

		if (aDown)
			if (player.getGridX() - 1 < map.getSize() && player.getGridX() - 1 >= 0)
				if (!graph[player.getGridX() - 1][player.getGridY()]->getFilled())
					player.setGridX(player.getGridX() - 1);

		if (dDown)
			if (player.getGridX() + 1 < map.getSize() && player.getGridX() + 1 >= 0)
				if (!graph[player.getGridX() + 1][player.getGridY()]->getFilled())
					player.setGridX(player.getGridX() + 1);

		shared_ptr<Tile> playerTile = graph[player.getGridX()][player.getGridY()];
		player.setPosition(playerTile->getWorldX(), playerTile->getWorldY());

		if (controlDown && sDown) {
			map.save();
		}

		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		for (int x = 0; x < map.getSize(); x++) {
			for (int y = 0; y < map.getSize(); y++) {
				shared_ptr<Tile> tile = graph[x][y];

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
		
		window.display();
	}
	
	return 0;
}
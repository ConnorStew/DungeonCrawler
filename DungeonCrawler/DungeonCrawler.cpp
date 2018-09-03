#include "stdafx.h"
#include "Tile.h"
#include <thread>
#include <chrono>
#include <vector>
#include "DungeonCrawler.h"
#include <queue>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "nlohmann/json.hpp"
#include "Entity.h"
#include <sstream>
#include <iostream>

using json = nlohmann::json;
using std::shared_ptr;
using std::vector;

/// <summary> X spacing between tiles. </summary>
const int xSpacing = 2;

/// <summary> Y spacing between tiles. </summary>
const int ySpacing = 2;

/// <summary> Size of the graph. </summary>
const int graphSize = 32;

/// <summary> Whether the tiles should allow diagonal movement. </summary>
const bool DIAGONAL_MOVEMENT = true;

/// <summary> The size of a tile. </summary>
const sf::Vector2f tileSize = sf::Vector2f(20,20);

/// <summary> The file to save the map to. </summary>
const std::string mapFile = "resources/map.json";

/// <summary> Whether a map has been loaded. </summary>
bool mapLoaded = false;

/// <summary> Mutex for to allow for editing vectors during multithreading. </summary>
std::mutex mutex;

/// <summary> Graph which stores tiles. </summary>
Graph<Tile> graph;

/// <summary> The SFML window. </summary>
sf::RenderWindow window(sf::VideoMode((graphSize * tileSize.x) + (graphSize * xSpacing) - xSpacing, (graphSize * tileSize.y) + (graphSize * ySpacing) - ySpacing), "Dungeon Crawler");

/// <summary> The start node. </summary>
shared_ptr<Tile> start = nullptr;

/// <summary> The end node. </summary>
shared_ptr<Tile> end = nullptr;

/// <summary> Whether the graph is running the A* algorithm. </summary>
bool started = false;

/// <summary>
/// Clears the connections which involve the given tile.
/// </summary>
/// <param name="tile">The tile to remove the connections of.</param>
void clearConnections(shared_ptr<Tile> tile) {
	vector<shared_ptr<Tile>>& connectedTiles = graph.adj(tile->getGridX(),tile->getGridY());

	for (int i = 0; i < connectedTiles.size(); i++) {
		shared_ptr<Tile>& connectedTile = connectedTiles.at(i);
		std::cout << *connectedTile << std::endl; //dereference pointer

		//remove the original tile from the connected tiles connections
		vector<shared_ptr<Tile>>& adjList = graph.adj(connectedTile->getGridX(), connectedTile->getGridY());
		auto it = std::find(adjList.begin(), adjList.end(), tile);
		if (it != adjList.end()) {
			adjList.erase(it);
		}
	}

	connectedTiles.clear();
}

/// <summary>
/// Connects tiles to the tile at x,y to the tile at x + xIncrease, y + yIncrease.
/// </summary>
/// <param name="x">X coordinate of node1.</param>
/// <param name="y">Y coordinate of node1.</param>
/// <param name="xIncrease">X distance of node2 away from node1.</param>
/// <param name="yIncrease">Y distance of node2 away from node1.</param>
void connectIfValid(int x, int y, int xIncrease, int yIncrease) {
	if (x < graphSize && x >= 0 && y < graphSize && y >= 0) {
		if (x + xIncrease < graphSize && x + xIncrease >= 0 && y + yIncrease < graphSize && y + yIncrease >= 0) {
			if (!graph[x][y]->getFilled() && !graph[x + xIncrease][y + yIncrease]->getFilled()) {
				graph.adj(x,y).push_back(graph[x + xIncrease][y + yIncrease]);
				graph.adj(x + xIncrease, y + yIncrease).push_back(graph[x][y]);
			}
		}
	}
}

/// <summary>
/// Adds connections for the given tile.
/// </summary>
/// <param name="tile">The tile to add connections to.</param>
void addConnections(shared_ptr<Tile> tile) {
	int x = tile->getGridX();
	int y = tile->getGridY();

	connectIfValid(x, y, 1, 0); //right
	connectIfValid(x, y, -1, 0); //left
	connectIfValid(x, y, 0, 1); //up
	connectIfValid(x, y, 0, -1); //down

	if (DIAGONAL_MOVEMENT) {
		connectIfValid(x, y, 1, -1); //bottom right
		connectIfValid(x, y, 1, 1); //top right
		connectIfValid(x, y, -1, -1); //bottom left
		connectIfValid(x, y, -1, 1); //top left
	}
}

int main() {
	sf::Texture texture;
	if (!texture.loadFromFile("resources/mage.png")) {
		std::cout << "Can't load texture." << std::endl;
	}

	Entity player(5,5);
	player.setTexture(texture);
	player.setScale(
		tileSize.x / player.getLocalBounds().width,
		tileSize.y / player.getLocalBounds().height);

	// read a JSON file
	std::ifstream in(mapFile);
	if (in.good()) {
		json map;
		in >> map;

		for (json::iterator it = map["tiles"].begin(); it != map["tiles"].end(); ++it) {
			auto member = *it;
			std::string fullLocation = it.key();
			vector<std::string> container;
			std::stringstream ss(fullLocation);
			while (ss.good()) {
				std::string substr;
				getline(ss, substr, ',');
				container.push_back(substr);
			}

			int x = std::stoi(container[0]);
			int y = std::stoi(container[1]);




			float worldX = (x * tileSize.x) + (x * xSpacing);
			float worldY = (y * tileSize.y) + (y * ySpacing);

			Tile tile(x, y, worldX, worldY, false, tileSize);
			tile.setSize(tileSize);
			tile.setPosition(worldX, worldY);
			tile.setFillColor(sf::Color::Color(100, 100, 100, 255));

			shared_ptr<Tile> stile = std::make_shared<Tile>(tile);
			graph[x][y] = stile;

			if (map["tiles"][fullLocation]["filled"] == true) {
				graph[x][y]->setFilled(true);
			}
		}


		int startX = map["start"]["x"];
		int startY = map["start"]["y"];
		int endX = map["end"]["x"];
		int endY = map["end"]["y"];

		start = graph[startX][startY];
		end = graph[endX][endY];

		mapLoaded = true;
	} else {
		//add tiles
		for (int x = 0; x < graphSize; x++) {
			for (int y = 0; y < graphSize; y++) {
				float worldX = (x * tileSize.x) + (x * xSpacing);
				float worldY = (y * tileSize.y) + (y * ySpacing);

				Tile tile(x, y, worldX, worldY, false, tileSize);
				tile.setSize(tileSize);
				tile.setPosition(worldX, worldY);
				tile.setFillColor(sf::Color::Color(100, 100, 100, 255));

				graph[x][y] = std::make_shared<Tile>(tile);
			}
		}
	}

	in.close();


	//add connections
	for (int x = 0; x < graphSize; x++) {
		for (int y = 0; y < graphSize; y++) {
			addConnections(graph[x][y]);
		}
	}
	
	while (window.isOpen()) {
		window.clear();

		std::lock_guard<std::mutex> lock(mutex);
		for (int x = 0; x < graphSize; x++) {
			for (int y = 0; y < graphSize; y++) {
				shared_ptr<Tile> tile = graph[x][y];

				tile->setFillColor(sf::Color::Color(100, 100, 100, 255));

				if (graph.inOpenList(graph[x][y])) {
					tile->setFillColor(sf::Color::Green);
				}

				if (graph.inClosedList(tile)) {
					tile->setFillColor(sf::Color::Red);
				}

				if (graph.inPathList(tile)) {
					tile->setFillColor(sf::Color::Yellow);
				}

				if (tile == start) {
					tile->setFillColor(sf::Color::Magenta);
				}

				if (tile == end) {
					tile->setFillColor(sf::Color::Blue);
				}

				if (tile->getFilled()) {
					tile->setFillColor(sf::Color::White);
				}

				window.draw(*tile);
			}
		}

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
			if (player.getGridY() - 1 < graphSize && player.getGridY() - 1 >= 0)
				if (!graph[player.getGridX()][player.getGridY() - 1]->getFilled())
					player.setGridY(player.getGridY() - 1);

		if (sDown)
			if (player.getGridY() + 1 < graphSize && player.getGridY() + 1 >= 0)
				if (!graph[player.getGridX()][player.getGridY() + 1]->getFilled())
					player.setGridY(player.getGridY() + 1);

		if (aDown)
			if (player.getGridX() - 1 < graphSize && player.getGridX() - 1 >= 0)
				if (!graph[player.getGridX() - 1][player.getGridY()]->getFilled())
					player.setGridX(player.getGridX() - 1);

		if (dDown)
			if (player.getGridX() + 1 < graphSize && player.getGridX() + 1 >= 0)
				if (!graph[player.getGridX() + 1][player.getGridY()]->getFilled())
					player.setGridX(player.getGridX() + 1);

		shared_ptr<Tile> playerTile = graph[player.getGridX()][player.getGridY()];
		player.setPosition(playerTile->getWorldX(), playerTile->getWorldY());

		if (controlDown && sDown) {
			json map = { { "size", graphSize }, 
			{"start", {
				{"x", start->getGridX()},
				{"y", start->getGridY() }
				}},
			{"end",{
				{"x", end->getGridX()},
				{"y", end->getGridY()}
				}},
			};

			for (int x = 0; x < graphSize; x++) {
				for (int y = 0; y < graphSize; y++) {
					map["tiles"][(std::to_string(x) + "," + std::to_string(y))]["filled"] = graph[x][y]->getFilled();
				}
			}

			// write prettified JSON to another file
			std::ofstream out(mapFile);
			out << std::setw(4) << map << std::endl;
			out.close();

			std::cout << "Map saved!" << std::endl;
		}

		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

		for (int x = 0; x < graphSize; x++) {
			for (int y = 0; y < graphSize; y++) {
				shared_ptr<Tile> tile = graph[x][y];

				if (eDown && tile->getGlobalBounds().contains(mousePos)) {
					tile->setFilled(true);
					clearConnections(tile);
				}

				if (qDown && tile->getGlobalBounds().contains(mousePos)) {
					tile->setFilled(false);
					addConnections(tile);
				}

				if (nDown && tile->getGlobalBounds().contains(mousePos)) {
					start = tile;
				}

				if (mDown && tile->getGlobalBounds().contains(mousePos)) {
					end = tile;
				}

				if (yDown && tile->getGlobalBounds().contains(mousePos)) {
					std::cout << *tile << std::endl; //dereference pointer
				}

				if (enterDown && !started) {
					//std::thread thread(&Graph<Tile>::aStar, &graph, start, end);
					//thread.detach();
					graph.aStar(start, end);
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
							sf::Vertex(sf::Vector2f(tile->getWorldX() + tileSize.x / 2, tile->getWorldY() + tileSize.y / 2), sf::Color::Color(0, 255, 221,255)),
							sf::Vertex(sf::Vector2f(connectedTile->getWorldX() + tileSize.x / 2, connectedTile->getWorldY() + tileSize.y / 2),  sf::Color::Color(0, 255, 221,255))
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
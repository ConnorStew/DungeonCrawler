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

using json = nlohmann::json;

const int xSpacing = 2;
const int ySpacing = 2;
const int points = 20;
const int graphSize = 32;
const bool DIAGONAL_MOVEMENT = false;
const sf::Vector2f tileSize = sf::Vector2f(10,10);

std::mutex mutex;
Graph<Tile, graphSize> graph;
sf::RenderWindow window(sf::VideoMode((graphSize * tileSize.x) + (graphSize * xSpacing) - xSpacing, (graphSize * tileSize.y) + (graphSize * ySpacing) - ySpacing), "Dungeon Crawler");
std::shared_ptr<Tile> start = nullptr;
std::shared_ptr<Tile> end = nullptr;
bool started = false;

void clearConnections(std::shared_ptr<Tile> tile) {
	std::vector<std::shared_ptr<Tile>>* connectedTiles = graph.adj(tile->getGridX(),tile->getGridY());

	for (int i = 0; i < connectedTiles->size(); i++) {
		std::shared_ptr<Tile> connectedTile = connectedTiles->at(i);
		std::cout << *connectedTile << std::endl; //dereference pointer

		//remove the original tile from the connected tiles connections
		std::vector<std::shared_ptr<Tile>>* adjList = graph.adj(connectedTile->getGridX(), connectedTile->getGridY());
		auto it = std::find(adjList->begin(), adjList->end(), tile);
		if (it != adjList->end()) {
			adjList->erase(it);
		}
	}

	connectedTiles->clear();
}

void connectIfValid(int x, int y, int xIncrease, int yIncrease) {
	if (x < graphSize && x >= 0 && y < graphSize && y >= 0) {
		if (x + xIncrease < graphSize && x + xIncrease >= 0 && y + yIncrease < graphSize && y + yIncrease >= 0) {
			if (!graph[x][y]->getFilled() && !graph[x + xIncrease][y + yIncrease]->getFilled()) {
				graph.adj(x,y)->push_back(graph[x + xIncrease][y + yIncrease]);
				graph.adj(x + xIncrease, y + yIncrease)->push_back(graph[x][y]);
			}
		}
	}
}

void addConnections(std::shared_ptr<Tile> tile) {
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
	
	//add tiles
	for (int x = 0; x < graphSize; x++) {
		for (int y = 0; y < graphSize; y++) {
			float worldX = (x * tileSize.x) + (x * xSpacing);
			float worldY = (y * tileSize.y) + (y * ySpacing);

			Tile tile(x, y, worldX, worldY, false, tileSize);
			tile.setPosition(worldX, worldY);
			tile.setFillColor(sf::Color::Color(100, 100, 100, 255));

			graph[x][y] = std::make_shared<Tile>(tile);
		}
	}

	// read a JSON file
	std::ifstream in("map.json");
	json map;
	in >> map;
	in.close();

	int startX = map["start"]["x"];
	int startY = map["start"]["y"];
	int endX = map["end"]["x"];
	int endY = map["end"]["y"];

	start = graph[startX][startY];
	end = graph[endX][endY];

	//fill based on json file
	for (int x = 0; x < graphSize; x++) {
		for (int y = 0; y < graphSize; y++) {
			if (map["tiles"][(std::to_string(x) + "," + std::to_string(y))]["filled"] == true) {
				graph[x][y]->setFilled(true);
			}
		}
	}

	//add connections
	for (int x = 0; x < graphSize; x++) {
		for (int y = 0; y < graphSize; y++) {
			addConnections(graph[x][y]);
		}
	}
	
	while (window.isOpen()) {
		window.clear();
		
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();
		
		for (int x = 0; x < graphSize; x++) {
			for (int y = 0; y < graphSize; y++) {
				std::lock_guard<std::mutex> lock(mutex);
				std::shared_ptr<Tile> tile = graph[x][y];

				tile->setFillColor(sf::Color::Color(100, 100, 100, 255));
				
				if (graph.inOpenList(tile)) {
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

		bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
		bool eDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
		bool qDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
		bool fDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F);
		bool controlDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
		bool sDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
		bool nDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N);
		bool mDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M);
		bool enterDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter);
		bool yDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y);

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
			std::ofstream out("map.json");
			out << std::setw(4) << map << std::endl;
			out.close();

			std::cout << "Map saved!" << std::endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
		}


		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		
		for (int x = 0; x < graphSize; x++) {
			for (int y = 0; y < graphSize; y++) {
				std::shared_ptr<Tile> tile = graph[x][y];
				
				if (spaceDown && tile->getGlobalBounds().contains(mousePos)) {
					//draw connections
					std::vector<std::shared_ptr<Tile>>* connections = graph.adj(x,y);
					for (int i = 0; i < connections->size(); i++) {
						std::shared_ptr<Tile> connectedTile = connections->at(i);
						sf::Vertex line[] =
						{
							sf::Vertex(sf::Vector2f(tile->getWorldX() + tileSize.x / 2, tile->getWorldY() + tileSize.y / 2), sf::Color::Color(0, 255, 221,255)),
							sf::Vertex(sf::Vector2f(connectedTile->getWorldX() + tileSize.x / 2, connectedTile->getWorldY() + tileSize.y / 2),  sf::Color::Color(0, 255, 221,255))
						};

						window.draw(line, 2, sf::Lines);
					}
				}

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
					std::thread thread(&Graph<Tile, graphSize>::aStar, &graph, start, end);
					thread.detach();
					started = true;
				}

				if (fDown) {
					graph.clear();
					started = false;
				}

			}
		}

		window.display();
	}
	
	return 0;
}
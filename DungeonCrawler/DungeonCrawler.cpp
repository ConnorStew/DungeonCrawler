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

void fillTiles(int startX, int startY, int width, int height) {
	for (int x = startX; x < startX + width; x++) {
		for (int y = startY; y < startY + height; y++) {
			graph[x][y]->setFilled(true);
		}
	}
}

int main() {
	
	//add tiles
	for (int x = 0; x < graphSize; x++) {
		for (int y = 0; y < graphSize; y++) {
			std::pair<int, int> gridLoc = std::make_pair(x, y);
			std::pair<float, float> worldLoc = std::make_pair((x * tileSize.x) + (x * xSpacing), (y * tileSize.y) + (y * ySpacing));

			Tile tile(gridLoc, worldLoc, false, tileSize);
			tile.setPosition(worldLoc.first, worldLoc.second);
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
			graph.connect(x, y, x + 1, y); //right
			graph.connect(x, y, x - 1, y); //left
			graph.connect(x, y, x, y + 1); //up
			graph.connect(x, y, x, y - 1); //down

			if (DIAGONAL_MOVEMENT) {
				graph.connect(x, y, x + 1, y - 1); //bottom right
				graph.connect(x, y, x + 1, y + 1); //top right
				graph.connect(x, y, x - 1, y - 1); //bottom left
				graph.connect(x, y, x - 1, y + 1); //top left
			}
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

		if (controlDown && sDown) {
			json map = { { "size", graphSize }, 
			{"start", {
				{"x", start->gridX()},
				{"y", start->gridY() }
				}},
			{"end",{
				{"x", end->gridX()},
				{"y", end->gridY()}
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
					std::vector<std::shared_ptr<Tile>> connections = graph.adj(x,y);
					for (std::shared_ptr<Tile> connection : connections) {
						sf::Vertex line[] =
						{
							sf::Vertex(sf::Vector2f(tile->getWorldLoc().first + tileSize.x / 2, tile->getWorldLoc().second + tileSize.y / 2), sf::Color::Color(0, 255, 221,255)),
							sf::Vertex(sf::Vector2f(connection->getWorldLoc().first + tileSize.x / 2, connection->getWorldLoc().second + tileSize.y / 2),  sf::Color::Color(0, 255, 221,255))
						};

						window.draw(line, 2, sf::Lines);
					}
				}

				if (eDown && tile->getGlobalBounds().contains(mousePos)) {
					tile->setFilled(true);
					//graph.clearConnections(tile);
				}

				if (qDown && tile->getGlobalBounds().contains(mousePos)) {
					tile->setFilled(false);
				}

				if (nDown && tile->getGlobalBounds().contains(mousePos)) {
					start = tile;
				}

				if (mDown && tile->getGlobalBounds().contains(mousePos)) {
					end = tile;
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
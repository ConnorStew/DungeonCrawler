#include "stdafx.h"
#include "Tile.h"
#include <thread>
#include <chrono>
#include <vector>
#include "DungeonCrawler.h"
#include <queue>
#include "Graph.h"

const int xSpacing = 2;
const int ySpacing = 2;
const int points = 20;
const int graphSize = 32;
const bool DIAGONAL_MOVEMENT = false;
const sf::Vector2f tileSize = sf::Vector2f(10,10);

std::mutex mutex;
Graph<Tile, graphSize> graph;
sf::RenderWindow window(sf::VideoMode((graphSize * tileSize.x) + (graphSize * xSpacing) - xSpacing, (graphSize * tileSize.y) + (graphSize * ySpacing) - ySpacing), "Dungeon Crawler");

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

	//fill tiles
	fillTiles(0, 10, 15, 5);
	fillTiles(16, 10, graphSize - 16, 5);
	fillTiles(0, 20, 15, 12);
	fillTiles(20, 20, graphSize - 21, 11);


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
	
	std::shared_ptr<Tile> start = graph[8][1];
	std::shared_ptr<Tile> end = graph[31][31];
	std::thread thread(&Graph<Tile, graphSize>::aStar, &graph, start, end);
	thread.detach();
	thread.detach();
	
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
				
				if (tile->getFilled()) {
					tile->setFillColor(sf::Color::White);
				}
				
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
				
				window.draw(*tile);
			}
		}

		bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

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

			}
		}

		window.display();
	}
	
	return 0;
}
#include "stdafx.h"
#include "Tile.h"
#include "Graph.h"
#include "Tile.h"
#include <thread>
#include <chrono>
#include <vector>
#include "DungeonCrawler.h"
#include <queue>

const int size = 1;
const int xSpacing = 2;
const int ySpacing = 2;
const int points = 20;
const int graphSize = 32;

int indexCount = 0;

std::vector<Tile> openList;
std::vector<Tile> closedList;
std::vector<std::vector<Tile>> adjList;
std::map<std::pair<int, int>, Tile> tiles;
std::mutex mutex;

Tile getTile(int x, int y) {
	auto it = tiles.find(std::make_pair(x, y));
	if (it != tiles.end()) {
		return it->second;
	}
}

void aStar(Tile start, Tile end) {
	
	openList.clear();
	closedList.clear();

	std::vector<std::shared_ptr<Tile>> path;

	openList.push_back(start);

	while (!openList.empty()) {
		
		
		if (std::find(openList.begin(), openList.end(), end) != openList.end()) {
			/*
			path.push_back(std::make_shared<Tile>(end));

			std::shared_ptr<Tile> current = std::make_shared<Tile>(end);
			while (current->getParent() != nullptr) {
				path.push_back(current->getParent());
				current = current->getParent();
			}
			*/

			std::cout << "found goal Tile" << std::endl;
			return;
		}

		//get lowest Tiles
		std::vector<Tile> lowestVertices = std::vector<Tile>();
		float lowestScore = std::numeric_limits<float>::max();

		for (Tile Tile : openList)
			if (Tile.getF() < lowestScore)
				lowestScore = Tile.getF();

		for (Tile Tile : openList)
			if (Tile.getF() == lowestScore)
				lowestVertices.push_back(Tile);

		for (Tile tile : lowestVertices) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			std::lock_guard<std::mutex> lock(mutex);
			
			closedList.push_back(tile);
			openList.erase(std::remove(openList.begin(), openList.end(), tile), openList.end());

			
			for (Tile neighbour : adjList[tile.getIndex()]) {
				if (!(std::find(openList.begin(), openList.end(), neighbour) != openList.end()) && !(std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())) {
					if (std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())
						continue;

					float tentativeG = tile.getG() + std::abs(tile.getX() - neighbour.getX()) + std::abs(tile.getY() - neighbour.getY());
					float tentativeH = std::abs(end.getX() - neighbour.getX()) + std::abs(end.getY() - neighbour.getY());
					float tentativeScore = (tentativeG + tentativeH);

					if (!(std::find(openList.begin(), openList.end(), neighbour) != openList.end())) {
						openList.push_back(neighbour);
						tile.updateScore(neighbour.getParent(), tentativeScore, tentativeG, tentativeH);
					}

					//update the score of the neighbouring vertex
					if (tentativeScore < neighbour.getF()) {
						tile.updateScore(neighbour.getParent(), tentativeScore, tentativeG, tentativeH);
					}
				}
			}
			
		}
	}
}

int main() {
	

	for (int x = 0; x < graphSize; x++) {
		for (int y = 0; y < graphSize; y++) {
			std::pair<int, int> location = std::make_pair(x, y);

			Tile tile(location, indexCount);
			tile.setSize(sf::Vector2f(2, 2));
			tile.setPosition((x * size) + (x * xSpacing), y * size + (y * ySpacing));
			tile.setFillColor(sf::Color::Color(100,100,100,255));

			adjList.push_back(std::vector<Tile>());
			tiles.insert(std::make_pair(location, tile));
			indexCount++;
		}
	}

	//add connections
	for (int x = 0; x < graphSize; x++) {
		for (int y = 0; y < graphSize; y++) {
			
			auto it = tiles.find(std::make_pair(x, y));
			if (it != tiles.end()) {
				Tile tile = it->second;

				auto it1 = tiles.find(std::make_pair(x + 1, y));
				if (it1 != tiles.end()) {
					adjList[tile.getIndex()].push_back(it1->second);
				}

				auto it2 = tiles.find(std::make_pair(x - 1, y));
				if (it2 != tiles.end()) {
					adjList[tile.getIndex()].push_back(it2->second);
				}
				
				auto it3 = tiles.find(std::make_pair(x, y + 1));
				if (it3 != tiles.end()) {
					adjList[tile.getIndex()].push_back(it3->second);
				}

				auto it4 = tiles.find(std::make_pair(x, y - 1));
				if (it4 != tiles.end()) {
					adjList[tile.getIndex()].push_back(it4->second);
				}
			}
		}
	}

	sf::RenderWindow window(sf::VideoMode(200, 200), "Dungeon Crawler");

	Tile start = getTile(10, 5);
	Tile end = getTile(31, 20);
	
	std::thread thread(aStar, start, end);
	thread.detach();

	while (window.isOpen()) {
		
		window.clear();
		
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();
		
		//draw tiles
		for (auto it = tiles.begin(); it != tiles.end(); it++) {
			std::lock_guard<std::mutex> lock(mutex);
			Tile tile = it->second;
			
			if (std::find(openList.begin(), openList.end(), tile) != openList.end()) {
				tile.setFillColor(sf::Color::Green);
			}

			if (std::find(closedList.begin(), closedList.end(), tile) != closedList.end()) {
				tile.setFillColor(sf::Color::Red);
			}

			if (tile == start) {
				tile.setFillColor(sf::Color::Magenta);
			}

			if (tile == end) {
				tile.setFillColor(sf::Color::Blue);
			}

			
			window.draw(tile);
		}
		
		window.display();
	}
	
	return 0;
}
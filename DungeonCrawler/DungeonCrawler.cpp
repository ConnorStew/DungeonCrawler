#include "stdafx.h"
#include "Tile.h"
#include "Graph.h"
#include "Tile.h"
#include <thread>
#include <chrono>
#include <vector>
#include "DungeonCrawler.h"
#include <queue>

const int xSpacing = 2;
const int ySpacing = 2;
const int points = 20;
const int graphSize = 32;
const sf::Vector2f tileSize = sf::Vector2f(10,10);

std::vector<std::shared_ptr<Tile>> openList;
std::vector<std::shared_ptr<Tile>> closedList;
std::vector<std::shared_ptr<Tile>> path;

std::vector<std::shared_ptr<Tile>> adjList[graphSize][graphSize];
std::shared_ptr<Tile> tiles[graphSize][graphSize];

sf::RenderWindow window(sf::VideoMode((graphSize * tileSize.x) + (graphSize * xSpacing) - xSpacing, (graphSize * tileSize.y) + (graphSize * ySpacing) - ySpacing), "Dungeon Crawler");
std::mutex mutex;

void tileConnect(std::shared_ptr<Tile> &tile, int x, int y, int xIncrease, int yIncrease) {
	int targetX = x + xIncrease;
	int targetY = y + yIncrease;
	if (targetX < graphSize && targetX >= 0 && targetY < graphSize && targetY >= 0) {
		std::shared_ptr<Tile> &other = tiles[targetX][targetY];
		adjList[tile->getGridLoc().first][tile->getGridLoc().second].push_back(other);
		adjList[other->getGridLoc().first][other->getGridLoc().second].push_back(tile);
	}
}

void aStar(std::shared_ptr<Tile> start, std::shared_ptr<Tile> end) {
	
	openList.clear();
	closedList.clear();
	path.clear();
	

	openList.push_back(start);

	while (!openList.empty()) {
		
		if (std::find(openList.begin(), openList.end(), end) != openList.end()) {
			
			std::shared_ptr<Tile> &current = end;
			path.push_back(current);
			
			do  {
				std::this_thread::sleep_for(std::chrono::milliseconds(2));
				std::lock_guard<std::mutex> lock(mutex);
				current = tiles[current->getParent().first][current->getParent().second];
				path.push_back(current);
			} while (current != start);


			std::cout << "found goal Tile" << std::endl;
			return;
		}

		//get lowest Tiles
		std::vector<std::shared_ptr<Tile>> lowestVertices = std::vector<std::shared_ptr<Tile>>();
		float lowestScore = std::numeric_limits<float>::max();

		for (std::shared_ptr<Tile> Tile : openList)
			if (Tile->getF() < lowestScore)
				lowestScore = Tile->getF();

		for (std::shared_ptr<Tile> Tile : openList)
			if (Tile->getF() == lowestScore)
				lowestVertices.push_back(Tile);

		for (std::shared_ptr<Tile> tile : lowestVertices) {
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
			std::lock_guard<std::mutex> lock(mutex);
			
			

			closedList.push_back(tile);
			openList.erase(std::remove(openList.begin(), openList.end(), tile), openList.end());


			for (std::shared_ptr<Tile> neighbour : adjList[tile->getGridLoc().first][tile->getGridLoc().second]) {
				if (!(std::find(openList.begin(), openList.end(), neighbour) != openList.end()) && !(std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())) {
					if (std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())
						continue;

					float tentativeG = tile->getG() + std::abs(tile->getGridLoc().first - neighbour->getGridLoc().first) + std::abs(tile->getGridLoc().second - neighbour->getGridLoc().second);
					float tentativeH = std::abs(end->getGridLoc().first - neighbour->getGridLoc().first) + std::abs(end->getGridLoc().second - neighbour->getGridLoc().second);
					float tentativeScore = (tentativeG + tentativeH);

					if (!(std::find(openList.begin(), openList.end(), neighbour) != openList.end())) {
						if (neighbour->getFilled() == false) {
							
							openList.insert(openList.begin(),neighbour);
							neighbour->updateScore(tile->getGridLoc(), tentativeScore, tentativeG, tentativeH);
						}
					}

					//update the score of the neighbouring vertex
					if (tentativeScore < neighbour->getF()) {
						neighbour->updateScore(tile->getGridLoc(), tentativeScore, tentativeG, tentativeH);
					}
				}
			}

		}
	}
}

int main() {

	for (int x = 0; x < graphSize; x++) {
		for (int y = 0; y < graphSize; y++) {

			std::pair<int, int> gridLoc = std::make_pair(x, y);
			std::pair<float, float> worldLoc = std::make_pair((x * tileSize.x) + (x * xSpacing), (y * tileSize.y) + (y * ySpacing));

			bool filled = false;
			if (x > 10 && x < 20 && y > 10 && y < 20) {
				filled = true;
			}

			if (x >= 0 && x < 10 && y > 10 && y < 15) {
				filled = true;
			}

			Tile tile(gridLoc, worldLoc, filled, tileSize);
			//tile.setSize(tileSize);

			tile.setPosition(worldLoc.first, worldLoc.second);
			tile.setFillColor(sf::Color::Color(100, 100, 100, 255));

			tiles[x][y] = std::make_shared<Tile>(tile);
		}
	}

	std::shared_ptr<Tile> &start = tiles[1][1];
	std::shared_ptr<Tile> &end = tiles[31][31];

	//add connections
	for (int x = 0; x < graphSize; x++) {
		for (int y = 0; y < graphSize; y++) {
			std::shared_ptr<Tile> &tile = tiles[x][y];
			std::vector<std::shared_ptr<Tile>> &list = adjList[x][y];

			tileConnect(tile, x, y, 1, 0);
			tileConnect(tile, x, y, -1, 0);
			tileConnect(tile, x, y, 0, 1);
			tileConnect(tile, x, y, 0, -1);	
		}
	}
	
	std::thread thread(aStar, start, end);
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
				std::shared_ptr<Tile> &tile = tiles[x][y];
				
				if (tile->getFilled()) {
					tile->setFillColor(sf::Color::White);
				}

				if (std::find(openList.begin(), openList.end(), tile) != openList.end()) {
					tile->setFillColor(sf::Color::Green);
				}

				if (std::find(closedList.begin(), closedList.end(), tile) != closedList.end()) {
					tile->setFillColor(sf::Color::Red);
				}

				if (std::find(path.begin(), path.end(), tile) != path.end()) {
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
		//std::cout << std::to_string(mousePos.x) + " , " + std::to_string(mousePos.y) << std::endl;

		for (int x = 0; x < graphSize; x++) {
			for (int y = 0; y < graphSize; y++) {
				std::shared_ptr<Tile> &tile = tiles[x][y];
				
				if (spaceDown && tile->getGlobalBounds().contains(mousePos)) {
					//draw connections
					std::vector<std::shared_ptr<Tile>> connections = adjList[x][y];
					for (std::shared_ptr<Tile> connection : connections) {
						sf::Vertex line[] =
						{
							sf::Vertex(sf::Vector2f(tile->getWorldLoc().first + tileSize.x / 2, tile->getWorldLoc().second + tileSize.y / 2)),
							sf::Vertex(sf::Vector2f(connection->getWorldLoc().first + tileSize.x / 2, connection->getWorldLoc().second + tileSize.y / 2))
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
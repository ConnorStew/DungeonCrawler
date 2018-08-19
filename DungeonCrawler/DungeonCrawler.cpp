#include "stdafx.h"
#include "Tile.h"
#include "Node.h"
#include "Graph.h"
#include <thread>
#include <chrono>
#include <vector>
#include "DungeonCrawler.h"

int main() {

	/*
	std::map<Key, int> map;
	map.insert(std::make_pair<Key, int>(Key(1, 1), 5));
	map.insert(std::make_pair<Key, int>(Key(2, 2), 76));
	map.insert(std::make_pair<Key, int>(Key(3, 2), 42));
	map.insert(std::make_pair<Key, int>(Key(2, 3), 10));
	map.insert(std::make_pair<Key, int>(Key(1, 2), 8));

	if (!map.insert(std::make_pair<Key, int>(Key(2, 1), 6)).second) {
		std::cout << "Element already present?" << std::endl;
	}

	std::map<Key, int>::iterator it;
	it = map.find(Key(3, 2));

	int foundval;

	if (it != map.end())
		foundval = it->second;

	std::cout << foundval << std::endl;

	return 0;
	
	*/

	std::vector<Tile*> tiles;
	const int size = 1;
	const int xSpacing = 2;
	const int ySpacing = 2;
	const int points = 20;
	const int graphSize = 32;

	Graph graph(graphSize * graphSize);

	for (int x = 0; x < graphSize; x++) {
		for (int y = 0; y < graphSize; y++) {
			Tile* tile = new Tile(x, y);
			graph.addNode(tile);

			tile->setRadius(size);
			tile->setPointCount(points);
			tile->setPosition((x * size) + (x * xSpacing), y * size + (y * ySpacing));
			tile->setFillColor(sf::Color::Red);

			tiles.push_back(tile);
		}
	}

	Node* node = graph.getNode(5, 3);
	Node* node2 = graph.getNode(3, 3);
	graph.addConnection(node, node2);
	node->printDebug();

	sf::RenderWindow window(sf::VideoMode(200, 200), "Dungeon Crawler");

	while (window.isOpen()) {
		// get the current mouse position in the window
		sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
		// convert it to world coordinates
		sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

		bool lmbDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear();

		for (int i = 0; i < tiles.size(); i++) {
			if (lmbDown && tiles.at(i)->getGlobalBounds().contains(worldPos)) {
				tiles.at(i)->setFillColor(sf::Color::Magenta);
				tiles.at(i)->printDebug();
			} else {
				tiles.at(i)->setFillColor(sf::Color::Red);
			}
			window.draw(*tiles.at(i));
		}
			

		window.display();
	}

	return 0;
	
}
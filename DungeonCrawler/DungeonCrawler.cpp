#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Node.h"
#include "Graph.h"
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

int main() {

	
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
	
	/*
	std::vector<sf::CircleShape> circles;
	const int size = 1;
	const int xSpacing = 2;
	const int ySpacing = 2;
	const int points = 20;

	Graph graph(32 * 32);
	//graph.addVertex(0, 1);
	//graph.addVertex(1, 1);

	
	for (int x = 0; x < 31; x++) {
		for (int y = 0; y < 31; y++) {
			graph.addVertex(x, y);
			sf::CircleShape circle(size, points);
			circle.setPosition((x * size) + (x * xSpacing), y * size + (y * ySpacing));
			circle.setFillColor(sf::Color::Red);
			circles.push_back(circle);
		}
	}

	Node* node = graph.getNode(5, 3);
	Node* node2 = graph.getNode(3, 3);
	graph.addConnection(node, node2);
	//node->printDebug();

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event))
			if (event.type == sf::Event::Closed)
				window.close();

		window.clear();

		for (int i = 0; i < circles.size(); i++)
			window.draw(circles.at(i));

		window.display();
	}

	return 0;
	*/
}
#pragma once
#ifndef TILE_H
#define TILE_H

#include "stdafx.h"
#include "Node.h"

class Tile : public sf::RectangleShape, public Node {
	private:
		const static sf::Color DEFAULT_COLOR;
		const static sf::Color FILLED_COLOR;

		sf::Vector2f size;
		sf::RectangleShape border;
	public:
		Tile(int gridX, int gridY, float worldX, float worldY, bool filled, sf::Vector2f tileSize);
		
		friend bool operator==(const Tile& lhs, const Tile& rhs);
		friend std::ostream& operator<<(std::ostream& os, const Tile& dt);

		void fill();
		void clear();

		sf::Vector2f getSize();
		sf::RectangleShape& getBorder();
};

#endif
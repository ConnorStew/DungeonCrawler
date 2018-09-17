#include "stdafx.h"
#include "TileMap.h"
#include "Tile.h"
#include "graph.h"

using json = nlohmann::json;
using std::string;
using std::shared_ptr;

TileMap::TileMap(string fileLocation, Graph<Tile>* graph) {
	
	this->size = DEFAULT_SIZE;
	this->fileLocation = fileLocation;
	
	// read a JSON file
	std::ifstream in(fileLocation);
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

			float worldX = (x * TILE_SIZE.x) + (x * X_SPACING);
			float worldY = (y * TILE_SIZE.y) + (y * Y_SPACING);

			Tile tile(x, y, worldX, worldY, false, TILE_SIZE);
			tile.setSize(TILE_SIZE);
			tile.setPosition(worldX, worldY);

			shared_ptr<Tile> stile = std::make_shared<Tile>(tile);
			graph->addNode(x,y,stile);

			if (map["tiles"][fullLocation]["filled"] == true) {
				graph->at(x,y)->setFilled(true);
			}
		}

		int startX = map["start"]["x"];
		int startY = map["start"]["y"];
		int endX = map["end"]["x"];
		int endY = map["end"]["y"];

		start = graph->at(startX,startY);
		end = graph->at(endX,endY);
	}
	else {
		std::cout << "Cannot load map at " << fileLocation << " generating default map." << std::endl;

		//add empty tiles
		for (int x = 0; x < size; x++) {
			for (int y = 0; y < size; y++) {
				float worldX = (x * TILE_SIZE.x) + (x * X_SPACING);
				float worldY = (y * TILE_SIZE.y) + (y * Y_SPACING);

				Tile tile(x, y, worldX, worldY, false, TILE_SIZE);
				tile.setSize(TILE_SIZE);
				tile.setPosition(worldX, worldY);

				graph->at(x,y) = std::make_shared<Tile>(tile);
			}
		}
	}

	in.close();

	//add connections
	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
			graph->addConnections(x, y, DIAGONAL_MOVEMENT);


}

void TileMap::save() {
	json map = { { "size", size },
		{ "start",{
			{ "x", start->getGridX() },
		{ "y", start->getGridY() }
		} },
		{ "end",{
			{ "x", end->getGridX() },
		{ "y", end->getGridY() }
		} },
	};

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			map["tiles"][(std::to_string(x) + "," + std::to_string(y))]["filled"] = graph->at(x,y)->getFilled();
		}
	}

	// write prettified JSON to another file
	std::ofstream out(fileLocation);
	out << std::setw(4) << map << std::endl;
	out.close();

	std::cout << "Map saved!" << std::endl;
}

int TileMap::getSize() {
	return size;
}

int TileMap::getYSpacing() {
	return Y_SPACING;
}

int TileMap::getXSpacing() {
	return X_SPACING;
}

sf::Vector2f TileMap::getTileSize() {
	return TILE_SIZE;
}

bool TileMap::usingDiagonalMovement() {
	return DIAGONAL_MOVEMENT;
}

int TileMap::getWidth() {
	return (size * TILE_SIZE.x) + (size * X_SPACING) - X_SPACING;
}

int TileMap::getHeight() {
	return (size *  TILE_SIZE.y) + (size * Y_SPACING) - Y_SPACING;
}

shared_ptr<Tile>& TileMap::getEnd() {
	return end;
}

shared_ptr<Tile>& TileMap::getStart() {
	return start;
}

void TileMap::setStart(shared_ptr<Tile> start) {
	this->start = start;
}

void TileMap::setEnd(shared_ptr<Tile> end) {
	this->end = end;
}

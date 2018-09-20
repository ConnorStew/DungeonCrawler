#include "stdafx.h"
#include "TileMap.h"
#include "Tile.h"
#include "TileMap.h"

using json = nlohmann::json;
using std::string;
using std::shared_ptr;
using std::vector;
using std::pair;
using std::map;
using std::cout;
using std::endl;

TileMap::TileMap(string fileLocation) {
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
			addNode(x,y,stile);
			stile->setFillColor(sf::Color(100, 100, 100, 255)); //default color

			if (map["tiles"][fullLocation]["filled"] == true) {
				at(x,y)->setFilled(true);
				at(x,y)->setFillColor(sf::Color::White);
			}
		}
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

				shared_ptr<Tile> stile = std::make_shared<Tile>(tile);
				addNode(x,y,stile);
				stile->setFillColor(sf::Color(100, 100, 100, 255)); //default color
			}
		}
	}

	in.close();

	//add connections
	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
			addConnections(x, y);


}

void TileMap::save() {
	json map = {{ "size", size }};

	for (int x = 0; x < size; x++) {
		for (int y = 0; y < size; y++) {
			std::shared_ptr<Tile> tile = at(x,y);
			bool tileFilled = tile->getFilled();
			map["tiles"][(std::to_string(x) + "," + std::to_string(y))]["filled"] = tileFilled;
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

void TileMap::connectIfValid(int x, int y, int xIncrease, int yIncrease) {
	if (getNode(x,y) != nullptr && getNode(x + xIncrease, y + yIncrease) != nullptr) {
		if (!getNode(x,y)->getFilled() && !getNode(x + xIncrease,y + yIncrease)->getFilled()) {
			adj(x, y).push_back(getNode(x + xIncrease,y + yIncrease));
			adj(x + xIncrease, y + yIncrease).push_back(getNode(x,y));
		}
	}
}

shared_ptr<Tile> TileMap::getNode(int x, int y) {
	pair<int, int> myPair = std::make_pair(x,y);
	typename std::map<pair<int, int>, shared_ptr<Tile>>::iterator it;
	it = nodes.find(myPair);
	if (it != nodes.end())
		return it->second;
	else
		return nullptr;
}

shared_ptr<Tile> TileMap::findNode(sf::Vector2f position) {
	for (const auto& entry : nodes) {
		shared_ptr<Node> node = entry.second;
	}

	return nullptr;
}

void TileMap::addNode(int x, int y, shared_ptr<Tile> node) {
	nodes.insert(std::make_pair(std::make_pair(x,y), node));
}

void TileMap::clear() {
	openList.clear();
	closedList.clear();
	path.clear();
}

void TileMap::addConnections(int x, int y) {
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

void TileMap::clearConnections(int x, int y) {
	shared_ptr<Tile> node = nullptr;
	auto it = nodes.find(std::make_pair(x, y));
	if (it != nodes.end())
		node = it->second;

	vector<shared_ptr<Tile>>& connectedTiles = adj(x,y);

	for (unsigned int i = 0; i < connectedTiles.size(); i++) {
		shared_ptr<Tile>& connectedTile = connectedTiles.at(i);

		//remove the original tile from the connected tiles connections
		vector<shared_ptr<Tile>>& adjList = adj(connectedTile->getGridX(), connectedTile->getGridY());
		auto it = std::find(adjList.begin(), adjList.end(), node);
		if (it != adjList.end()) {
			adjList.erase(it);
		}
	}

	connectedTiles.clear();
}

vector<shared_ptr<Tile>>& TileMap::adj(int x, int y) {
	return adjList[std::make_pair(x,y)];
}

bool TileMap::inOpenList(shared_ptr<Tile> node) {
	return std::find(openList.begin(), openList.end(), node) != openList.end();
}

bool TileMap::inClosedList(shared_ptr<Tile> node) {
	return std::find(closedList.begin(), closedList.end(), node) != closedList.end();
}

const vector<shared_ptr<Tile>>& TileMap::getOpenList() {
	return openList;
}

const vector<shared_ptr<Tile>>& TileMap::getClosedList() {
	return closedList;
}

const vector<shared_ptr<Tile>>& TileMap::getPathList() {
	return path;
}

bool TileMap::inPathList(shared_ptr<Tile> node) {
	return std::find(path.begin(), path.end(), node) != path.end();
}

map<pair<int, int>, shared_ptr<Tile>> TileMap::getNodes() {
	return nodes;
}

shared_ptr<Tile> TileMap::at(int x, int y) {
	return getNode(x,y);
}

vector<shared_ptr<Tile>> TileMap::aStar(int x1, int y1, int x2, int y2) {
	shared_ptr<Tile> start = getNode(x1, y1);
	shared_ptr<Tile> end = getNode(x2, y2);

	if (start == nullptr || end == nullptr) {
		std::cout << "Error: Tiles given are not on graph..." << std::endl;
		return vector<shared_ptr<Tile>>();
	}

	clear();
	openList.push_back(start);

	while (!openList.empty()) {

		if (inOpenList(end)) {
			shared_ptr<Tile> &current = end;

			while (current != start) {
				path.push_back(current);
				current = nodes[std::make_pair(current->getParentX(),current->getParentY())];
			}

			return path;
		}

		//get lowest nodes
		vector<shared_ptr<Tile>> lowestVertices = vector<shared_ptr<Tile>>();
		float lowestScore = std::numeric_limits<float>::max();

		for (shared_ptr<Tile> node : openList)
			if (node->getF() < lowestScore)
				lowestScore = node->getF();

		for (shared_ptr<Tile> node : openList)
			if (node->getF() == lowestScore)
				lowestVertices.push_back(node);

		for (shared_ptr<Tile> node : lowestVertices) {
			//std::lock_guard<std::mutex> lock(mutex);

			closedList.push_back(node);
			openList.erase(std::remove(openList.begin(), openList.end(), node), openList.end());

			for (shared_ptr<Tile> neighbour : adjList[std::make_pair(node->getGridX(),node->getGridY())]) {
				if (!inClosedList(neighbour)) {

					//the tile distance away
					float tentativeG = node->getG() + std::abs(node->getGridX() - neighbour->getGridX()) + std::abs(node->getGridY() - neighbour->getGridY());

					//hueristic distance the world distance away in pixels
					float tentativeH = std::abs(end->getWorldX() - neighbour->getWorldX()) + std::abs(end->getWorldY() - neighbour->getWorldY());

					//total the distance metrics
					float tentativeScore = (tentativeG + tentativeH);

					//add to open list and update score if not in open list
					if (!inOpenList(neighbour)) {
						openList.insert(openList.begin(), neighbour);
						neighbour->updateScore(node->getGridX(), node->getGridY(), tentativeScore, tentativeG, tentativeH);
					}

					//update the score of the neighbouring vertex
					if (tentativeScore < neighbour->getF()) {
						neighbour->updateScore(node->getGridX(), node->getGridY(), tentativeScore, tentativeG, tentativeH);
					}
				}
			}
		}
	}

	return path;
}
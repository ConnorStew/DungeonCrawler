#include "stdafx.h"
#include "TileMap.h"
#include "Tile.h"
#include "TileMap.h"
#include <cmath>
#include <limits>
#include <cstdlib>
#include <chrono>
#include <thread>
#include "mingw.thread.h"
#include "Room.h"

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
			//stile->setFillColor(sf::Color(100, 100, 100, 255)); //default color

			if (map["tiles"][fullLocation]["filled"] == true) {
				at(x,y)->fill();
			}
		}
	} else {
		std::cout << "Cannot load map at " << fileLocation << " generating default map." << std::endl;
		initMap();
	}

	in.close();

	//add connections
	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
			addConnections(x, y);
}

TileMap::TileMap(string fileLocation, int size) {
	this->fileLocation = fileLocation;
	this->size = size;
	initMap();
}

vector<int> TileMap::getTileInts() {
	return tileInts;
}

void TileMap::initMap() {
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

void TileMap::generate(unsigned int targetRoomCount) {
	//fill the border
	for (int x = 0; x < size; x++)
		for (int y = 0; y < size; y++)
			if (y == 0 || y == size - 1 || x == 0 || x == size - 1)
				at(x,y)->fill();

	cout << "Room count: " << targetRoomCount << endl;
	
	int loopCount = 0;

	auto seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 rng(seed);
	std::uniform_int_distribution<int> dist(1, size);
	std::uniform_int_distribution<int> distDirection(0, 3);
	std::uniform_int_distribution<int> distSize(4, 16);
	vector<Room> rooms;

	Room lastRoom = Room(4, 4, 5, 10);
	cout << lastRoom.getX() << ", " << lastRoom.getY() << endl;
	rooms.push_back(lastRoom);

		for (int x = lastRoom.getX(); x < lastRoom.getX() + lastRoom.getWidth() + 1; x++) {
			at(x, lastRoom.getY())->fill();
			at(x, lastRoom.getY() + lastRoom.getHeight())->fill();
		}

		for (int y = lastRoom.getY(); y < lastRoom.getY() + lastRoom.getHeight(); y++) {
			at(lastRoom.getX(), y)->fill();
			at(lastRoom.getX() + lastRoom.getWidth(), y)->fill();
		}


	while (rooms.size() < targetRoomCount) {
		cout << "At start" << endl;

		loopCount++;
		if (loopCount > 500) {
			break;
		}

		int lastX;
		int lastY;
		int width = 5; //distSize(rng);
		int height = 5; //distSize(rng);
		int dir = distDirection(rng);
		switch (dir) {
			case 0: //right
				cout << "Right" << endl;
				lastX = lastRoom.getRight().x + 1;
				lastY = lastRoom.getY();
				break;
			case 1: //left
				cout << "Left" << endl;
				lastX = lastRoom.getLeft().x - width - 1;
				lastY = lastRoom.getY();			
				break;
			case 2: //top
				cout << "Top" << endl;
				lastX = lastRoom.getX();
				lastY = lastRoom.getY() - height - 1;				
				break;
			case 3: //bottom
				cout << "Bottom" << endl;
				lastX = lastRoom.getX();
				lastY = lastRoom.getBottom().y + 1;				
				break;
		}

		cout << lastX << ", " << lastY << endl;

		Room room(lastX, lastY, width, height);

		if (room.nearSide(2, size, size)) {
			cout << "Too close to the side." << endl;
			continue;
		}
		
		if (at(room.getX(), room.getY())->getFilled()) {
			cout << room.getX() << ", " << room.getY() << " is filled." << endl;
			continue;
		}

		rooms.push_back(room);
		lastRoom = room;

		for (int x = room.getX(); x < room.getX() + room.getWidth() + 1; x++) {
			at(x, room.getY())->fill();
			at(x, room.getY() + room.getHeight())->fill();
		}

		for (int y = room.getY(); y < room.getY() + room.getHeight(); y++) {
			at(room.getX(), y)->fill();
			at(room.getX() + room.getWidth(), y)->fill();
		}

		//open way to other rooms
		shared_ptr<Tile> topNode = at(lastRoom.getTop().x, lastRoom.getTop().y);
		shared_ptr<Tile> aboveTopNode = at(lastRoom.getTop().x, lastRoom.getTop().y - 1);
		if (topNode != nullptr && aboveTopNode != nullptr && aboveTopNode->getFilled()) {
			topNode->clear();
			aboveTopNode->clear();
		}
		
		shared_ptr<Tile> bottomNode = at(lastRoom.getBottom().x, lastRoom.getBottom().y);
		shared_ptr<Tile> belowBottomNode = at(lastRoom.getBottom().x, lastRoom.getBottom().y + 1);
		if (bottomNode != nullptr && belowBottomNode != nullptr && belowBottomNode->getFilled()) {
			bottomNode->clear();
			belowBottomNode->clear();
		}

		shared_ptr<Tile> rightNode = at(lastRoom.getRight().x, lastRoom.getRight().y);
		shared_ptr<Tile> rightOfRightNode = at(lastRoom.getRight().x + 1, lastRoom.getRight().y);
		if (rightNode != nullptr && rightOfRightNode != nullptr && rightOfRightNode->getFilled()) {
			rightNode->clear();
			rightOfRightNode->clear();
		}

		shared_ptr<Tile> leftNode = at(lastRoom.getLeft().x, lastRoom.getLeft().y);
		shared_ptr<Tile> leftOfLeftNode = at(lastRoom.getLeft().x - 1, lastRoom.getLeft().y);
		if (leftNode != nullptr && leftOfLeftNode != nullptr && leftOfLeftNode->getFilled()) {
			leftNode->clear();
			leftOfLeftNode->clear();
		}

		cout << "Sleeping..." << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

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
	return diagonalMovement;
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
			at(x,y)->addConnection(Connection(at(x + xIncrease,y + yIncrease), 0));
			at(x + xIncrease,y + yIncrease)->addConnection(Connection(at(x, y), 0));
		}
	}
}

shared_ptr<Tile> TileMap::getNode(int x, int y) {
	pair<int, int> myPair = std::make_pair(x,y);
	map<pair<int, int>, shared_ptr<Tile>>::iterator it;

	//std::lock_guard<std::mutex> guard(mutex);
	it = nodes.find(myPair);
	if (it != nodes.end())
		return it->second;
	else
		return nullptr;
}

vector<shared_ptr<Tile>> TileMap::getSurroundingNodes(sf::Vector2f position) {
	shared_ptr<Tile> centerTile = findNode(position);

	if (centerTile == nullptr)
		return vector<shared_ptr<Tile>>();

	//get tiles in a square around the tile
	vector<shared_ptr<Tile>> toCheck;
	int centerX = centerTile->getGridX();
	int centerY = centerTile->getGridY();

	appendNodeIfExists(toCheck, centerX + 1, centerY);
	appendNodeIfExists(toCheck, centerX - 1, centerY);
	appendNodeIfExists(toCheck, centerX, centerY + 1);
	appendNodeIfExists(toCheck, centerX, centerY - 1);
	appendNodeIfExists(toCheck, centerX + 1, centerY + 1);
	appendNodeIfExists(toCheck, centerX - 1, centerY + 1);
	appendNodeIfExists(toCheck, centerX + 1, centerY - 1);
	appendNodeIfExists(toCheck, centerX - 1, centerY - 1);

	return toCheck;
}

shared_ptr<Tile> TileMap::findNode(sf::Vector2f position) {
	int spaceBetweenTilesX = TILE_SIZE.x + X_SPACING;
	int spaceBetweenTilesY = TILE_SIZE.y + Y_SPACING;
	
	//get the closest tile
	int roundedX = roundUp(position.x, spaceBetweenTilesX);
	int roundedY = roundUp(position.y, spaceBetweenTilesY);

	//convert world position to tile position
	int tileX = roundedX / spaceBetweenTilesX;
	int tileY = roundedY / spaceBetweenTilesY;

	shared_ptr<Tile> centerTile = getNode(tileX, tileY);

	if (centerTile == nullptr)
		return nullptr;

	//get tiles in a square around the tile
	vector<shared_ptr<Tile>> toCheck;
	int centerX = centerTile->getGridX();
	int centerY = centerTile->getGridY();

	appendNodeIfExistsAndNotFilled(toCheck, centerX + 1, centerY);
	appendNodeIfExistsAndNotFilled(toCheck, centerX - 1, centerY);
	appendNodeIfExistsAndNotFilled(toCheck, centerX, centerY + 1);
	appendNodeIfExistsAndNotFilled(toCheck, centerX, centerY - 1);
	appendNodeIfExistsAndNotFilled(toCheck, centerX + 1, centerY + 1);
	appendNodeIfExistsAndNotFilled(toCheck, centerX - 1, centerY + 1);
	appendNodeIfExistsAndNotFilled(toCheck, centerX + 1, centerY - 1);
	appendNodeIfExistsAndNotFilled(toCheck, centerX - 1, centerY - 1);

	//get highest distance between the centers of the tiles and the given postion
	float lowestDistance = std::numeric_limits<float>::max();
	shared_ptr<Tile> lowestTile = nullptr;
	for (shared_ptr<Tile> tile : toCheck) {
		float xDist = tile->getWorldX() - position.x;
		float yDist = tile->getWorldY() - position.y;
		float xSquared = pow(xDist, 2);
		float ySquared = pow(yDist, 2);

		float distance = sqrt(xSquared + ySquared);
		if (distance < lowestDistance) {
			lowestDistance = distance;
			lowestTile = tile;
		}
	}

	return lowestTile;
}

void TileMap::appendNodeIfExists(vector<shared_ptr<Tile>>& appendTo, int gridX, int gridY) {
	shared_ptr<Tile> tile = getNode(gridX, gridY);
	if (tile != nullptr) {
		appendTo.push_back(tile);
	}
}

void TileMap::appendNodeIfExistsAndNotFilled(vector<shared_ptr<Tile>>& appendTo, int gridX, int gridY) {
	shared_ptr<Tile> tile = getNode(gridX, gridY);
	if (tile != nullptr && !tile->getFilled()) {
		appendTo.push_back(tile);
	}
}

int TileMap::roundUp(int numToRound, int multiple) {
    if (multiple == 0)
        return numToRound;

    int remainder = abs(numToRound) % multiple;
    if (remainder == 0)
        return numToRound;

    if (numToRound < 0)
        return -(abs(numToRound) - remainder);
    else
        return numToRound - multiple - remainder;
}

void TileMap::addNode(int x, int y, shared_ptr<Tile> tile) {
	nodes.insert(std::make_pair(std::make_pair(x,y), tile));
}

void TileMap::addConnections(int x, int y) {
	connectIfValid(x, y, 1, 0); //right
	connectIfValid(x, y, -1, 0); //left
	connectIfValid(x, y, 0, 1); //up
	connectIfValid(x, y, 0, -1); //down

	if (diagonalMovement) {
			connectIfValid(x, y, 1, -1); //bottom right
			connectIfValid(x, y, 1, 1); //top right
			connectIfValid(x, y, -1, -1); //bottom left
			connectIfValid(x, y, -1, 1); //top left
	}
}


void TileMap::clearConnections(int x, int y) {
	shared_ptr<Tile> tile = nullptr;
	auto it = nodes.find(std::make_pair(x, y));
	if (it != nodes.end())
		tile = it->second;

	vector<Connection> connections = adj(x,y);

	for (Connection connection : connections) {
		shared_ptr<Tile> connectedTile = connection.getConnectedTile();

		//remove the original tile from the connected tiles connections
		vector<Connection> otherConnections = connectedTile->getConnections();
		otherConnections.erase(std::remove_if(otherConnections.begin(), otherConnections.end(), 
		[tile](Connection & o) { 
			return o.getConnectedTile() == tile;
		}), otherConnections.end());
	}

	connections.clear();
}

const vector<Connection>& TileMap::adj(int x, int y) {
	return at(x,y)->getConnections();
}

map<pair<int, int>, shared_ptr<Tile>> TileMap::getNodes() {
	return nodes;
}

shared_ptr<Tile> TileMap::at(int x, int y) {
	return getNode(x,y);
}

vector<sf::Vector2f> TileMap::aStar(sf::Vector2f startPos, sf::Vector2f target) {
	/** @breif A list of nodes on the path. */
	vector<sf::Vector2f> path;

	/** @breif A list of nodes still being considered for the path. */
	vector<shared_ptr<Tile>> openList;

	/** @breif A list of nodes not being considered for the path. */
	vector<shared_ptr<Tile>> closedList;

	shared_ptr<Tile> start = findNode(startPos);
	shared_ptr<Tile> end = findNode(target);

	if (start == nullptr || end == nullptr) {
		std::cout << "Error: Tiles given are not on graph..." << std::endl;
		return vector<sf::Vector2f>();
	}

	openList.push_back(start);

	while (!openList.empty()) {

		if (std::find(openList.begin(), openList.end(), end) != openList.end()) {
			shared_ptr<Tile> &currentTile = end;

			//path.push_back(sf::Vector2f(start->getPosition().x + TILE_SIZE.x / 2, start->getPosition().y + TILE_SIZE.y / 2));

			while (currentTile != start) {
				sf::Vector2f tilePos = currentTile->getPosition();
				//path.push_back(sf::Vector2f(tilePos.x + TILE_SIZE.x / 2, tilePos.y + TILE_SIZE.y / 2));
				path.push_back(tilePos);
				currentTile = nodes[std::make_pair(currentTile->getParentX(),currentTile->getParentY())];
			};

			

			return path;
		}

		//get lowest nodes
		vector<shared_ptr<Tile>> lowestVertices = vector<shared_ptr<Tile>>();
		float lowestScore = std::numeric_limits<float>::max();

		for (shared_ptr<Tile> tile : openList)
			if (tile->getF() < lowestScore)
				lowestScore = tile->getF();

		for (shared_ptr<Tile> tile : openList)
			if (tile->getF() == lowestScore)
				lowestVertices.push_back(tile);

		for (shared_ptr<Tile> tile : lowestVertices) {
			//std::lock_guard<std::mutex> lock(mutex);

			closedList.push_back(tile);
			openList.erase(std::remove(openList.begin(), openList.end(), tile), openList.end());

			for (Connection neighbourConnection : tile->getConnections()) {
				shared_ptr<Tile> neighbour = neighbourConnection.getConnectedTile();
				if (std::find(closedList.begin(), closedList.end(), neighbour) == closedList.end()) {

					//the tile distance away
					float tentativeG = tile->getG() + std::abs(tile->getGridX() - neighbour->getGridX()) + std::abs(tile->getGridY() - neighbour->getGridY());

					//heuristic distance the world distance away in pixels
					float tentativeH = std::abs(end->getWorldX() - neighbour->getWorldX()) + std::abs(end->getWorldY() - neighbour->getWorldY());

					//total the distance metrics
					float tentativeScore = (tentativeG + tentativeH);

					//add to open list and update score if not in open list
					if (std::find(openList.begin(), openList.end(), neighbour) == openList.end()) {
						openList.insert(openList.begin(), neighbour);
						neighbour->updateScore(tile->getGridX(), tile->getGridY(), tentativeScore, tentativeG, tentativeH);
					}

					//update the score of the neighbouring vertex
					if (tentativeScore < neighbour->getF()) {
						neighbour->updateScore(tile->getGridX(), tile->getGridY(), tentativeScore, tentativeG, tentativeH);
					}
				}
			}
		}
	}

	return path;
}

vector<shared_ptr<Tile>> TileMap::aStarTiles(shared_ptr<Tile> start, shared_ptr<Tile> end) {

	/** @breif A list of nodes on the path. */
	vector<shared_ptr<Tile>> tilePath;

	/** @breif A list of nodes still being considered for the path. */
	vector<shared_ptr<Tile>> openList;

	/** @breif A list of nodes not being considered for the path. */
	vector<shared_ptr<Tile>> closedList;

	if (start == nullptr || end == nullptr) {
		std::cout << "Error: Tiles given are not on graph..." << std::endl;
		return vector<shared_ptr<Tile>>();
	}

	openList.push_back(start);
	//start->setFillColor(sf::Color::Green);

	while (!openList.empty()) {

		if (std::find(openList.begin(), openList.end(), end) != openList.end()) {
			shared_ptr<Tile> currentTile = end;

			while (currentTile != start) {
				tilePath.push_back(currentTile);
				//start->setFillColor(sf::Color::Yellow);
				currentTile = nodes[std::make_pair(currentTile->getParentX(),currentTile->getParentY())];
			};

			return tilePath;
		}

		//get lowest nodes
		vector<shared_ptr<Tile>> lowestVertices = vector<shared_ptr<Tile>>();
		float lowestScore = std::numeric_limits<float>::max();

		for (shared_ptr<Tile> tile : openList)
			if (tile->getF() < lowestScore)
				lowestScore = tile->getF();

		for (shared_ptr<Tile> tile : openList)
			if (tile->getF() == lowestScore)
				lowestVertices.push_back(tile);

		for (shared_ptr<Tile> tile : lowestVertices) {
			//std::lock_guard<std::mutex> lock(mutex);

			closedList.push_back(tile);
			//tile->setFillColor(sf::Color::Red);
			openList.erase(std::remove(openList.begin(), openList.end(), tile), openList.end());

			for (Connection neighbourConnection : tile->getConnections()) {
				shared_ptr<Tile> neighbour = neighbourConnection.getConnectedTile();
				//if not found in the closed list
				if (std::find(closedList.begin(), closedList.end(), neighbour) == closedList.end()) {

					//the tile distance away
					float tentativeG = tile->getG() + std::abs(tile->getGridX() - neighbour->getGridX()) + std::abs(tile->getGridY() - neighbour->getGridY());

					//heuristic distance the world distance away in pixels
					float tentativeH = std::abs(end->getWorldX() - neighbour->getWorldX()) + std::abs(end->getWorldY() - neighbour->getWorldY());

					//total the distance metrics
					float tentativeScore = (tentativeG + tentativeH);

					//add to open list and update score if not in open list
					if (std::find(openList.begin(), openList.end(), neighbour) == openList.end()) {
						openList.insert(openList.begin(), neighbour);
						//neighbour->setFillColor(sf::Color::Green);
						neighbour->updateScore(tile->getGridX(), tile->getGridY(), tentativeScore, tentativeG, tentativeH);
					}

					//update the score of the neighbouring vertex
					if (tentativeScore < neighbour->getF()) {
						neighbour->updateScore(tile->getGridX(), tile->getGridY(), tentativeScore, tentativeG, tentativeH);
					}
				}
			}
		}
	}

	return tilePath;
}
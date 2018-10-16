#pragma once
#ifndef MAP_H
#define MAP_H

#include "stdafx.h"
#include "Tile.h"
#include "TileMap.h"

using std::string;
using std::shared_ptr;
using std::vector;
using std::pair;
using std::map;
using std::cout;
using std::endl;

/// <summary>
/// A map containing tiles.
/// </summary>
class TileMap {
private:

	/// <summary> X spacing between tiles. </summary>
	const int X_SPACING = 0;

	/// <summary> Y spacing between tiles. </summary>
	const int Y_SPACING = 0;

	/// <summary> Whether the tiles should allow diagonal movement. </summary>
	const bool DIAGONAL_MOVEMENT = true;

	/// <summary> The size of a tile. </summary>
	const sf::Vector2f TILE_SIZE = sf::Vector2f(20, 20);

	/// <summary> The default size of the tile. </summary>
	const int DEFAULT_SIZE = 32;

	/// <summary> The size of the map. </summary>
	int size;

	/// <summary> Where to load/save the map. </summary>
	string fileLocation;

	/// <summary> A list of nodes still being considered for the path. </summary>
	vector<shared_ptr<Tile>> openList;

	/// <summary> A list of nodes not being considered for the path. </summary>
	vector<shared_ptr<Tile>> closedList;

	/// <summary> A list of nodes on the path. </summary>
	vector<sf::Vector2f> path;

	/// <summary> The nodes stored in the graph. </summary>
	map<pair<int, int>, shared_ptr<Tile>> nodes;

	map<pair<int, int>, vector<shared_ptr<Tile>>> adjList;

	void connectIfValid(int x, int y, int xIncrease, int yIncrease);
	shared_ptr<Tile> getNode(int x, int y);
	int roundUp(int numToRound, int multiple);
public:
	/// <summary>
	///	Constructor for a tilemap, the tiles are loaded and put into the graph object provided.
	/// Reference constructor: https://stackoverflow.com/questions/6576109/initialising-reference-in-constructor-c
	/// </summary>
	/// <param name="fileLocation">The location to save/load the map from.</param>
	/// <param name="graph">The graph object to pass the loaded tiles to.</param>
	TileMap(string fileLocation);

	TileMap(string fileLocation, int size, int roomSize, int targetRoomCount, int corridorSize, int roomDistance);

	/// <summary> Saves the tile to the given fileLocation. </summary>
	void save();

	/// <summary> Gets the size of the graph.</summary>
	int getSize();

	/// <summary> Gets the y spacing in pixels between tiles. </summary>
	int getYSpacing();

	/// <summary> Gets the x spacing in pixels between tiles. </summary>
	int getXSpacing();

	/// <summary> Gets the size of an individual tile. </summary>
	sf::Vector2f getTileSize();

	/// <summary> Whether this map can be traversed with diagonal movement. </summary>
	bool usingDiagonalMovement();

	/// <summary> Gets the width of a tile. </summary>
	int getWidth();

	/// <summary> Gets the height of a tile. </summary>
	int getHeight();

	void appendNodeIfExistsAndNotFilled(vector<shared_ptr<Tile>>& appendTo, int gridX, int gridY);
	void appendNodeIfExists(vector<shared_ptr<Tile>>& appendTo, int gridX, int gridY);
	vector<shared_ptr<Tile>> getSurroundingNodes(sf::Vector2f position);
	shared_ptr<Tile> findNode(sf::Vector2f position);
	void addNode(int x, int y, shared_ptr<Tile> node);
	void clear();
	void addConnections(int x, int y);
	void clearConnections(int x, int y);
	vector<shared_ptr<Tile>>& adj(int x, int y);
	bool inOpenList(shared_ptr<Tile> node);
	bool inClosedList(shared_ptr<Tile> node);
	const vector<shared_ptr<Tile>>& getOpenList();
	const vector<shared_ptr<Tile>>& getClosedList();
	const vector<sf::Vector2f>& getPathList();
	map<pair<int, int>, shared_ptr<Tile>> getNodes();
	shared_ptr<Tile> at(int x, int y);
	vector<sf::Vector2f> aStar(sf::Vector2f startPos, sf::Vector2f target);
};

#endif
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

/** @breif A map containing tiles. */
class TileMap {
private:

	vector<int> tileInts;
	const int width = 128;
	const int height = 128;

	/** @breif X spacing between tiles. */
	const int X_SPACING = 0;

	/** @breif Y spacing between tiles. */
	const int Y_SPACING = 0;

	/** @breif Whether the tiles should allow diagonal movement. */
	bool diagonalMovement = true;

	/** @breif The size of a tile. */
	const sf::Vector2f TILE_SIZE = sf::Vector2f(20, 20);

	/** @breif The default size of the tile. */
	const int DEFAULT_SIZE = 32;

	/** @brief The size of the map in tiles.  */
	int size;

	/** @breif Where to load/save the map. */
	string fileLocation;

	/** @breif The nodes stored in the graph. */
	map<pair<int, int>, shared_ptr<Tile>> nodes;

	/**
	 * @brief Connects a node relative to the grid x/y position based on the xIncrease and yIncrease
	 * 
	 * @param x the base node grid x
	 * @param y the base node grid y
	 * @param xIncrease the amount of x nodes away from the base
	 * @param yIncrease the amount of y nodes away from the base 
	 */
	void connectIfValid(int x, int y, int xIncrease, int yIncrease);

	/**
	 * @brief Gets a node at a certain grid position.
	 * 
	 * @param x grid x
	 * @param y grid y
	 * @return shared_ptr<Tile> the node at the given position 
	 */
	shared_ptr<Tile> getNode(int x, int y);

	/**
	 * @brief Rounds a number up to a multiple.
	 * 
	 * @param numToRound the number to round
	 * @param multiple the multiple to round to
	 * @return int the rounded number
	 */
	int roundUp(int numToRound, int multiple);

	/** @brief Fills the map with empty tiles. */
	void initMap();
public:
	vector<int> getTileInts();

	/**
	 * @brief Construct a new Tile Map object from a file
	 * @param fileLocation the location to save/load the map from
	 */
	TileMap(string fileLocation);

	/**
	 * @brief Construct a new Tile Map object with a randomly generated map.
	 * 
	 * @param fileLocation the location to save/load the map from
	 * @param targetRoomCount the amount of rooms to put in the map
	 */
	TileMap(string fileLocation, unsigned int targetRoomCount);

	/**
	 * @brief Construct an empty Tile Map object
	 * 
	 * @param fileLocation the location to save the map
	 * @param size the size of the map in tiles
	 */
	TileMap(string fileLocation, int size);

	/**
	 * @brief Replaces the map with a randomly generated one.
	 * 
	 * @param targetRoomCount the amount of rooms to put in the map
	 */
	void generate(unsigned int targetRoomCount);

	/** @breif Saves the tile to the given fileLocation. */
	void save();

	/** @breif Gets the size of the graph.*/
	int getSize();

	/** @breif Gets the y spacing in pixels between tiles. */
	int getYSpacing();

	/** @breif Gets the x spacing in pixels between tiles. */
	int getXSpacing();

	/** @breif Gets the size of an individual tile. */
	sf::Vector2f getTileSize();

	/** @breif Whether this map can be traversed with diagonal movement. */
	bool usingDiagonalMovement();

	/** @breif Gets the width of a tile. */
	int getWidth();

	/** @breif Gets the height of a tile. */
	int getHeight();

	/**
	 * @brief Appends the node to a given list if it exists and isn't filled.
	 * 
	 * @param appendTo the list to append the node to
	 * @param gridX the gridX of the node to append
	 * @param gridY the gridY of the node to append
	 */
	void appendNodeIfExistsAndNotFilled(vector<shared_ptr<Tile>>& appendTo, int gridX, int gridY);

	/**
	 * @brief Append the node to the given list if it exists.
	 * 
	 * @param appendTo the list to append the node to
	 * @param gridX the gridX of the node to append
	 * @param gridY the gridY of the node to append
	 */
	void appendNodeIfExists(vector<shared_ptr<Tile>>& appendTo, int gridX, int gridY);

	/**
	 * @brief Gets a list of nodes surrounding the closet tile to a given postion.
	 * 
	 * @param position the position of the starting node
	 * @return vector<shared_ptr<Tile>> the tiles surrounding the node that was found.
	 */
	vector<shared_ptr<Tile>> getSurroundingNodes(sf::Vector2f position);

	/**
	 * @brief Finds the closest node to a given world position.
	 * 
	 * @param position the position to search for
	 * @return shared_ptr<Tile> the node that was the closest to the given position.
	 */
	shared_ptr<Tile> findNode(sf::Vector2f position);

	/**
	 * @brief Adds a node to the graph.
	 * 
	 * @param x the nodes grid x position
	 * @param y the nodes grid y position
	 * @param node the node to add to the graph.
	 */
	void addNode(int x, int y, shared_ptr<Tile> node);

	/**
	 * @brief Connects nearby nodes to a node at the given grid position
	 * 
	 * @param x the grid x coordinate
	 * @param y the grid y coordinate
	 */
	void addConnections(int x, int y);

	/**
	 * @brief Clears the connections to the node at the given position
	 * 
	 * @param x the grid x coordinate
	 * @param y the grid y coordinate
	 */
	void clearConnections(int x, int y);

	/**
	 * @brief Gets a list of adjacent/connected nodes to the node at the given position.
	 * 
	 * @param x the grid x coordinate
	 * @param y the grid y coordinate
	 * @return vector<shared_ptr<Tile>>& A list of adjacent nodes
	 */
	const vector<Connection>& adj(int x, int y);

	/**
	 * @brief Get the Nodes in this map
	 * 
	 * @return map<pair<int, int>, shared_ptr<Tile>> the nodes in the map
	 */
	map<pair<int, int>, shared_ptr<Tile>> getNodes();

	/**
	 * @brief Gets the node at the given grid position.
	 * 
	 * @param x the grid x coordinate
	 * @param y the grid y coordinate
	 * @return shared_ptr<Tile> the node at the given postion.
	 */
	shared_ptr<Tile> at(int x, int y);

	/**
	 * @brief Runs the A* pathfinding algorithm to find the best path through the map.
	 * 
	 * @param startPos the starting position in world coordinates
	 * @param target the target position in world cooridnates
	 * @return vector<sf::Vector2f> the points in the path to get from the starting position to the target position
	 */
	vector<sf::Vector2f> aStar(sf::Vector2f startPos, sf::Vector2f target);

	/**
	 * @brief Runs the A* pathfinding algorithm to find the best path through the map.
	 * 
	 * @param start the starting tile
	 * @param target the target tile
	 * @return shared_ptr<Tile> tiles to get from the start to the end
	 */
	vector<shared_ptr<Tile>> aStarTiles(shared_ptr<Tile> start, shared_ptr<Tile> target);
};

#endif
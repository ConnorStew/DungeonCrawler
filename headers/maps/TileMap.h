#pragma once
#ifndef MAP_H
#define MAP_H

#include "stdafx.h"
#include "Tile.h"
#include "Graph.h"

using std::string;
using std::shared_ptr;

/// <summary>
/// A map containing tiles.
/// </summary>
class TileMap {
private:

	/// <summary> X spacing between tiles. </summary>
	const int X_SPACING = 2;

	/// <summary> Y spacing between tiles. </summary>
	const int Y_SPACING = 2;

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

	/// <summary> The graph to add tiles to. </summary>
	Graph<Tile>* graph;

public:
	/// <summary>
	///	Constructor for a tilemap, the tiles are loaded and put into the graph object provided.
	/// Reference constructor: https://stackoverflow.com/questions/6576109/initialising-reference-in-constructor-c
	/// </summary>
	/// <param name="fileLocation">The location to save/load the map from.</param>
	/// <param name="graph">The graph object to pass the loaded tiles to.</param>
	TileMap(string fileLocation, Graph<Tile>* graph);

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
};

#endif
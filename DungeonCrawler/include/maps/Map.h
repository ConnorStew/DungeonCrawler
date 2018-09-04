#pragma once
#ifndef MAP_H
#define MAP_H

#include "stdafx.h"

using std::string;

class Map {
private:

	/// <summary> X spacing between tiles. </summary>
	const int xSpacing = 2;

	/// <summary> Y spacing between tiles. </summary>
	const int ySpacing = 2;

	/// <summary> The size of the map. </summary>
	int size;

	/// <summary> Where to load/save the map. </summary>
	string fileLocation;

public:
	Map(int size, string fileLocation);
};

#endif
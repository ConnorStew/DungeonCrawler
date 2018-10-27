#pragma once
#ifndef CONNECTION_H
#define CONNECTION_H

#include "stdafx.h"

class Tile;

using std::shared_ptr;
using std::vector;

class Connection {
	private:
        shared_ptr<Tile> connectedTile;
        int weight;
		
	public:
        Connection(shared_ptr<Tile> connectedTile, int weight);

        int getWeight();
        shared_ptr<Tile> getConnectedTile();
};

#endif

#include "stdafx.h"
#include "Connection.h"

using std::shared_ptr;
using std::vector;

Connection::Connection(shared_ptr<Tile> connectedTile, int weight) {
    this->connectedTile = connectedTile;
    this->weight = weight;
}

int Connection::getWeight() {
    return weight;
}

shared_ptr<Tile> Connection::getConnectedTile() {
    return connectedTile;
}
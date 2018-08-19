#pragma once
#ifndef TILE_H
#define TILE_H

class Tile {
	private:
		float width, height, worldX, worldY;
		int gridX, gridY;
		bool filled;
	public:
		Tile();

};

#endif
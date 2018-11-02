#pragma once
#ifndef ROOM_H
#define ROOM_H

#include "stdafx.h"
#include "TileMap.h"

class Room {
private:
    /** @brief The center of the room. */
    sf::Vector2f centerPoint;

protected:
    /** @brief The rooms top left grid x position. */
    int gridX;

    /** @brief The rooms top left grid y position. */
    int gridY;

    /** @breif The rooms grid x plus its width. */
    int gridXAndWidth;

    /** @breif The rooms grid y plus its height. */
    int gridYAndHeight;

    /** @brief The width of the room. */
    int width;

    /** @brief The height of the room. */
    int height;

public:
    /**
     * @brief Construct a new Room object
     * 
     * @param gridX the rooms top left grid x coordinate
     * @param gridY the rooms top left grid y coordinate 
     * @param width the width of the room
     * @param height the height of the room
     */
    Room(int gridX, int gridY, int width, int height);

    /**
     * @brief Checks if this rooms is near any filled tiles on the map.
     * 
     * @param minDistance the minimum distance away from any filled tile
     * @param rooms the other rooms
     * @return true the room is near a filled tile
     * @return false the room isn't near a filled tile
     */
    bool nearOtherRooms(int minDistance, std::vector<Room> rooms);

    /**
     * @brief Checks if the room is overlapping/near the side of the tile map.
     * 
     * @param minDistance the minumum distance away from the side
     * @param width the width of the map
     * @param height the height of the map
     * @return true the room is overlapping/near the side of the map
     * @return false the room isn't overlapping/near the side of the map 
     */
    bool nearSide(int minDistance, int width, int height);

    /**
     * @brief Checks if a grid location is contained in this room (including walls)
     * 
     * @param x the x location to check
     * @param y the y location to check
     * @return true the given location is in the room 
     * @return false the given location is outside of the room
     */
    bool inside(int x, int y);

    sf::Vector2f getCenter();
    sf::Vector2f getTop();
    sf::Vector2f getBottom();
    sf::Vector2f getLeft();
    sf::Vector2f getRight();
    int getX();
    int getY();
    int getWidth();
    int getHeight();
};

#endif
#include "Room.h"
#include "Tile.h"

Room::Room(int gridX, int gridY, int width, int height) {
    this->gridX = gridX;
    this->gridY = gridY;
    this->width = width;
    this->height = height;
    this->gridXAndWidth = gridX + width;
    this->gridYAndHeight = gridY + height;
    this->centerPoint = sf::Vector2f(std::abs((gridX + (gridX + width)) / 2),std::abs((gridY + (gridY + height)) / 2));
}

bool Room::nearOtherRooms(int minDistance, std::vector<Room> rooms) {
    for (Room room : rooms) {
        int checkX = gridX - minDistance;
        int checkY = gridY - minDistance;
        int maxX = gridXAndWidth + minDistance;
        int maxY = gridYAndHeight + minDistance;

        if(checkX <= room.gridXAndWidth && maxX >= room.gridX && checkY <= room.gridYAndHeight && maxY >= room.gridY)
            return true;
    }

    return false;
}

bool Room::nearSide(int minDistance, int mapWidth, int mapHeight) {
    int maxX = mapWidth - minDistance;
    int minX = minDistance;
    int maxY = mapHeight - minDistance;
    int minY = minDistance;

	return (gridX <= minX || gridXAndWidth >= maxX || gridY <= minY || gridYAndHeight >= maxY);
}

bool Room::inside(int x, int y) {
    return (x >= gridX && x <= gridXAndWidth && y >= gridY && y <= gridYAndHeight);
}

sf::Vector2f Room::getCenter() {
    return centerPoint;
}

int Room::getX() {
    return gridX;
}

int Room::getY() {
    return gridY;
}

int Room::getWidth() {
    return width;
}

int Room::getHeight() {
    return height;
}

sf::Vector2f Room::getTop() {
    return sf::Vector2f(gridX + width / 2, gridY);
}

sf::Vector2f Room::getBottom() {
    return sf::Vector2f(gridX + width / 2, gridY + height);
}

sf::Vector2f Room::getLeft() {
    return sf::Vector2f(gridX, gridY + height / 2);
}

sf::Vector2f Room::getRight() {
    return sf::Vector2f(gridX + width, gridY + height / 2);
}
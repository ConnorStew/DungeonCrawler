#pragma once
#ifndef WORLD_H
#define WORLD_H

#include "stdafx.h"
#include "TileMap.h"
#include "Player.h"
#include "Entity.h"

class World {
private:
    static float delta;
    sf::RenderWindow* window;
    TileMap* gameMap;
    Entity* skeleton;
    Entity* skeleton2;
    Player* player;
    vector<sf::Vertex> debugLines;

    /** @brief Renders the world. */
    void render();
    /** @brief Updates the world. */
    void update();
public:
    World();
    ~World();
};

#endif
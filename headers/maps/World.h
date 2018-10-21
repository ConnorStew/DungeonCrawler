#pragma once
#ifndef WORLD_H
#define WORLD_H

#include "stdafx.h"
#include "TileMap.h"
#include "Player.h"
#include "Entity.h"

class World {
private:
    const bool DRAW_PATH = true;

    static float delta;
    sf::RenderWindow* window;
    TileMap* gameMap;
    sf::Clock* timer;
    Entity* skeleton;
    Entity* skeleton2;
    Player* player;

    /** @brief Renders the world. */
    void render();
    /** @brief Updates the world. */
    void update();
public:
    static float getDeltaTime();
    
    World();
    ~World();
};

#endif
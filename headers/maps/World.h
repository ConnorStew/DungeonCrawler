#pragma once
#ifndef WORLD_H
#define WORLD_H

#include "stdafx.h"
#include "TileMap.h"
#include "Player.h"
#include "Entity.h"

class World {
private:
    const bool DRAW_PATH = false;

    static float delta;
    sf::RenderWindow* window;
    TileMap* gameMap;
    sf::Clock* timer;
    Entity* skeleton;
    Entity* skeleton2;
    Player* player;

    void render();
    void update();
public:
    static float getDeltaTime();
    
    World();
    ~World();
};

#endif
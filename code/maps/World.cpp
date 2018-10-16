#include "World.h"
#include "MoveTo.h"
#include "Player.h"
#include "Entity.h"

float World::delta = 0;

World::World() {
    gameMap = new TileMap("res/map.json");
    window = new sf::RenderWindow(sf::VideoMode(gameMap->getWidth(), gameMap->getHeight()), "Dungeon Crawler");
    timer = new sf::Clock();
    player = new Player("res/mage.png", "Player", 5, 6, gameMap->getTileSize().x, gameMap->getTileSize().y, gameMap, 1);
    skeleton = new Entity("res/skeleton.png", "Skeleton", 28, 28, gameMap->getTileSize().x, gameMap->getTileSize().y, gameMap, 1);
    skeleton2 = new Entity("res/skeleton.png", "Skeleton", 6, 6, gameMap->getTileSize().x, gameMap->getTileSize().y, gameMap, 1);
    // player.setRoutine(new Sequence {

	// 	new MoveTo(21, 25),
	// 	new MoveTo(21, 24),
	// 	new Sequence {
	// 		new MoveTo(8, 28),
	// 		new MoveTo(16, 1),
	// 		new MoveTo(2, 22)
	// 	}gameMap
	// 	//new Wander()
	// });

	// skeleton.setRoutine(new Sequence{
	// 	new MoveTo(12,12),
	// 	new Wander()
	// });

	skeleton->setRoutine(new MoveTo(player));
    skeleton2->setRoutine(new MoveTo(skeleton));

	while (window->isOpen()) {
		window->clear();

		sf::Event event;
		while (window->pollEvent(event))
			if (event.type == sf::Event::Closed)
				window->close();

		//framerate
		World::delta = 1.f / timer->getElapsedTime().asMilliseconds();
		timer->restart();
		//cout << std::to_string(delta) << endl;

		player->update();
		skeleton->update();
        skeleton2->update();
		
		update();
		render();
	}
}

World::~World() {
    delete window;
    delete gameMap;
    delete timer;
    delete skeleton;
    delete player;
}

float World::getDeltaTime() {
    return World::delta;
}

void World::update() {
	sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
	bool eDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
	bool qDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
	bool controlDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
	bool yDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y);
	bool sDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
	bool pDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);
	bool lmbDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

	if (lmbDown) {
		shared_ptr<Tile> playerTile = gameMap->findNode(player->getCenter());
		shared_ptr<Tile> mouseTile = gameMap->findNode(mousePos);

		if (playerTile != nullptr && mouseTile != nullptr)
			player->setRoutine(new MoveTo(mousePos));
	}

	if (controlDown && sDown)
		gameMap->save();

	for (int x = 0; x < gameMap->getSize(); x++) {
		for (int y = 0; y < gameMap->getSize(); y++) {
			shared_ptr<Tile> tile = gameMap->at(x,y);

			if (eDown && tile->getGlobalBounds().contains(mousePos)) {
				tile->setFilled(true);
				gameMap->clearConnections(x, y);
			}

			if (qDown && tile->getGlobalBounds().contains(mousePos)) {
				tile->setFilled(false);
				gameMap->addConnections(x, y);
			}

			if (pDown) {
				gameMap->findNode(player->getPosition());
			}

			if (yDown && tile->getGlobalBounds().contains(mousePos)) {
				std::cout << *tile << std::endl; //dereference pointer
			}

			if (spaceDown && tile->getGlobalBounds().contains(mousePos)) {
				//draw connections
				vector<shared_ptr<Tile>> &connections = gameMap->adj(x, y);
				for (unsigned int i = 0; i < connections.size(); i++) {
					shared_ptr<Tile> connectedTile = connections.at(i);
					sf::Vertex line[] =
					{
						sf::Vertex(sf::Vector2f(tile->getWorldX() + gameMap->getTileSize().x / 2, tile->getWorldY() + gameMap->getTileSize().y / 2), sf::Color(0, 255, 221,255)),
						sf::Vertex(sf::Vector2f(connectedTile->getWorldX() + gameMap->getTileSize().x / 2, connectedTile->getWorldY() + gameMap->getTileSize().y / 2),  sf::Color(0, 255, 221,255))
					};

					window->draw(line, 2, sf::Lines);
				}
			}
		}
	}
}

void World::render() {

	for (auto const& tileEntry : gameMap->getNodes()) {
		shared_ptr<Tile> tile = tileEntry.second;
		tile->setFillColor(sf::Color(100, 100, 100, 255)); //default color

		if (tile->getFilled())
			tile->setFillColor(sf::Color::White);
	}

    // for (shared_ptr<Tile> tile : gameMap->getSurroundingNodes(player->getPosition())) {
    //     tile->setFillColor(sf::Color::Red);
    // }

    //gameMap->findNode(player->getPosition())->setFillColor(sf::Color::Green);

	if (DRAW_PATH) {
		for (shared_ptr<Tile> tile : gameMap->getOpenList())
			tile->setFillColor(sf::Color::Green);

		for (shared_ptr<Tile> tile : gameMap->getClosedList())
			tile->setFillColor(sf::Color::Red);
	}

	for (auto const& tileEntry : gameMap->getNodes())
	window->draw(*tileEntry.second);

	if (DRAW_PATH) {

		sf::Vector2f lastPoint = sf::Vector2f(0,0);
		for (sf::Vector2f pathPoint : gameMap->getPathList()) {
			sf::CircleShape shape(5);
			shape.setFillColor(sf::Color::Magenta);
			shape.setPosition(sf::Vector2f(pathPoint.x - 5, pathPoint.y - 5));
			window->draw(shape);

			if (lastPoint.x != 0 && lastPoint.y != 0) {
				sf::Vertex line[] = {
					sf::Vertex(lastPoint),
					sf::Vertex(pathPoint)
				};
				window->draw(line, 2, sf::Lines);
			}
			lastPoint = pathPoint;
		}
	}

	//window->draw(player.getBoundingBox());
	window->draw(*player);
	window->draw(*skeleton);
    window->draw(*skeleton2);
	window->display();
}
#include "World.h"
#include "MoveTo.h"
#include "Player.h"
#include "Entity.h"
#include <thread>
#include <mutex>
#include "mingw.thread.h"
#include "mingw.mutex.h"

using namespace std::chrono_literals;
float World::delta = 0;
sf::View view;

sf::Clock timer;
int m_frames{0};

const int FILLED_TILE = 0;
const int OPEN_TILE = 1;
const int WIDTH = 640;
const int HEIGHT = 480;
const int TILE_AMOUNT = 128;
vector<int> tileInts;
shared_ptr<Tile> lastPlayerTile = nullptr;

World::World() {
	gameMap = new TileMap("res/proceduralMap.json", 1000);
    window = new sf::RenderWindow(sf::VideoMode(WIDTH, HEIGHT), "Dungeon Crawler");
    player = new Player("res/mage.png", "Player", 5, 6, gameMap->getTileSize().x, gameMap->getTileSize().y, gameMap, 2);
    skeleton = new Entity("res/skeleton.png", "Skeleton", 28, 28, gameMap->getTileSize().x, gameMap->getTileSize().y, gameMap, 1);
    skeleton2 = new Entity("res/skeleton.png", "Skeleton", 6, 6, gameMap->getTileSize().x, gameMap->getTileSize().y, gameMap, 1);
	view.setSize(sf::Vector2f(500,500));
	//gameMap->generate(4,4,3,2);

	const std::chrono::nanoseconds FRAMETIME(16666667);		// Equal to 16.66ms or 1000/60
	using clock = std::chrono::high_resolution_clock;
	
	auto gameTimeStart  = clock::now();						// Total runtime of the program
	auto frameTimeStart = clock::now();		
	std::chrono::nanoseconds accumulator(0ns);				// Time passed since the last update

	const int MAX_SKIP = 5;
	int loopCount = 0;

	for (int i = 0; i < 128; i++)
		for (int j = 0; j < 128; j++)
			tileInts.push_back(1);

	while (window->isOpen())
	{
		
		if (clock::now() - frameTimeStart > std::chrono::milliseconds(1))
		{
			auto deltaTime = clock::now() - frameTimeStart;		// Time passed for last loop
			frameTimeStart = clock::now();						// Reset
			accumulator += deltaTime;							// Add time passed last loop
		}
									

		sf::Event event;
		while (window->pollEvent(event))
			if (event.type == sf::Event::Closed)
				window->close();
		
		while (accumulator >= FRAMETIME)					// Update if enough time has passed
		{
			accumulator -= FRAMETIME;						
			update();
			
			if (loopCount >= MAX_SKIP) {
				loopCount = 0;
				accumulator = std::chrono::milliseconds(0);
				break;
			}
		}
		render();		
		m_frames++;
		// For checkign what the fps was
		auto timePassed = clock::now() - gameTimeStart;
		auto millisPassed = std::chrono::duration_cast<std::chrono::milliseconds>(timePassed);
		// std::cout << "milliseconds passed: " << millisPassed.count() << std::endl;
		// std::cout << "frames passed: " << m_frames << std::endl;
		std::cout << "frames per second: "	<< (float) m_frames / ( (float)millisPassed.count() /1000.f) << endl;
		// Render ever loop
	}


		// if (drawn) {
		// 	//sf::sleep(sf::milliseconds(0.01f));
		// } else {
		// 	// Draw everything.
			
		// 	drawn = true;
		// }
}

	// fpsTimer.restart();
    // int loops;
	// while (window->isOpen()) {
    //     window->clear();

	// 	sf::Event event;
	// 	while (window->pollEvent(event))
	// 		if (event.type == sf::Event::Closed)
	// 			window->close();

	// 	cout << fpsTimer.getElapsedTime().asSeconds() << endl;
	// 	fpsTimer.restart();
    //     loops = 0;
    //     while (getSystemTime() > nextTick && loops < MAX_FRAMESKIP) {
    //         update();

    //         nextTick += SKIPPED_TICKS;
    //         loops++;
    //     }

	// 	render();
    //     //render(float(getSystemTime() + SKIPPED_TICKS - nextTick ) / float(SKIPPED_TICKS));
    // }

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

	// skeleton->setRoutine(new MoveTo(player));
    // skeleton2->setRoutine(new MoveTo(skeleton));

World::~World() {
    delete gameMap;
    delete skeleton;
    delete player;
}

void World::update() {
	// sf::Vector2f mousePos = window->mapPixelToCoords(sf::Mouse::getPosition(*window));
	// bool spaceDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);
	// bool eDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E);
	// bool qDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q);
	// bool controlDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);
	// bool yDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Y);
	// bool sDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
	// bool pDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P);
	// bool lmbDown = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
	// bool rArrowDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right);
	// bool lArrowDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left);
	// bool uArrowDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up);
	// bool dArrowDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down);

	player->update();
	// if (lmbDown) {
	// 	// shared_ptr<Tile> playerTile = gameMap->findNode(player->getCenter());
	// 	shared_ptr<Tile> mouseTile = gameMap->findNode(mousePos);

	// 	// if (playerTile != nullptr && mouseTile != nullptr)
	// 	// 	player->setRoutine(new MoveTo(mousePos));
	// 	if (mouseTile != nullptr)
	// 		cout << *mouseTile << endl;
		
	// 	lmbDown = false;
	// }

	// if (controlDown && sDown)
	// 	gameMap->save();

	// for (int x = 0; x < gameMap->getSize(); x++) {
	// 	for (int y = 0; y < gameMap->getSize(); y++) {
	// 		shared_ptr<Tile> tile = gameMap->at(x,y);

	// 		if (eDown && tile->getGlobalBounds().contains(mousePos)) {
	// 			tile->fill();
	// 			gameMap->clearConnections(x, y);
	// 		}

	// 		if (qDown && tile->getGlobalBounds().contains(mousePos)) {
	// 			tile->clear();
	// 			gameMap->addConnections(x, y);
	// 		}

	// 		if (pDown) {
	// 			gameMap->findNode(player->getPosition());
	// 		}

	// 		if (yDown && tile->getGlobalBounds().contains(mousePos)) {
	// 			std::cout << *tile << std::endl; //dereference pointer
	// 		}

	// 		if (spaceDown && tile->getGlobalBounds().contains(mousePos)) {
	// 			//draw connections
	// 			vector<Connection> connections = tile->getConnections();

	// 			for (Connection connection : connections) {
	// 				shared_ptr<Tile> connectedTile = connection.getConnectedTile();
	// 				sf::Vector2f connectedCenter = sf::Vector2f(tile->getWorldX() + gameMap->getTileSize().x / 2, tile->getWorldY() + gameMap->getTileSize().y / 2);
	// 				sf::Vector2f tileCenter = sf::Vector2f(connectedTile->getWorldX() + gameMap->getTileSize().x / 2, connectedTile->getWorldY() + gameMap->getTileSize().y / 2);
	// 				debugLines.push_back(sf::Vertex(tileCenter, sf::Color(0, 255, 221,255)));
	// 				debugLines.push_back(sf::Vertex(connectedCenter,  sf::Color(0, 255, 221,255)));
	// 			}
	// 		}
	// 	}
	// }
}

void World::render() {
	window->clear();

	view.setCenter(player->getCenter().x, player->getCenter().y);
	window->setView(view);

	sf::Vector2f viewWorldPosition = view.getCenter();
	float rectWidth = view.getSize().x;
	float rectHeight = view.getSize().y;
	float rectX = viewWorldPosition.x - rectWidth / 2;
	float rectY = viewWorldPosition.y - rectHeight / 2;

	int drawCount = 0;

	shared_ptr<Tile> playerTile = gameMap->findNode(player->getPosition());
	if (playerTile != nullptr) {
		lastPlayerTile = playerTile;
	} else {
		playerTile = lastPlayerTile;
	}

	int bufferAmount = 30;
	for (int x = playerTile->getGridX() - bufferAmount; x < playerTile->getGridX() + bufferAmount; x++) {
		for (int y = playerTile->getGridY() - bufferAmount; y < playerTile->getGridY() + bufferAmount; y++) {
			shared_ptr<Tile> tile = gameMap->at(x,y);
			
			if (tile == nullptr)
				continue;

			float tileX = tile->getWorldX();
			float tileY = tile->getWorldY();

			if (tileX > rectX - gameMap->getTileSize().x && tileX < rectX + rectWidth && tileY > rectY - gameMap->getTileSize().y && tileY < rectY + rectHeight) {
				window->draw(*tile);
				window->draw(tile->getBorder());
				drawCount++;
			}
		}
	}
	// for (auto const& tileEntry : gameMap->getNodes()) {
	// 	shared_ptr<Tile> tile = tileEntry.second;
		
	// 	float tileX = tile->getWorldX();
	// 	float tileY = tile->getWorldY();

	// 	if (tileX > rectX && tileX < rectX + rectWidth - gameMap->getTileSize().x && tileY > rectY && tileY < rectY + rectHeight - gameMap->getTileSize().y) {
	// 		window->draw(*tile);
	// 		window->draw(tile->getBorder());
	// 		drawCount++;
	// 	}
	// }



	cout << drawCount << endl;

	if (debugLines.size() > 0) {
		sf::Vertex lines[debugLines.size()];
		std::copy(debugLines.begin(), debugLines.end(), lines);
		window->draw(lines, debugLines.size(), sf::Lines);
		debugLines.clear();
	}

	// sf::Sprite sprite(texture.getTexture());
	// window->draw(sprite);
	//window->draw(player.getBoundingBox());
	window->draw(*player);
	window->draw(*skeleton);
    window->draw(*skeleton2);
	window->display();
}
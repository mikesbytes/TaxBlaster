#include "game.h"
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>

Game::Game()
{
	bulletTex.loadFromFile("res/bullet.png");
	splashTex.loadFromFile("res/splash.png");
	shipTex.loadFromFile("res/ship.png");
	enemyTex.loadFromFile("res/tax.png");
	
	arialFont.loadFromFile("res/arialbd.ttf");
	
	splashSprite.setTexture(splashTex);

	shipSprite.setTexture(shipTex);
	shipSprite.setPosition(368,504);

	livesText.setFont(arialFont);
	livesText.setCharacterSize(16);
	livesText.setPosition(4, 580);
	livesText.setColor(sf::Color::Black);
	
	scoreText.setFont(arialFont);
	scoreText.setCharacterSize(16);
	scoreText.setPosition(100, 580);
	scoreText.setColor(sf::Color::Black);
	
	lastScoreText.setFont(arialFont);
	lastScoreText.setCharacterSize(22);
	lastScoreText.setPosition(100, 550);
	lastScoreText.setColor(sf::Color::Black);
	lastScoreText.setString("Your last score will be displayed here.");
	
	difficulty = 1;
	enemySpawnClock = 5.f;
	lives = 3;
	gunCooldown = 0;
	livesChanged = true;
	scoreChanged = true;
	score = 0;
}

Game::~Game()
{
}

void Game::logic()
{
	// Update difficulty
	difficulty += .01 * dTime;
	
	// Move Ship and fire
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		shipSprite.move(-256 * dTime * difficulty, 0);
		if (shipSprite.getPosition().x <= -32) {
			shipSprite.setPosition(-32, shipSprite.getPosition().y);
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		shipSprite.move(256 * dTime * difficulty, 0);
		if (shipSprite.getPosition().x >= 768) {
			shipSprite.setPosition(768, shipSprite.getPosition().y);
		}
	}
	
	gunCooldown -= dTime * difficulty;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && gunCooldown <= 0) { //if space is pressed, and cooldown is 0
		gunCooldown = .5;
		sf::Sprite newBullet;
		newBullet.setTexture(bulletTex);
		newBullet.setPosition(shipSprite.getPosition().x + 24, 480);
		bulletVector.push_back(newBullet);
	}
	
	//Spawn enemies
	enemySpawnClock = enemySpawnClock - (dTime * difficulty);
	if (enemySpawnClock <= 0) { //spawn an enemy
		enemySpawnClock = 0.1 * rand.getRand(2, 50 * (1/difficulty));
		sf::Sprite newEnemySprite;
		newEnemySprite.setTexture(enemyTex);
		newEnemySprite.setPosition(rand.getRand(0, 736), -64);
		enemyVector.push_back(newEnemySprite);
	}
	
	//Move enemies
	for (auto& i : enemyVector) {
		i.move(0, 64 * dTime * difficulty);
	}
	
	//Move bullets
	for (auto& i : bulletVector) {
		i.move(0, -200 * dTime * difficulty);
	}
	
	//Check bullet collision
	std::vector<sf::Sprite>::iterator it;
	for (it = bulletVector.begin(); it != bulletVector.end();) {
		bool deleteBullet = false;
		std::vector<sf::Sprite>::iterator it2;
		for (it2 = enemyVector.begin(); it2 != enemyVector.end();) {
			if (it->getGlobalBounds().intersects(it2->getGlobalBounds())) {
				enemyVector.erase(it2);
				deleteBullet = true;
				score += floor(100 * difficulty);
				scoreChanged = true;
			} else {
				it2++;
			}
		}
		if (deleteBullet) {
			bulletVector.erase(it);
		} else {
			it++;
		}
	}
	
	//cleanup bullets and enemies
	for (it = enemyVector.begin(); it != enemyVector.end();) {
		if (it->getPosition().y >= 600) {
			enemyVector.erase(it);
			lives--;
			livesChanged = true;
		} else {
			it++;
		}
	}

	for (it = bulletVector.begin(); it != bulletVector.end();) {
		if (it->getPosition().y >= 664) {
			bulletVector.erase(it);
		} else {
			it++;
		}
	}
	
	//Display score and lives
	if (livesChanged) {
		livesChanged = false;
		std::stringstream livesStream;
		livesStream << lives;
		livesText.setString("Lives: " + livesStream.str());
	}
	
	if (scoreChanged) {
		scoreChanged = false;
		std::stringstream scoreStream;
		scoreStream << score;
		scoreText.setString("Score: " + scoreStream.str());
	}
	
	//Move bullets
	for (auto& i : bulletVector) {
		i.move(0, -200 * dTime * difficulty);
	}
	
	//LOSE IT ALLLLLLLLL
	if (lives <= -1) {
		//Reset stuff
		lastScore = score;
		splashScreen = true;
		difficulty = 1;
		enemySpawnClock = 5.f;
		lives = 3;
		gunCooldown = 0;
		livesChanged = true;
		scoreChanged = true;
		score = 0;
		hasLastScore = true;
		bulletVector.clear();
		enemyVector.clear();
		
		//Last Score text
		std::stringstream ss;
		ss << lastScore;
		lastScoreText.setString("Your Last Score: " + ss.str());
	}
	
	//Draw stuff
	for (auto& i : enemyVector) {
		window.draw(i);
	}
	for (auto& i : bulletVector) {
		window.draw(i);
	}
	window.draw(shipSprite);
	window.draw(livesText);
	window.draw(scoreText);
}

void Game::loop()
{
	while (running) {
		dTime = loopClock.restart().asSeconds();
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type)
			{
				case sf::Event::Closed:
					running = false;
					break;
				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left) {
						splashScreen = false;
					}
					break;
				default:
					break;
			}
		}
		window.clear(sf::Color::White);
		if (splashScreen) {
			splash();
		} else {
			logic();
		}
		window.display();
	}
}

void Game::splash()
{
	window.draw(splashSprite);
	window.draw(lastScoreText);
}

void Game::start()
{
	running = true;
	splashScreen = true;
	window.create(sf::VideoMode(800,600), "Tax Blaster", sf::Style::Close);
	window.setVerticalSyncEnabled(true);
	loop();
}

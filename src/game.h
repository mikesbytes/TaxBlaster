#ifndef GAME_H
#define GAME_H

#include <vector>

#include "SFML/Graphics.hpp"
#include "random.h"

class Game
{
public:
	Game();
	~Game();

	void start();
	void loop();
	
	void logic();
	void splash();
	
private:
	bool running;
	bool splashScreen;
	sf::RenderWindow window;
	
	//Textures
	sf::Texture enemyTex;
	sf::Texture splashTex;
	sf::Texture shipTex;
	sf::Texture bulletTex;
	
	//Fonts
	sf::Font arialFont;
	
	//Sprites
	sf::Sprite splashSprite;
	sf::Sprite shipSprite;
	std::vector<sf::Sprite> enemyVector; //enemies
	std::vector<sf::Sprite> bulletVector; //bullets
	
	//Texts
	sf::Text livesText;
	sf::Text scoreText;
	sf::Text lastScoreText;
	
	//Logic Data
	sf::Clock loopClock;
	Random rand; //RNG
	float dTime;
	float enemySpawnClock; //how fast enemies spawn
	float difficulty; //speed up the game
	float gunCooldown;
	unsigned score;
	bool scoreChanged;
	int lives;
	bool livesChanged;
	unsigned lastScore;
	bool hasLastScore;
};

#endif // GAME_H

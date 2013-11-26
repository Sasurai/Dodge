/* **************************************************************************************** *
	Copyright (c) 2013 Jorge Martínez Vargas												*
*																							*
*	Permission is hereby granted, free of charge, to any person obtaining a copy			*
*	of this software and associated documentation files (the "Software"), to deal			*
*	in the Software without restriction, including without limitation the rights			*
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell				*
*	copies of the Software, and to permit persons to whom the Software is					*
*	furnished to do so, subject to the following conditions:								*
*																							*
*	The above copyright notice and this permission notice shall be included in				*
*	all copies or substantial portions of the Software.										*
*																							*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR				*
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,				*
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE				*
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER					*
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,			*
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN				*
*	THE SOFTWARE.																			*
*	 ____________________________________________________									*
*	| This file is part of the "Dodge!" game source code.|									*
*	|	gamejolt.com/games/other/dodge/18599/			 |									*
*	 ----------------------------------------------------									*
*																							*
* ***************************************************************************************** */

#ifndef GAMECONTROLLER_H_
#define GAMECONTROLLER_H_

#include <SFML/Graphics/RenderWindow.hpp>
#include "GameObject.h"
#include "GameState.h"

// Initial parameters for the game
#define INI_INITIAL_VELOCITY 3
#define INI_INITIAL_IMPULSE 3
#define INI_TIME_BETWEEN_IMPULSES 1
#define INI_SPAWN_INTERVAL 3
#define INI_SPAWN_NUMBER 1

// Parameter limits for the enemies
#define MAX_INITIAL_VELOCITY 10
#define MAX_INITIAL_IMPULSE 10
#define MIN_TIME_BETWEEN_IMPULSES 0.25
#define MIN_INITIAL_DELAY 1
// Spawn interval and number limits
#define MIN_SPAWN_INTERVAL 1
#define MAX_SPAWN_NUMBER 4
#define MAX_ACTIVE_MINES 4
// Time between events (level increase and powerup creation)

#define LEVEL_INCREASE_TIME 2
#define TIME_BETWEEN_POWERUPS 10

using namespace std;

/*
	GameController class takes care of enemy and powerup spawning, and therefore of difficulty increase
	over time. It also takes care of generating the particles when an enemy explodes, and the fireballs
	when a mine does.
*/
class GameController
{
public:
	GameController(GameState *gameState);
	~GameController();
	void update(double dt);
	void setPaused(bool paused);
	void resetGame();
	void explodeMine(b2Vec2 &source);
	void generateParticles(b2Vec2 &source);
	int getInternalLevel(){ return m_internalLevel; } // Used by GameState for score calculations

private:
	GameState *m_gameState; // m_gameState is used to add the generated enemies and other stuff to the GameState
	bool m_paused;
	float m_initialDelay;
	float m_initialVelocity;
	float m_initialImpulse;
	float m_timeBetweenImpulses;
	float m_spawnInterval;
	int m_spawnNumber;
	double m_timeSinceLastIncrease;
	double m_timeSinceLastSpawn;
	double m_timeSinceLastPowerup;
	int m_internalLevel;
	double m_totalTime;
	int m_mineCount;

	// Private methods
	b2Vec2 m_generateRandomPosition();
};

#endif
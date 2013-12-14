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

#include <time.h>
#include <stdlib.h>

#include "GameController.h"
#include "ImpulseEnemy.h"
#include "VelocityEnemy.h"
#include "Mine.h"
#include "Player.h"
#include "Powerup.h"
#include "Particle.h"
#include "Fireball.h"

GameController::GameController(GameState *gameState) : m_gameState(gameState),
	m_initialVelocity(INI_INITIAL_VELOCITY),
	m_initialImpulse(INI_INITIAL_IMPULSE),
	m_timeBetweenImpulses(INI_TIME_BETWEEN_IMPULSES),
	m_spawnInterval(INI_SPAWN_INTERVAL),
	m_spawnNumber(INI_SPAWN_NUMBER),
	m_timeSinceLastIncrease(0.0),
	m_timeSinceLastSpawn(INI_SPAWN_INTERVAL+1),
	m_internalLevel(1),
	m_timeSinceLastPowerup(0.0),
	m_initialDelay(2),
	m_mineCount(0)
{
	m_paused = true; // Start the game paused
	Player *player = new Player(gameState); // With only the player created, 
	player->setPosition(b2Vec2(0.0f, 0.0f)); // at the center of the screen
	m_gameState->setPlayer(player);

	// Initialize the random generator
	srand(time(NULL));
}

GameController::~GameController(){}

void GameController::update(double dt){
	m_timeSinceLastIncrease += dt;
	// Check if you have to increase level and do the proper modifications
	if(m_timeSinceLastIncrease > LEVEL_INCREASE_TIME){
		m_internalLevel++;
		if(m_internalLevel%2 == 0 && m_initialVelocity < MAX_INITIAL_VELOCITY)
			m_initialVelocity += 1.0;
		if(m_internalLevel%2 == 0 && m_initialImpulse < MAX_INITIAL_IMPULSE)
			m_initialImpulse += 0.25;
		if(m_internalLevel%2 == 0 && m_timeBetweenImpulses > MIN_TIME_BETWEEN_IMPULSES)
			m_timeBetweenImpulses -= 0.25;
		if(m_internalLevel%2 == 0 && m_initialDelay > MIN_INITIAL_DELAY)
			m_initialDelay -= 0.10;
		if(m_internalLevel%3 == 0 && m_spawnInterval > MIN_SPAWN_INTERVAL)
			m_spawnInterval -= 0.2;
		if(m_internalLevel%5 == 0 && m_spawnNumber < MAX_SPAWN_NUMBER)
			m_spawnNumber++;
		m_timeSinceLastIncrease = 0;
	}
	m_timeSinceLastSpawn += dt;
	m_timeSinceLastPowerup += dt;
	if(m_timeSinceLastSpawn > m_spawnInterval){
		// Can't spawn more than one mine each spawn round
		bool mineSpawned = false;
		if(m_spawnNumber < 2) // And mines won't appear when under 2 spawns a time
			mineSpawned = true;
		for(int i=0; i<m_spawnNumber; i++){
			// Determine the kind of enemy to spawn
			int type = rand()%3;
			if(type==TYPE_MINE){
				if(mineSpawned || m_mineCount >= MAX_ACTIVE_MINES){ // Number of active mines is also limited
					type = TYPE_VELOCITY;
				}else{
					Mine *newEnemy = new Mine(m_gameState);
					newEnemy->setPosition(m_generateRandomPosition());
					m_gameState->addObject(newEnemy);
					mineSpawned = true;
					m_mineCount++;
				}
			}
			if(type==TYPE_VELOCITY){
				VelocityEnemy *newEnemy = new VelocityEnemy(m_gameState, m_initialDelay);
				newEnemy->setVelocity(m_initialVelocity);
				newEnemy->setPosition(m_generateRandomPosition());
				m_gameState->addObject(newEnemy);
			}else{ // TYPE_IMPULSE
				ImpulseEnemy *newEnemy = new ImpulseEnemy(m_gameState, m_initialDelay);
				newEnemy->setImpulse(m_initialImpulse);
				newEnemy->setTimeBetweenImpulses(m_timeBetweenImpulses);
				newEnemy->setPosition(m_generateRandomPosition());
				m_gameState->addObject(newEnemy);
			}
		}
		m_timeSinceLastSpawn = 0;
	}
	if(m_timeSinceLastPowerup > TIME_BETWEEN_POWERUPS){
		// Determine the kind of powerup to spawn
		int type = rand()%4 + 10;
		Powerup *newPow = new Powerup(m_gameState, type);
		newPow->setPosition(m_generateRandomPosition());
		m_gameState->addObject(newPow);
		m_timeSinceLastPowerup = 0;
	}
}

void GameController::setPaused(bool paused){
	m_paused = paused;
}

/*
	Random position generation that takes into account where the player is.
	If the generated position is too close to the player its modified to a safe distance.
*/
b2Vec2 GameController::m_generateRandomPosition(){
	b2Vec2 pos;
	pos.x = (rand()%GAMESIZE - GAMESIZE/2);
	pos.y = (rand()%GAMESIZE - GAMESIZE/2);
	b2Vec2 playerPos = m_gameState->getPlayer()->getPosition();
	// Make sure it doesn't spawn too close to (or on top of) the player
	if(pos.x > playerPos.x){
		if(pos.x - playerPos.x < 5)
			pos.x = pos.x + 5;
	}else if(pos.x - playerPos.x > -5)
		pos.x = pos.x -5;
	
	if(pos.y > playerPos.y){
		if(pos.y - playerPos.y < 5)
			pos.y = pos.y + 5;
	}else if(pos.y - playerPos.y > -5)
		pos.y = pos.y -5;

	return pos;
}

// Method for restarting the game after player's death
void GameController::resetGame(){
	m_paused = true; // Start the game paused
	Player *player = new Player(m_gameState); // The "Player" instance is destroyed at the end of the game, so each new game we create a new one
	player->setPosition(b2Vec2(0.0f, 0.0f)); // Screen center
	m_gameState->setPlayer(player);

	// Initialize the counters and timers
	m_initialVelocity = INI_INITIAL_VELOCITY;
	m_initialImpulse = INI_INITIAL_IMPULSE;
	m_timeBetweenImpulses = INI_TIME_BETWEEN_IMPULSES;
	m_spawnInterval = INI_SPAWN_INTERVAL;
	m_spawnNumber = INI_SPAWN_NUMBER;
	m_timeSinceLastIncrease = 0.0;
	m_timeSinceLastSpawn = m_spawnInterval + 1;
	m_timeSinceLastPowerup = 0;
	m_internalLevel = 1;
	m_mineCount = 0;
	// Initialize the random generator
	srand(time(NULL));
}

void GameController::generateParticles(b2Vec2 &source){
	for(int i=0;i<NUM_SPAWN;i++){
		b2Vec2 tar = m_generateRandomPosition();
		// Calculate the distance vector
		float dx, dy;
		dx = tar.x - source.x;
		dy = tar.y - source.y;
		// Normalize the distance (we want only the direction)
		float d = sqrt(dx*dx + dy*dy);
		dx = dx/d;
		dy = dy/d;
		// Calculate velocity
		dx *= PARTICLE_VELOCITY;
		dy *= PARTICLE_VELOCITY;
		Particle *part = new Particle(m_gameState,source,b2Vec2(dx,dy));
		m_gameState->addObject(part);
	}
}	

/*
	When a mine explodes we need to generate FIREBALL_NUMBER fireballs, and also register that we have one
	less mine in the game (since the total number of active mines is limited)
*/
void GameController::explodeMine(b2Vec2 source){
	for(int i=0;i<FIREBALL_NUMBER;i++){
		b2Vec2 tar = m_generateRandomPosition();
		// Calculate the distance vector
		float dx, dy;
		dx = tar.x - source.x;
		dy = tar.y - source.y;
		// Normalize the distance (we want only the direction)
		float d = sqrt(dx*dx + dy*dy);
		dx = dx/d;
		dy = dy/d;
		// Calculate velocity
		dx *= FIREBALL_VELOCITY;
		dy *= FIREBALL_VELOCITY;
		Fireball *fball = new Fireball(m_gameState);
		fball->setPosition(source);
		fball->setVelocity(b2Vec2(dx,dy));
		m_gameState->addObject(fball);
	}
	m_mineCount--;
}	
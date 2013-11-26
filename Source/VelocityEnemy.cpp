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

#include "VelocityEnemy.h"
#include "GameState.h"
#include "Player.h"

VelocityEnemy::VelocityEnemy(GameState *gameState, float initialDelay) :
	Enemy(gameState, initialDelay),
	m_velocity(0.0)
{
	setColor(0.75f, 0.45f,0.0f);
	sf::Texture *texture = m_gameState->getTextureManager().getTexture("Textures/Enemy1.png");
	setTexture(*texture);
}

VelocityEnemy::~VelocityEnemy(void)
{
}

void VelocityEnemy::setVelocity(float velocity){
	m_velocity = velocity;
}

void VelocityEnemy::update(double dt)
{
	double slowTime = m_gameState->getSlowTime();
	if(!m_gameState->getPaused()){
		if(!m_active){
			m_initialDelay-=dt;
			if(m_initialDelay<0)
				m_active = true;
		}else{
			Player* player = m_gameState->getPlayer();
			b2Vec2 playerPos = player->getPosition();
			b2Vec2 ownPos = getPosition();
			// Calculate the distance to the player
			float dx, dy;
			dx = playerPos.x - ownPos.x;
			dy = playerPos.y - ownPos.y;
			// Normalize the distance (we want only the direction)
			float d = sqrt(dx*dx + dy*dy);
			dx = dx/d;
			dy = dy/d;
			// Multiply the direction and the velocity to obtain the new linear velocity vector
			float ndx = dx*m_velocity*slowTime;
			float ndy = dy*m_velocity*slowTime;
			m_body->SetLinearVelocity(b2Vec2(ndx, ndy));
		}
	}
}
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

#include "ImpulseEnemy.h"
#include "GameState.h"
#include "Player.h"

ImpulseEnemy::ImpulseEnemy(GameState *gameState, float initialDelay) :
	Enemy(gameState, initialDelay),
	m_impulse(0.0),
	m_timeSinceLastImpulse(0.0)
{
	setColor(1.0f,0.0f,0.0f);
	sf::Texture *texture = m_gameState->getTextureManager().getTexture("Textures/Enemy2.png");
	setTexture(*texture);
}

ImpulseEnemy::~ImpulseEnemy(void)
{
}

void ImpulseEnemy::setImpulse(float impulse){
	m_impulse = impulse;
}

void ImpulseEnemy::update(double dt){
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
			m_timeSinceLastImpulse+=dt;
			if(m_timeSinceLastImpulse>m_timeBetweenImpulses){
				float ndx = dx*m_impulse;
				float ndy = dy*m_impulse;
				m_body->ApplyLinearImpulse(b2Vec2(ndx,ndy),m_body->GetWorldCenter());
				m_timeSinceLastImpulse = 0.0;
				b2Vec2 vel = m_body->GetLinearVelocity();
				vel.x *= slowTime;
				vel.y *= slowTime;
				m_body->SetLinearVelocity(vel);
			}
		}
	}
}

void ImpulseEnemy::setTimeBetweenImpulses(float impInterval){
	m_timeBetweenImpulses = impInterval;
}
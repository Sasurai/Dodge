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

#include <math.h>

#include "GameState.h"
#include "Player.h"
#include "Enemy.h"

Enemy::Enemy(GameState *gameState, float initialDelay) : 
	PhysicalObject(gameState),
	m_initialDelay(initialDelay)
{
	if(m_initialDelay<0)
		m_active = true;
	else
		m_active = false;
	addProperty("enemy");
	getBodyDef()->type = b2_dynamicBody;
	getBodyDef()->fixedRotation = true;
	b2FixtureDef *fixDef = new b2FixtureDef;
	b2CircleShape *shape = new b2CircleShape;
	shape->m_radius = 1.0f;
	setSize(b2Vec2(2.0f,2.0f));
	fixDef->shape = shape;
	fixDef->density = 0.0f;
	fixDef->restitution = 0.0f;
	addFixtureDef(fixDef);
	setRotation(0.0f);
	m_lightTexture = m_gameState->getTextureManager().getTexture("Textures/Light.png");
	m_velocityBeforePause.x = m_velocityBeforePause.y = 0;
}

Enemy::~Enemy(){}

void Enemy::update(double dt)
{
	if(!m_gameState->getPaused() && !m_active){
		m_initialDelay-=dt;
		if(m_initialDelay<0)
			m_active = true;
	}
}

void Enemy::beginContact(b2Contact *contact)
{
	PhysicalObject *objectA = (PhysicalObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysicalObject *objectB = (PhysicalObject*)contact->GetFixtureB()->GetBody()->GetUserData();
	PhysicalObject *obj;
	if(objectA == this) // objectB is what you contacted
		obj = objectB;
	else
		obj = objectA;
	if(obj->hasProperty("enemy") && obj->isActive() && m_active){ // Crashed with another enemy
		kill(); // Only dies if both the enemy and itself are active
	}else if(obj->hasProperty("player") && m_active){ // Crashed with the player 
		kill(); //Only dies if it is active itself
	}
}

void Enemy::endContact(b2Contact *contact)
{
}

void Enemy::pause(){
	m_velocityBeforePause = m_body->GetLinearVelocity();
	m_body->SetLinearVelocity(b2Vec2(0.0, 0.0));
}

void Enemy::unpause(){
	m_body->SetLinearVelocity(m_velocityBeforePause);
}

void Enemy::draw(){
	if(!m_visible)
		return;
	if(!m_body)
		return;
	b2Vec2 position = getPosition();
	b2Vec2 oldPosition = position;
	bool outOfScreen = false;
	if(!m_active){ // If it's not active we show a light where it's going to appear
		float alpha = 1-m_initialDelay; // Light visibility depends on time left to pop
		if(alpha<=0)
			alpha = 0.1;
		glColor4f(m_color[0],m_color[1],m_color[2],alpha);
		if(m_lightTexture)
			sf::Texture::bind(m_lightTexture);
	}else{
		// Check if it's out of screen
		if(position.x>GAMESIZE/2){
			outOfScreen = true;
			position.x = GAMESIZE/2;
		}
		if(position.x<-GAMESIZE/2){
			outOfScreen = true;
			position.x = -GAMESIZE/2;
		}
		if(position.y>GAMESIZE/2){
			outOfScreen = true;
			position.y = GAMESIZE/2;
		}
		if(position.y<-GAMESIZE/2){
			outOfScreen = true;
			position.y = -GAMESIZE/2;
		}
		if(outOfScreen){ // If out of screen we also show a light to indicate where it will come from when reentering game screen
			setPosition(position); // Temporarily set the position inside the screen, only for drawing
			glColor4fv(m_color);
			if(m_lightTexture)
				sf::Texture::bind(m_lightTexture);
		}else if(m_texture)
			sf::Texture::bind(m_texture);
		else
			glColor4fv(m_color);
	}
	m_drawCurrentTexture();
	if(outOfScreen) // Restore actual position out of screen
		setPosition(oldPosition);
	sf::Texture::bind(NULL);
}
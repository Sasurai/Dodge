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

#include "GameState.h"
#include "Player.h"
#include "Powerup.h"

Powerup::Powerup(GameState *gameState, int type) : PhysicalObject(gameState),
	m_type(type)
{
	addProperty("powerup");
	getBodyDef()->type = b2_dynamicBody;
	getBodyDef()->fixedRotation = true; 
	b2FixtureDef *fixDef = new b2FixtureDef;
	b2PolygonShape *shape = new b2PolygonShape;
	shape->SetAsBox(1.0,1.0);
	fixDef->shape = shape;
	setSize(b2Vec2(2.0f,2.0f));
	fixDef->density = 0.0f;
	fixDef->restitution = 0.0f;
	addFixtureDef(fixDef);
	setRotation(0.0f);
	setColor(0.0f,1.0f,0.0f);
	sf::Texture *texture = 0;
	if(m_type == POINTS_POWERUP){
		texture = m_gameState->getTextureManager().getTexture("Textures/Points.png");
	}
	if(m_type == SHIELD_POWERUP){
		texture = m_gameState->getTextureManager().getTexture("Textures/Shield.png");
	}
	if(m_type == BOOM_POWERUP){
		texture = m_gameState->getTextureManager().getTexture("Textures/Boom.png");
	}
	if(m_type == SLOW_POWERUP){
		texture = m_gameState->getTextureManager().getTexture("Textures/SlowTime.png");
	}
	if(texture)
		setTexture(*texture);
}

Powerup::~Powerup() {}

void Powerup::update(double dt) {}

void Powerup::beginContact(b2Contact *contact){
	PhysicalObject *objectA = (PhysicalObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysicalObject *objectB = (PhysicalObject*)contact->GetFixtureB()->GetBody()->GetUserData();
	PhysicalObject *obj;
	if(objectA == this) // objectB is what you contacted
		obj = objectB;
	else
		obj = objectA;
	if(obj->hasProperty("player")){ // Powerup picked
		if(m_type == POINTS_POWERUP){
			m_gameState->increasePoints();
			m_gameState->getSoundManager().playSound(string("Sounds/Points.ogg"));
		}if(m_type == SHIELD_POWERUP){
			m_gameState->getPlayer()->giveShield();
			m_gameState->getSoundManager().playSound(string("Sounds/Shield.ogg"));
		}if(m_type == BOOM_POWERUP)
			m_gameState->killAll();
		if(m_type == SLOW_POWERUP){
			m_gameState->setSlowTime(0.1);
			m_gameState->getSoundManager().playSound(string("Sounds/Slow_Time.ogg"));
		}
		kill();
	}
}

void Powerup::endContact(b2Contact *contact){}

void Powerup::draw(){
	if(!m_visible)
		return;
	if(!m_body)
		return;
	if(m_texture)
		sf::Texture::bind(m_texture);
	else
		glColor4fv(m_color);
	b2Vec2 position = getPosition();
	float rotation = getRotation();
	b2Vec2 size = getSize();
	m_drawCurrentTexture();
	sf::Texture::bind(NULL);
}
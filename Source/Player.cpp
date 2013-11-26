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

#include "Player.h"
#include "Enemy.h"

Player::Player(GameState *gameState) : PhysicalObject(gameState),
	m_shieldTimeLeft(0.0)
{
	addProperty("player");
	getBodyDef()->type = b2_dynamicBody;
	getBodyDef()->fixedRotation = true; 
	b2FixtureDef *fixDef = new b2FixtureDef;
	b2CircleShape *shape = new b2CircleShape;
	shape->m_radius = 1.0f;
	setSize(b2Vec2(2.0f,2.0f));
	setRotation(0);
	fixDef->shape = shape;
	fixDef->density = 0.0f;
	fixDef->restitution = 0.0f;
	addFixtureDef(fixDef);
	setRotation(0.0f);
	setColor(0.0f,0.0f,1.0f);
	sf::Texture *texture = m_gameState->getTextureManager().getTexture("Textures/Player.png");
	setTexture(*texture);
	m_alternativeTexture = m_gameState->getTextureManager().getTexture("Textures/PlayerShield.png");
}

Player::~Player(){}

void Player::update(double dt)
{
	if(!m_gameState->getPaused()){
		sf::Vector2i mousePos = m_gameState->getMousePosition();
		float mouseX, mouseY; // We need to convert from window to game coordinates
		mouseX = -(float(GAMESIZE)/2) + mousePos.x*float(GAMESIZE)/WINDOWSIZE;
		mouseY = (float(GAMESIZE)/2) - mousePos.y*float(GAMESIZE)/WINDOWSIZE;
		if(mouseX < - GAMESIZE/2)
			mouseX = -GAMESIZE/2;
		else if(mouseX > GAMESIZE/2)
			mouseX = GAMESIZE/2;
		if(mouseY < -GAMESIZE/2)
			mouseY = -GAMESIZE/2;
		else if(mouseY > GAMESIZE/2)
			mouseY = GAMESIZE/2;
		setPosition(b2Vec2(mouseX,mouseY)); // The player follows the mouse
		m_shieldTimeLeft -= dt;
	}

}

void Player::beginContact(b2Contact *contact)
{
	PhysicalObject *objectA = (PhysicalObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	PhysicalObject *objectB = (PhysicalObject*)contact->GetFixtureB()->GetBody()->GetUserData();
	PhysicalObject *obj;
	if(objectA == this) // objectB is what you contacted
		obj = objectB;
	else
		obj = objectA;
	if(m_shieldTimeLeft<=0 && obj->hasProperty("enemy")){ // Case it hits an enemy and has no shield
		if(((Enemy*)obj)->isActive()){
			kill();
			m_body->SetLinearVelocity(b2Vec2(0,0));
		}
	}
}

void Player::endContact(b2Contact *contact)
{
}

void Player::mousePressed(sf::Vector2i &mousePos){
	b2Vec2 playerPos = getPosition();
	float mouseX, mouseY; // We need to convert from window to game coordinates
	mouseX = -(float(GAMESIZE)/2) + mousePos.x*float(GAMESIZE)/WINDOWSIZE;
	mouseY = (float(GAMESIZE)/2) - mousePos.y*float(GAMESIZE)/WINDOWSIZE;
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		mouseX + 1 > playerPos.x && mouseX - 1 < playerPos.x &&
		mouseY + 1 > playerPos.y && mouseY - 1 < playerPos.y){
			m_gameState->togglePause(); // Clicking on the player pauses/unpauses the game
	}
}

void Player::draw(){
	if(!m_visible)
		return;
	if(!m_body)
		return;
	if(m_shieldTimeLeft > 0 && m_alternativeTexture)
		sf::Texture::bind(m_alternativeTexture);
	else if(m_texture)
		sf::Texture::bind(m_texture);
	else
		glColor4fv(m_color);
	m_drawCurrentTexture();
	sf::Texture::bind(NULL);
}

void Player::giveShield(){
	m_shieldTimeLeft = 2.0;
}
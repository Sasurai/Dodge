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

#include "Fireball.h"

Fireball::Fireball(GameState *gameState) : Enemy(gameState, -1), m_timeToLive(FIREBALL_LIFE_TIME)
{
	addProperty("fireball");
	// Initialize with a random color
	float red = m_randomColorComponent();
	float green = m_randomColorComponent();
	float blue = m_randomColorComponent();
	setColor(red,green,blue);
	m_texture = m_gameState->getTextureManager().getTexture("Textures/Fireball.png");
}

Fireball::~Fireball(void)
{
}

void Fireball::update(double dt){
	m_timeToLive-=dt;
	if(m_timeToLive < 0)
		kill();
}

void Fireball::draw(){
	glColor4fv(m_color);
	Enemy::draw();
}

void Fireball::setVelocity(b2Vec2 velocity){
	getBodyDef()->linearVelocity = velocity;
}

float Fireball::m_randomColorComponent(){
	return float(rand()%100)/100;
}
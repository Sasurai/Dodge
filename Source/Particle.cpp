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

#include "Particle.h"

Particle::Particle(GameState *gameState, b2Vec2 &pos, b2Vec2 &vel) : GameObject(gameState), m_velocity(vel), m_timeToLive(LIFE_TIME)
{
	setSize(b2Vec2(0.2f,0.2f));
	m_red = m_randomColorComponent();
	m_green = m_randomColorComponent();
	m_blue = m_randomColorComponent();
	setPosition(pos);
	m_texture = m_gameState->getTextureManager().getTexture("Textures/Particle.png");
}

Particle::~Particle(void){}

void Particle::update(double dt){
	if(m_timeToLive<0)
		kill();
	if(!m_gameState->getPaused()){
		m_timeToLive -= dt;
		b2Vec2 pos = getPosition();
		pos.x = pos.x + m_velocity.x*dt;
		pos.y = pos.y + m_velocity.y*dt;
		setPosition(pos);
	}
}

void Particle::pause(){
	m_velocityBeforePause = m_velocity;
	m_velocity.x = m_velocity.y = 0;
}

void Particle::unpause(){
	m_velocity = m_velocityBeforePause;
}

void Particle::draw(){
	if(!m_visible)
		return;
	glColor4f(m_red, m_green, m_blue, m_timeToLive/LIFE_TIME);
	if(m_texture)
		sf::Texture::bind(m_texture);
	m_drawCurrentTexture();
	sf::Texture::bind(NULL);
}

float Particle::m_randomColorComponent(){
	return float(rand()%100)/100;
}
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

#ifndef PARTICLE_H
#define PARTICLE_H

#define LIFE_TIME 2
#define NUM_SPAWN 25
#define PARTICLE_VELOCITY 5

#include "GameObject.h"

class Particle : public GameObject
{
public:
	Particle(GameState *gameState, b2Vec2 &pos, b2Vec2 &vel);
	~Particle(void);

	virtual void update(double dt);
	void pause();
	void unpause();

	virtual void draw();
	
private:
	double m_timeToLive;

	b2Vec2 m_position;
	b2Vec2 m_velocity; 
	
	float m_red, m_green, m_blue;
	
	// We need to save the velocity before pausing to be able to restore it
	b2Vec2 m_velocityBeforePause;

	// Private method for generating random color components
	float m_randomColorComponent();
};

#endif
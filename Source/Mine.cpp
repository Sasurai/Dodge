/* **************************************************************************************** *
	Copyright (c) 2013 Jorge Mart�nez Vargas												*
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

#include "Mine.h"

Mine::Mine(GameState *gameState) : Enemy(gameState)
{
	setColor(0.8f, 0.8f,0.8f);
	sf::Texture *texture = m_gameState->getTextureManager().getTexture("Textures/Mine.png");
	setTexture(*texture);
	addProperty("mine");
}

Mine::~Mine()
{
}

void Mine::kill(){
	Enemy::kill();
	// Play explosion sound
	m_gameState->getSoundManager().playSound(string("Sounds/Explosion3.ogg"));
	// Trying to generate the explosion from here causes a crash that I can't figure out
}
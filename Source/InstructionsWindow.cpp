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

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "InstructionsWindow.h"
#include "GameState.h"

InstructionsWindow::InstructionsWindow(sf::RenderWindow &window) : BaseWindow(window, 2)
{
}

InstructionsWindow::~InstructionsWindow()
{
}

void InstructionsWindow::setImage(sf::Texture *texture){
	m_texture = texture;
}

// The instructions window is just an image with the instructions on it
void InstructionsWindow::draw(){	
	if(m_texture)
		sf::Texture::bind(m_texture);
	glPushMatrix();
	{
		glTranslatef(0.0,0.0,0);
		glScalef(GAMESIZE, GAMESIZE, 0.0f);
		glBegin(GL_QUADS);
		{
			glTexCoord2d(0.0f,1.0f);
			glVertex3f( -0.5f, -0.5f,  0.0f);
			glTexCoord2d(1.0f,1.0f);
			glVertex3f( 0.5f, -0.5f,  0.0f);
			glTexCoord2d(1.0f,0.0f);
			glVertex3f( 0.5f, 0.5f,  0.0f);
			glTexCoord2d(0.0f,0.0f);
			glVertex3f( -0.5f, 0.5f,  0.0f);
		}
		glEnd();
	}
	glPopMatrix();
	sf::Texture::bind(NULL);
}

int InstructionsWindow::checkEvents(){
	sf::Event event;
	m_nextState = 2;
    while (m_window.pollEvent(event))
	{
        if(event.type == sf::Event::Closed)
            return -1;		
		if(event.type == sf::Event::MouseButtonPressed){ // Go back to menu
			m_nextState = 0;
		}	
    }
	return 0;
}